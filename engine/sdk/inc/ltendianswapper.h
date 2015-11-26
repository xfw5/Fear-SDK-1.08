//-----------------------------------------------------------------------------
// LTEndianSwapper.h
//
// Utility function that handles swapping of endian order of certain data types
//
//-----------------------------------------------------------------------------

#ifndef __LTENDIANSWAPPER_H__
#define __LTENDIANSWAPPER_H__

class LTEndianSwapper
{
public:

	//swapping for each individual type
	static int8		Swap(int8 nVal)			{ return InternalSwap(nVal); }
	static uint8	Swap(uint8 nVal)		{ return InternalSwap(nVal); }
	static int16	Swap(int16 nVal)		{ return InternalSwap(nVal); }
	static uint16	Swap(uint16 nVal)		{ return InternalSwap(nVal); }
	static int32	Swap(int32 nVal)		{ return InternalSwap(nVal); }
	static uint32	Swap(uint32 nVal)		{ return InternalSwap(nVal); }
	static int64	Swap(int64 nVal)		{ return InternalSwap(nVal); }
	static uint64	Swap(uint64 nVal)		{ return InternalSwap(nVal); }
	static float	Swap(float fVal)		{ return InternalSwap(fVal); }
	static double	Swap(double fVal)		{ return InternalSwap(fVal); }
	static bool		Swap(bool bVal)			{ return InternalSwap(bVal); }

private:

	//don't allow construction of this object
	LTEndianSwapper()		{}

	//an internal utility that handles the swapping operation
	template <class T>
	static T InternalSwap(const T& InVal)
	{
		//the output value
		T Swapped;

		//setup our buffers so we can perform the copy
		const uint8* pSrc = (const uint8*)&InVal;
		uint8* pDest = (uint8*)&Swapped;

		//now perform the swap
		uint32 nTypeSize = sizeof(T);

		for(uint32 nCurrByte = 0; nCurrByte < nTypeSize; nCurrByte++)
		{
			pDest[nCurrByte] = pSrc[nTypeSize - nCurrByte - 1];
		}

		return Swapped;
	}
};


#endif
