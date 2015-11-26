// *********************************************************************** //
//
// MODULE  : win32_ltfilewrite.h
//
// PURPOSE : Win32 implementation for ILTFileWrite.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __WIN32_LTFILEWRITE_H__
#define __WIN32_LTFILEWRITE_H__

#ifndef __ILTFILEWRITE_H__
#include "iltfilewrite.h"
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

class CWin32_LTFileWrite : private ILTFileWrite,
						   private CWin32_LTFileBase
{
public:

	CWin32_LTFileWrite() {}
	virtual ~CWin32_LTFileWrite() {}

	// open the file for writing.  This will close a previously opened file prior
	// to opening the new file.  If bAppend is false, this will truncate the
	// file, otherwise it will be opened in append mode.  The file is created
	// if it does not exist.
	virtual bool Open(const char* pszFilename, const bool bAppend);

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	virtual bool GetIsOpen(bool& bIsOpen);

	// write nBytesToWrite bytes to the file from the buffer - the number of bytes
	// written is returned in the nBytesWritten parameter.  Returns false if the
	// number of bytes specified cannot be written, or if an error occurs during 
	// the write operation.
	virtual bool Write(const void* const pBuffer, const uint32 nBytesToWrite);

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

	// prevent copy and assignment
	PREVENT_OBJECT_COPYING(CWin32_LTFileWrite);

};

inline bool CWin32_LTFileWrite::Open(const char* pszFilename, const bool bAppend)
{
	CWin32_LTFileBase::EOpenMode eOpenMode;

	if (bAppend)
	{
		eOpenMode = eOpenWriteAppend;
	}
	else
	{
		eOpenMode = eOpenWrite;
	}

	if (!OpenImpl(pszFilename, eOpenMode))
	{
		return false;
	}

	// if the append flag is set, we need to seek to the end of the file
	if (bAppend)
	{
		if (!SeekToEnd())
		{
			return false;
		}
	}

	// success
	return true;
}

inline bool CWin32_LTFileWrite::GetIsOpen(bool& bIsOpen)
{
	return GetIsOpenImpl(bIsOpen);
}

inline bool CWin32_LTFileWrite::Write(const void* const pBuffer, const uint32 nBytesToWrite)
{
	LTASSERT(GetFileHandle() != INVALID_HANDLE_VALUE, "invalid file handle");

	DWORD nBytesWritten = 0;
	BOOL  bResult = ::WriteFile(GetFileHandle(), pBuffer, nBytesToWrite, &nBytesWritten, NULL);

	// check to see if the call failed, or if we did not actually write the number of 
	// bytes specified by the caller
	if (!bResult || (nBytesWritten != nBytesToWrite))
	{
		return false;
	}

	// success
	return true;
}

inline bool CWin32_LTFileWrite::Seek(uint64 nPos)
{ 
	return SeekImpl(nPos); 
}

inline bool CWin32_LTFileWrite::SeekToEnd()
{
	return SeekToEndImpl();
}

inline bool CWin32_LTFileWrite::GetPos(uint64& nPos)
{
	return GetPosImpl(nPos);
}

inline bool CWin32_LTFileWrite::GetIsEOF(bool& bIsEOF)
{
	return GetIsEOFImpl(bIsEOF);
}

inline bool CWin32_LTFileWrite::GetFileSize(uint64& nSize)
{
	return GetFileSizeImpl(nSize);
}

inline bool CWin32_LTFileWrite::Close()
{
	return CloseImpl();
}

#endif // __WIN32_LTFILEWRITE_H__
