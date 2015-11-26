//----------------------------------------------------------------------------------------
// TypedBitField.h
//
// Provides a template class that can be used for a bit field that allows for flags within
// the bit field to be used in a type safe manner. This helps avoid issues of accidentally
// intermingling flags, and allows for type checking of parameters and values
//----------------------------------------------------------------------------------------
#ifndef __TYPEDBITFIELD_H__
#define __TYPEDBITFIELD_H__

template <typename TypeCheck, typename IntType>
class CTypedBitField
{
public:

	//construction
	CTypedBitField()												{}
	CTypedBitField(IntType nVal)									{ m_nValue = nVal; }
	CTypedBitField(const CTypedBitField<TypeCheck, IntType>& rhs)	{ m_nValue = rhs.m_nValue; }

	//assignment
	CTypedBitField<TypeCheck, IntType>& operator=(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		m_nValue = rhs.m_nValue;
		return *this;
	}

	//comparison
	bool operator==(const CTypedBitField<TypeCheck, IntType>& rhs) const
	{
		return m_nValue == rhs.m_nValue;
	}

	bool operator!=(const CTypedBitField<TypeCheck, IntType>& rhs) const
	{
		return m_nValue != rhs.m_nValue;
	}

	//or operator
	CTypedBitField<TypeCheck, IntType> operator|(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		return CTypedBitField<TypeCheck, IntType>(m_nValue | rhs.m_nValue);
	}

	CTypedBitField<TypeCheck, IntType>& operator|=(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		m_nValue |= rhs.m_nValue;
		return *this;
	}

	//and operator
	CTypedBitField<TypeCheck, IntType> operator&(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		return CTypedBitField<TypeCheck, IntType>(m_nValue & rhs.m_nValue);
	}

	CTypedBitField<TypeCheck, IntType>& operator&=(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		m_nValue &= rhs.m_nValue;
		return *this;
	}

	//xor operator
	CTypedBitField<TypeCheck, IntType> operator^(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		return CTypedBitField<TypeCheck, IntType>(m_nValue ^ rhs.m_nValue);
	}

	CTypedBitField<TypeCheck, IntType>& operator^=(const CTypedBitField<TypeCheck, IntType>& rhs)
	{
		m_nValue ^= rhs.m_nValue;
		return *this;
	}

	//not operator
	CTypedBitField<TypeCheck, IntType> operator~()
	{
		return CTypedBitField<TypeCheck, IntType>(~m_nValue);
	}

	//type conversion operators
	operator IntType()
	{
		return m_nValue;
	}

	//bit manipulation utilities
	
	//given a bit field, this will determine if any of those bits are set on this bit field
	bool IsAnySet(IntType nBits) const
	{
		return (m_nValue & nBits) != 0;
	}

	//given a bit field, this will determine if all of those bits are set on this bit field
	bool IsAllSet(IntType nBits) const
	{
		return (m_nValue & nBits) == nBits;
	}

	//given a bit field, this will handle clearing out all of the provided bits
	void Clear(IntType nBits)
	{
		m_nValue &= ~nBits;
	}

	//given a bit field, this will handle toggling all of the bits provided
	void Toggle(IntType nBits)
	{
		//the first part clears out all of the bits specified, the second adds in the on
		//bits from the toggled value
		m_nValue = (m_nValue & ~nBits) | (~m_nValue & nBits);
	}

private:

	IntType		m_nValue;
};


#endif
