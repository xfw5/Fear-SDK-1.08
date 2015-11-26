// *********************************************************************** //
//
// MODULE  : win32_ltfileread.h
//
// PURPOSE : Win32 implementation for ILTFileRead.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __WIN32_LTFILEREAD_H__
#define __WIN32_LTFILEREAD_H__

#ifndef __ILTFILEREAD_H__
#include "iltfileread.h"
#endif

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTASSERT_H__
#include "ltassert.h"
#endif

#ifndef __WIN32_LTFILEBASE_H__
#include "win32_ltfilebase.h"
#endif

#ifndef __WIN32_LTFILEBUFFER_H__
#include "win32_ltfilebuffer.h"
#endif

class CWin32_LTFileRead : private ILTFileRead,
						  private CWin32_LTFileBase
{
public:

	CWin32_LTFileRead();
	virtual ~CWin32_LTFileRead();

	// open the file for reading.  This will close a previously opened file prior
	// to opening the new file.  Returns false if the file does not exist on disk.
	virtual bool Open(const char* pszFilename);

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	virtual bool GetIsOpen(bool& bIsOpen);

	// read nBytesToRead bytes from the file into the buffer - the buffer is assumed to be 
	// large enough to hold the data.  This returns false if the number of bytes
	// specified cannot be read, or if there is an error during the read operation.
	virtual bool Read(void* const pBuffer, const uint32 nBytesToRead);

	// seek to the specified position in the file relative to the start of the file.
	// Returns false if an error occurs during the seek.
	virtual bool Seek(uint64 nPos);
	
	// seek to the current end of file.  Returns false if an error occurs during the
	// seek operation.
	virtual bool SeekToEnd();

	// return the current position based on the start of the file.  Returns false if 
	// an error occurs while retrieving the position.
	virtual bool GetPos(uint64& nPos);

	// test to see if the file is at EOF.  Returns false if an error occurs while
	// testing for EOF.
	virtual bool GetIsEOF(bool& bIsEOF);

	// return the size of the currently opened file.  Returns false if an error occurs
	// while determining the file size.
	virtual bool GetFileSize(uint64& nSize);

	// close the file.  Returns false if an error occurs while closing the file.
	virtual bool Close();

private:

	// helper for determining how much is left in the buffer
	uint32 GetRemainingBytesInBuffer() const { return m_nBufferAmount - m_nCurrentBufferPos; }
	
	// helper for copying data from the buffer
	bool GetDataFromBuffer(void* pDest, uint32 nAmountToCopy);
	
	// helper for reading from the file
	bool ReadFromFile(void* const pDest, const uint32 nAmountToRead, uint32& nAmountReadFromFile);

	// helper for filling the read buffer with data from the file
	bool FillBufferFromFile();

	// helper for resetting the buffer positions
	void ResetBuffer() { m_nBufferAmount = 0; m_nCurrentBufferPos = 0; }

	// file data buffer
	CWin32_LTFileBuffer* m_pFileBuffer;

	// current position in the buffer
	uint32 m_nCurrentBufferPos;

	// amount of data in the buffer, regardless of the current position
	uint32 m_nBufferAmount;

	// prevent copy and assignment
	PREVENT_OBJECT_COPYING(CWin32_LTFileRead);

};

inline CWin32_LTFileRead::CWin32_LTFileRead()
{
	// acquire a file buffer
	m_pFileBuffer = CWin32_LTFileBuffer::Allocate();

	ResetBuffer();
}

inline CWin32_LTFileRead::~CWin32_LTFileRead()
{
	// release the file buffer 
	m_pFileBuffer->Release();
}

inline bool CWin32_LTFileRead::Open(const char* pszFilename)
{
	return OpenImpl(pszFilename, eOpenRead);
}

inline bool CWin32_LTFileRead::GetIsOpen(bool& bIsOpen)
{
	return GetIsOpenImpl(bIsOpen);
}

inline bool CWin32_LTFileRead::Seek(uint64 nPos)
{ 
	// reset the buffer since we're moving the file pointer
	ResetBuffer();

	return SeekImpl(nPos); 
}

inline bool CWin32_LTFileRead::SeekToEnd()
{
	// reset the buffer since we're moving the file pointer
	ResetBuffer();

	return SeekToEndImpl();
}

inline bool CWin32_LTFileRead::GetPos(uint64& nPos)
{
	// call base class to get file position
	if (!GetPosImpl(nPos))
	{
		return false;
	}

	// adjust for file buffer
	nPos -= GetRemainingBytesInBuffer();

	return true;
}

inline bool CWin32_LTFileRead::GetIsEOF(bool& bIsEOF)
{
	// get current file position (call derived class version to account for buffering)
	uint64 nPos = 0;
	if (!GetPos(nPos))
	{
		return false;
	}

	// get file size
	uint64 nFileSize = 0;
	if (!GetFileSize(nFileSize))
	{
		return false;
	}
	
	// if equal, we're at EOF
	bIsEOF = (nPos == nFileSize);

	return true;
}

inline bool CWin32_LTFileRead::GetFileSize(uint64& nSize)
{
	return GetFileSizeImpl(nSize);
}

inline bool CWin32_LTFileRead::Close()
{
	return CloseImpl();
}

inline bool CWin32_LTFileRead::GetDataFromBuffer(void* const pDest, const uint32 nAmountToCopy)
{
	// make sure we have enough data in the buffer to satisfy the request
	if ((m_nCurrentBufferPos + nAmountToCopy) > m_nBufferAmount)
	{
		LTERROR("read from buffer exceeds amount of available buffer data");
		return false;
	}
	
	// copy the data
	::memcpy(pDest, m_pFileBuffer->GetBuffer() + m_nCurrentBufferPos, nAmountToCopy); 

	// move the current position
	m_nCurrentBufferPos += nAmountToCopy;

	// if we're at the end of the buffer, reset it
	if (m_nCurrentBufferPos == m_nBufferAmount)
	{
		ResetBuffer();
	}

	return true;
}

inline bool CWin32_LTFileRead::ReadFromFile(void* const pDest, const uint32 nAmountToRead, uint32& nAmountReadFromFile)
{
	return !!::ReadFile(GetFileHandle(), pDest, nAmountToRead, (LPDWORD)&nAmountReadFromFile, NULL);
}

inline bool CWin32_LTFileRead::FillBufferFromFile()
{
	if (ReadFromFile(m_pFileBuffer->GetBuffer(), k_nWin32FileBufferSize, m_nBufferAmount) == FALSE)
	{
		return false;
	}

	// reset the current position
	m_nCurrentBufferPos = 0;

	return true;
}

#endif // __WIN32_LTFILEREAD_H__
