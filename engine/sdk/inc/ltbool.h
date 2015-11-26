//-------------------------------------------------------------------------------------
// LTBool.h
//
// This template class allows for the type safety of a boolean but allows for 
// designating the integer type that it will be stored into which allows for better
// optimizing the layout of structures for memory and performance. These are meant
// to be used exactly like normal boolean types other than their declaration.
//
//-------------------------------------------------------------------------------------
#ifndef __LTBOOL_H__
#define __LTBOOL_H__

template <typename Storage>
class LTBool
{
public:

	//default uninitialized constructor
	LTBool()								{}

	//allow construction from a boolean type
	LTBool(bool bVal)						{ m_nValue = static_cast<Storage>(bVal); }

	//allow type conversion back to a boolean
	operator bool () const					{ return !!m_nValue; }

	//assignment operation
	LTBool& operator=(bool bVal)			{ m_nValue = static_cast<Storage>(bVal); return *this; }

	//comparison operations
	bool operator==(bool bVal) const		{ return m_nValue == static_cast<Storage>(bVal); }
	bool operator!=(bool bVal) const		{ return m_nValue != static_cast<Storage>(bVal); }

	//the storage for our boolean
	Storage		m_nValue;
};

//common boolean types
typedef LTBool<uint8>		bool8;
typedef LTBool<uint16>		bool16;
typedef LTBool<uint32>		bool32;

#endif
