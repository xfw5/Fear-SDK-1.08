//--------------------------------------------------------------------------------------
// ILTOutConverter
//
// This is an output stream class that only allows for typed writing to prevent any
// ambiguous writes, thus allowing for implementations of this interface that can 
// perform conversions such as endian swapping. Due to this need for the typed 
// information, there is an override for each supported type, and there is no generic 
// write function
//--------------------------------------------------------------------------------------

#ifndef __ILTOUTCONVERTER_H__
#define __ILTOUTCONVERTER_H__

//types we support writing out
#ifndef __LTVECTOR_H__
#	include "ltvector.h"
#endif

#ifndef __LTTRANSFORM_H__
#	include "lttransform.h"
#endif

#ifndef __LTROTATION_H__
#	include "ltrotation.h"
#endif

#ifndef __LTPLANE_H__
#	include "ltplane.h"
#endif

class ILTOutConverter
{
public:

	//allow derived classes to destroy themselves
	virtual ~ILTOutConverter()		{}

	//called to destroy this object. This object must not be used after this call is made
	virtual void		Release() = 0;

	//this function can be called to determine if this stream supports seeking
	virtual bool		CanSeek() = 0;

	//called to seek to a particular byte from the beginning of the file. This will return LT_ERROR
	//if the offset is invalid, or if the stream cannot support seeking
	virtual LTRESULT	SeekTo(uint64 nOffset) = 0;

	//called to get the current position offset in the stream. This will always return zero if 
	//CanSeek() is not supported
	virtual uint64		GetPos() = 0;

	//called to write out a block of data that will not be converted. This is very dangerous to call
	//and should only be called in very specific cases (hence the different name from the ILTOutStream
	//to make it more detectable when converting from stream to a converter)
	virtual bool		WriteDataRaw(const void* pMemory, uint32 nMemSize) = 0;

	//support for writing out each of the supported types. For enumerations, they should
	//be type cast to the appropriate integer size
	virtual ILTOutConverter&	operator<<(int8 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(uint8 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(int16 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(uint16 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(int32 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(uint32 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(int64 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(uint64 nOut)	= 0;
	virtual ILTOutConverter&	operator<<(bool bOut)	= 0;
	virtual ILTOutConverter&	operator<<(float fOut)	= 0;
	virtual ILTOutConverter&	operator<<(double fOut)	= 0;

	//some more complex types, that can be overridden if they should not simply call into the
	//base type implementations. Note that these should match the ordering of binary layout of
	//the types in order to allow for reading this data in as blocks
	virtual ILTOutConverter&	operator<<(const LTVector2& vOut)
	{
		*this << vOut.x;
		*this << vOut.y;
		return *this;
	}

	virtual ILTOutConverter&	operator<<(const LTVector& vOut)
	{
		*this << vOut.x;
		*this << vOut.y;
		*this << vOut.z;
		return *this;
	}

	virtual ILTOutConverter&	operator<<(const LTVector4& vOut)
	{
		*this << vOut.x;
		*this << vOut.y;
		*this << vOut.z;
		*this << vOut.w;
		return *this;
	}

	virtual ILTOutConverter&	operator<<(const LTRotation& rOut)
	{
		for(uint32 nCurrComponent = 0; nCurrComponent < LTARRAYSIZE(rOut.m_Quat); nCurrComponent++)
			*this << rOut.m_Quat[nCurrComponent];
		return *this;
	}

	virtual ILTOutConverter&	operator<<(const LTRigidTransform& tOut)
	{
		*this << tOut.m_vPos;
		*this << tOut.m_rRot;
		return *this;
	}

	virtual ILTOutConverter&	operator<<(const LTTransform& tOut)
	{
		*this << tOut.m_vPos;
		*this << tOut.m_rRot;
		*this << tOut.m_fScale;
		return *this;
	}

	virtual ILTOutConverter&	operator<<(const LTPlane& Out)
	{
		*this << Out.m_Normal;
		*this << Out.m_Dist;
		return *this;
	}

	//writes out a string in a format that is compatible with ILTInStream::ReadString which is a
	//16 bit size followed by the string which is not null terminated
	virtual void WriteString(const char* pszString)
	{
		uint32 nStrLen = LTStrLen(pszString);
		LTASSERT(nStrLen <= 0xFFFF, "Error: Overflow in writing out a string");
		*this << (uint16)nStrLen;
		for(uint32 nCurrChar = 0; nCurrChar < nStrLen; nCurrChar++)
			*this << pszString[nCurrChar];
	}
};

#endif
