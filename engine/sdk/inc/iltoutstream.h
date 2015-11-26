//------------------------------------------------------------------------------
// ILTOutStream.h
//
// Provides the definition for the ILTOutStream interface. This interface should
// be implemented for all data streams that provide write functionality. Streams
// do not support simultanious read/write functionality.
//
// All streams that implement this must support sequential writing of data but
// some streams may also support random access of data. In order to determine
// if this is supported, the function CanSeek() should be called, and if this
// returns true, the seek functionality can be used, otherwise this will only
// return LT_ERROR
//
//------------------------------------------------------------------------------
#ifndef __ILTOUTSTREAM_H__
#define __ILTOUTSTREAM_H__

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

class ILTOutStream 
{
public:

	//called when the stream is no longer needed. The stream must not be used beyond this call. Note
	//that streams are not reference counted so this will always terminate the stream.
	virtual void Release() = 0;

	//this is called to write the specified number of bytes to the stream. If it successfully writes
	//the specified number of bytes, LT_OK will be returned, otherwise LT_ERROR will be returned.
	virtual LTRESULT Write(const void *pData, uint32 size) = 0;

	//this will write out the specified string to the output so that it can be loaded in later
	//through ReadString. This will return LT_OK if the full string was able to be loaded, 
	//otherwise this will return an error code. The format for strings in a file are:
	// uint16 - Number of characters in string without terminator
	// nLen   - The characters in the string not including null terminator
	virtual LTRESULT WriteString(const char *pStr) = 0;

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

	//provides the standard stream syntax for reading. This will delegate out the reading
	//functionality to an external function named LTStream_Read that takes the appropriate type.
	//This allows for extentions to the streaming without requiring the stream know about all types
	template<class T>
	ILTOutStream &operator<<(const T &toWrite) 
	{
		//delegate out to an external function that can provide loading support
		LTStream_Write(this, toWrite);
		return *this;
	}

protected:

	//must provide a virtual destructor in order to allow derived implementations to
	//override it
	virtual ~ILTOutStream() {}
};



//Provide reading functionality for all of the native types that we support

//utility macro to aid in the creation of the read functionality
#define IMPL_WRITE_FN(type)												\
	inline void LTStream_Write(ILTOutStream *pStream, const type &val) {	\
		pStream->Write(&val, sizeof(val));								\
	}                                                                  

IMPL_WRITE_FN(bool);
IMPL_WRITE_FN(int8);
IMPL_WRITE_FN(uint8);
IMPL_WRITE_FN(int16);
IMPL_WRITE_FN(uint16);
IMPL_WRITE_FN(int32);
IMPL_WRITE_FN(uint32);
IMPL_WRITE_FN(int64);
IMPL_WRITE_FN(uint64);
IMPL_WRITE_FN(float);
IMPL_WRITE_FN(double);

//remove this utility macro from usage to avoid conflicts
#undef IMPL_WRITE_FN

//DEPRECATED:
//
//a helper macro to write a piece of data. This assumes that the name of the stream class
//is pStream
#define STREAM_WRITE(_x_) pStream->Write(&(_x_), sizeof(_x_));

#endif
