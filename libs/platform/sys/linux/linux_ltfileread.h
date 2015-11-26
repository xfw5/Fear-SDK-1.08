// *********************************************************************** //
//
// MODULE  : linux_ltfileread.h
//
// PURPOSE : Linux implementation for ILTFileRead.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LINUX_LTFILEREAD_H__
#define __LINUX_LTFILEREAD_H__

#include "iltfileread.h"
#include "linux_ltfilebase.h"
#include "ltassert.h"

class CLinux_LTFileRead : private ILTFileRead,
						  private CLinux_LTFileBase
{
public:

	CLinux_LTFileRead() {}
	virtual ~CLinux_LTFileRead() {}

	// open the file for reading.  This will close a previously opened file prior
	// to opening the new file.  Returns false if the file does not exist on disk.
	virtual bool Open(const char* pszFilename);

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	virtual bool GetIsOpen(bool& bIsOpen);
	
	// read nSize bytes from the file into the buffer - the buffer is assumed to be 
	// large enough to hold the data.  The number of bytes read is returned in the
	// nBytesRead parameter.
	virtual bool Read(void* const pBuffer, const uint32 nBufferLength);

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

};

inline bool CLinux_LTFileRead::Open(const char* pszFilename)
{
	return OpenImpl(pszFilename, eOpenRead);
}

inline bool CLinux_LTFileRead::GetIsOpen(bool& bIsOpen)
{
	return GetIsOpenImpl(bIsOpen);
}

inline bool CLinux_LTFileRead::Read(void* const pBuffer, const uint32 nSize)
{
	LTASSERT(GetFileHandle() != NULL, "invalid file handle");

	// handle zero-byte read
	if (nSize == 0)
	{
		return true;
	}
	
	size_t nRead = ::fread(pBuffer, nSize, 1, GetFileHandle());
	
	if ((::ferror(GetFileHandle())) || (nRead != 1))
	{
		return false;
	}
	
	return true;
}

inline bool CLinux_LTFileRead::Seek(uint64 nPos)
{ 
	return SeekImpl(nPos); 
}

inline bool CLinux_LTFileRead::SeekToEnd()
{
	return SeekToEndImpl();
}

inline bool CLinux_LTFileRead::GetPos(uint64& nPos)
{
	return GetPosImpl(nPos);
}

inline bool CLinux_LTFileRead::GetIsEOF(bool& bIsEOF)
{
	return GetIsEOFImpl(bIsEOF);
}

inline bool CLinux_LTFileRead::GetFileSize(uint64& nSize)
{
	return GetFileSizeImpl(nSize);
}

inline bool CLinux_LTFileRead::Close()
{
	return CloseImpl();
}

#endif // __LINUX_LTFILEREAD_H__
