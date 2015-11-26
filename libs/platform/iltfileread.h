// *********************************************************************** //
//
// MODULE  : iltfileread.h
//
// PURPOSE : Interface class for reading from a disk based file.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTFILEREAD_H__
#define __ILTFILEREAD_H__

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

class ILTFileRead
{
public:

	// open the file for reading.  This will close a previously opened file prior
	// to opening the new file.  Returns false if the file does not exist on disk.
	virtual bool Open(const char* pszFilename) = 0;

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	virtual bool GetIsOpen(bool& bIsOpen) = 0;

	// read nBytesToRead bytes from the file into the buffer - the buffer is assumed to be 
	// large enough to hold the data.  This returns false if the number of bytes
	// specified cannot be read, or if there is an error during the read operation.
	virtual bool Read(void* const pBuffer, const uint32 nBytesToRead) = 0;

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
