// *********************************************************************** //
//
// MODULE  : iltfilewrite.h
//
// PURPOSE : Interface class for writing to a disk based file.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTFILEWRITE_H__
#define __ILTFILEWRITE_H__

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

class ILTFileWrite
{
public:

	// open the file for writing.  This will close a previously opened file prior
	// to opening the new file.  If bAppend is false, this will truncate the
	// file, otherwise it will be opened in append mode.  The file is created
	// if it does not exist.
	virtual bool Open(const char* pszFilename, const bool bAppend) = 0;

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	virtual bool GetIsOpen(bool& bIsOpen) = 0;

	// write nBytesToWrite bytes to the file from the buffer - the number of bytes
	// written is returned in the nBytesWritten parameter.  Returns false if the
	// number of bytes specified cannot be written, or if an error occurs during 
	// the write operation.
	virtual bool Write(const void* const pBuffer, const uint32 nBytesToWrite) = 0;

	// seek to the specified position in the file relative to the start of the file.
	// Returns false if an error occurs during the seek.
	virtual bool Seek(uint64 nPos) = 0;

	// seek to the current end of file.  Returns false if an error occurs during the
	// seek operation.
	virtual bool SeekToEnd() = 0;

	// return the current position based on the start of the file.  Returns false if 
	// an error occurs while retrieving the position.
	virtual bool GetPos(uint64& nPos) = 0;

	// test to see if the file is at EOF.  Returns false if an error occurs while
	// testing for EOF.
	virtual bool GetIsEOF(bool& bIsEOF) = 0;

	// return the size of the currently opened file.  Returns false if an error occurs
	// while determining the file size.
	virtual bool GetFileSize(uint64& nSize) = 0;

	// close the file.  Returns false if an error occurs while closing the file.
	virtual bool Close() = 0;

};

#endif // __ILTFILEREAD_H__
