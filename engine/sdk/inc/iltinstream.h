//------------------------------------------------------------------------------
// ILTInStream.h
//
// Provides the definition for the ILTInStream interface. This interface should
// be implemented for all data streams that provide read functionality. Streams
// do not support simultanious read/write functionality.
//
// All streams that implement this must support sequential reading of data but
// some streams may also support random access of data. In order to determine
// if this is supported, the function CanSeek() should be called, and if this
// returns true, the seek functionality can be used, otherwise this will only
// return LT_ERROR
//
//------------------------------------------------------------------------------
#ifndef __ILTINSTREAM_H__
#define __ILTINSTREAM_H__

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

class ILTInStream 
{
public:

	//called when the stream is no longer needed. The stream must not be used beyond this call. Note
	//that streams are not reference counted so this will always terminate the stream.
	virtual void Release() = 0;

	//this is called to read in the specified number of bytes into the specified memory location. If
	//it successfully reads in that number of bytes it will return LT_OK. If it is unable to read
	//in the specified number of bytes it will return an error code, and fill in the specified buffer
	//with all zeros. 
	virtual LTRESULT Read(void *pData, uint32 size) = 0;

	//this will read in a string from a file that was written out using WriteString on an out stream.
	//This takes the pointer to the string memory, and the length of the available buffer in characters.
	//This will return LT_OK if the full string was able to be loaded, otherwise this will return an
	//error code. The format for strings in a file are:
	// uint16 - Number of characters in string without terminator
	// nLen   - The characters in the string not including null terminator
	virtual LTRESULT ReadString(char *pStr, uint32 maxBytes) = 0;

	//this function will determine if an error has occurred in this stream, either through writing,
	//seeking, or opening. This can be used instead of checking each individual occurrance of read
	virtual bool HasErrorOccurred() = 0;

	//this function can be called to determine if this stream supports seeking
	virtual bool CanSeek() = 0;

	//called to seek to a particular byte from the beginning of the file. This will return LT_ERROR
	//if the offset is invalid, or if the stream cannot support seeking
	virtual LTRESULT SeekTo(uint64 offset) = 0;

	//called to get the current position offset in the stream. This will always return zero if 
	//CanSeek() is not supported
	virtual uint64 GetPos() = 0;

	//called to get the total length of this file in bytes. 
	virtual uint64 GetLen() = 0;

	//provides the standard stream syntax for reading. This will delegate out the reading
	//functionality to an external function named LTStream_Read that takes the appropriate type.
	//This allows for extentions to the streaming without requiring the stream know about all types
	template<class T>
	ILTInStream &operator>>(T &toRead) 
	{
		//delegate out to an external function that can provide loading support
		LTStream_Read(this, toRead);
		return *this;
	}

protected:

	//must provide a virtual destructor in order to allow derived implementations to
	//override it
	virtual ~ILTInStream()	{}
};



//Provide reading functionality for all of the native types that we support

//utility macro to aid in the creation of the read functionality
#define IMPL_READ_FN(type)                                              \
	inline void LTStream_Read(ILTInStream *pStream, type &val) {          \
		pStream->Read(&val, sizeof(val));								\
	}                                                                  

IMPL_READ_FN(bool);
IMPL_READ_FN(int8);
IMPL_READ_FN(uint8);
IMPL_READ_FN(int16);
IMPL_READ_FN(uint16);
IMPL_READ_FN(int32);
IMPL_READ_FN(uint32);
IMPL_READ_FN(int64);
IMPL_READ_FN(uint64);
IMPL_READ_FN(float);
IMPL_READ_FN(double);

//remove this utility macro from usage to avoid conflicts
#undef IMPL_READ_FN

//DEPRECATED:
//
//a helper macro to read a piece of data. This assumes that the name of the stream class
//is pStream
#define STREAM_READ(_x_) pStream->Read(&(_x_), sizeof(_x_));

#endif
