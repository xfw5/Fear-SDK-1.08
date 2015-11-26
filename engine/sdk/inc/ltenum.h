//-------------------------------------------------------------------------------------
// LTEnum.h
//
// This template class allows for the type safety of an enumeration but allows for 
// designating the integer type that it will be stored into which allows for better
// optimizing the layout of structures for memory and performance. These are meant
// to be used exactly like normal enumeration types other than their declaration.
//
//-------------------------------------------------------------------------------------
#ifndef __LTENUM_H__
#define __LTENUM_H__

template <typename Storage, typename Enumeration>
class LTEnum
{
public:

	//default uninitialized constructor
	LTEnum()			{}

	//allow construction from an enumeration type
	LTEnum(Enumeration e)					{ m_nValue = static_cast<Storage>(e); }

	//allow type conversion back to an enumeration
	operator Enumeration () const			{ return static_cast<Enumeration>(m_nValue); }

	//assignment operation
	LTEnum& operator=(Enumeration e)		{ m_nValue = static_cast<Storage>(e); return *this; }

	//comparison operations
	bool operator==(Enumeration e) const	{ return m_nValue == static_cast<Storage>(e); }
	bool operator!=(Enumeration e) const	{ return m_nValue != static_cast<Storage>(e); }
	bool operator<=(Enumeration e) const	{ return m_nValue <= static_cast<Storage>(e); }
	bool operator>=(Enumeration e) const	{ return m_nValue >= static_cast<Storage>(e); }
	bool operator<(Enumeration e) const		{ return m_nValue < static_cast<Storage>(e); }
	bool operator>(Enumeration e) const		{ return m_nValue > static_cast<Storage>(e); }

	//the storage for our enumeration
	Storage		m_nValue;
};

#endif
