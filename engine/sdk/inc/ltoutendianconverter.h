//--------------------------------------------------------------------------------------
// LTOutEndianConverter
//
// This is an output stream class that implements the ILTOutConverter interface and
// performs endian swapping on all of the data before writing it out to the associated
// output stream.
//--------------------------------------------------------------------------------------

#ifndef __LTOUTENDIANCONVERTER_H__
#define __LTOUTENDIANCONVERTER_H__

//our base class
#ifndef __ILTOUTCONVERTER_H__
#	include "iltoutconverter.h"
#endif

//the class that implements the actual writing
#ifndef __ILTOUTSTREAM_H__
#	include "iltoutstream.h"
#endif

//interface that handles swapping the endian of our data
#ifndef __LTENDIANSWAPPER_H__
#	include "ltendianswapper.h"
#endif

class LTOutEndianConverter :
	public ILTOutConverter
{
public:

	//this object wraps an output stream of which must be valid. It takes full ownership of this stream
	//and releases it once this object is destroyed
	LTOutEndianConverter(ILTOutStream& OutStream)		{ m_pStream = &OutStream; }
	virtual ~LTOutEndianConverter()						{ m_pStream->Release(); }

	//called to destroy this object. This object must not be used after this call is made
	virtual void		Release()						{ delete this; }

	//this function can be called to determine if this stream supports seeking
	virtual bool		CanSeek()						{ return m_pStream->CanSeek(); }

	//called to seek to a particular byte from the beginning of the file. This will return LT_ERROR
	//if the offset is invalid, or if the stream cannot support seeking
	virtual LTRESULT	SeekTo(uint64 nOffset)			{ return m_pStream->SeekTo(nOffset); }

	//called to get the current position offset in the stream. This will always return zero if 
	//CanSeek() is not supported
	virtual uint64		GetPos()						{ return m_pStream->GetPos(); }

	//called to write out a block of data that will not be converted. This is very dangerous to call
	//and should only be called in very specific cases (hence the different name from the ILTOutStream
	//to make it more detectable when converting from stream to a converter)
	virtual bool		WriteDataRaw(const void* pMemory, uint32 nMemSize)
	{
		return m_pStream->Write(pMemory, nMemSize) == LT_OK;
	}

	//support for writing out each of the supported types. For enumerations, they should
	//be type cast to the appropriate integer size
	virtual ILTOutConverter&	operator<<(int8 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(uint8 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(int16 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(uint16 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(int32 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(uint32 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(int64 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(uint64 nOut)	{ *m_pStream << LTEndianSwapper::Swap(nOut); return *this; }
	virtual ILTOutConverter&	operator<<(bool bOut)	{ *m_pStream << LTEndianSwapper::Swap(bOut); return *this; }
	virtual ILTOutConverter&	operator<<(float fOut)	{ *m_pStream << LTEndianSwapper::Swap(fOut); return *this; }
	virtual ILTOutConverter&	operator<<(double fOut)	{ *m_pStream << LTEndianSwapper::Swap(fOut); return *this; }

private:

	//the stream that handles the actual output of the data
	ILTOutStream*		m_pStream;
};

#endif
