// *********************************************************************** //
//
// MODULE  : linux_ltfilewrite.h
//
// PURPOSE : Linux implementation for ILTFileWrite.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LINUX_LTFILEWRITE_H__
#define __LINUX_LTFILEWRITE_H__

#include "iltfilewrite.h"
#include "linux_ltfilebase.h"

class CLinux_LTFileWrite : private ILTFileWrite,
						   private CLinux_LTFileBase
{
public:

	CLinux_LTFileWrite() {}
	virtual ~CLinux_LTFileWrite() {}

	// open the file for writing.  This will close a previously opened file prior
	// to opening the new file.  If bAppend is false, this will truncate the
	// file, otherwise it will be opened in append mode.  The file is created
	// if it does not exist.
	virtual bool Open(const char* pszFilename, const bool bAppend);

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	virtual bool GetIsOpen(bool& bIsOpen);
	
	// write nSize bytes to the file from the buffer - the number of bytes
	// written is returned in the nBytesWritten parameter.  Returns false if an 
	// error occurs during the write operation.
	virtual bool Write(const void* const pBuffer, const uint32 nSize);

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

inline bool CLinux_LTFileWrite::Open(const char* pszFilename, const bool bAppend)
{
	if (!OpenImpl(pszFilename, bAppend ? eOpenWriteAppend : eOpenWrite))
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

	// disable buffering on this stream
	if (::setvbuf(GetFileHandle(), NULL, _IONBF, 0))
	{
		return false;
	}

	// success
	return true;
}

inline bool CLinux_LTFileWrite::GetIsOpen(bool& bIsOpen)
{
	return GetIsOpenImpl(bIsOpen);
}

inline bool CLinux_LTFileWrite::Write(const void* const pBuffer, const uint32 nSize)
{
	LTASSERT(GetFileHandle() != NULL, "invalid file handle");

	size_t nWritten = ::fwrite(pBuffer, nSize, 1, GetFileHandle());
	
	if ((::ferror(GetFileHandle())) || (nWritten != 1))
	{
		return false;
	}
	
	return true;
}

inline bool CLinux_LTFileWrite::Seek(uint64 nPos)
{ 
	return SeekImpl(nPos); 
}

inline bool CLinux_LTFileWrite::SeekToEnd()
{
	return SeekToEndImpl();
}

inline bool CLinux_LTFileWrite::GetPos(uint64& nPos)
{
	return GetPosImpl(nPos);
}

inline bool CLinux_LTFileWrite::GetIsEOF(bool& bIsEOF)
{
	return GetIsEOFImpl(bIsEOF);
}

inline bool CLinux_LTFileWrite::GetFileSize(uint64& nSize)
{
	return GetFileSizeImpl(nSize);
}

inline bool CLinux_LTFileWrite::Close()
{
	return CloseImpl();
}

#endif // __LINUX_LTFILEWRITE_H__
