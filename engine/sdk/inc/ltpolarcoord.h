#ifndef __LTPOLARCOORD_H__
#define __LTPOLARCOORD_H__

#ifndef __LTVECTOR_H__
	#include "ltvector.h"
#endif

#ifndef __ILTINSTREAM_H__
	#include "iltinstream.h"
#endif

#ifndef __ILTOUTSTREAM_H__
	#include "iltoutstream.h"
#endif
 
#ifndef __LTBASETYPES_H__
	#include "ltbasetypes.h"
#endif

#ifndef __LTASSERT_H__
	#include "ltassert.h"
#endif

#ifndef __LTMATH_H__
	#include "ltmath.h"
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS

//---------------------------------------------------------------------------//
struct LTPolarCoord 
{
    float x, y;
 
	LTPolarCoord()
	{}
	LTPolarCoord( const float x, const float y )
		:	x(x), y(y)
	{}

	LTPolarCoord( LTVector const& vUnitVector )
	{
		Init( vUnitVector );
	}

	//called to get an identity version of this primitive
	static LTPolarCoord GetIdentity()	{ return LTPolarCoord(0, 0); }

	//initializes all values to 0
	void Init()
	{
		x = 0.0f;
		y = 0.0f;
	}

	//initializes all values
	void Init( float a, float b )
	{
		x = a;
		y = b;
	}

	void Init( LTVector const& vUnitVector )
	{
		//the Y is simple, we can find the cosine of the Y axis by simply examining the Y, which can
		//then be converted to an angle in the range of [0..pi], which can be then be shifted
		y = MATH_HALFPI - acosf(LTCLAMP(vUnitVector.y, -1.0f, 1.0f));

		//the X is trickier. We must project into the XZ plane and find the angle. At that point it can
		//lie on either the right or left hand, so the angle must be setup accordingly

		//first handle the projection into the XZ plane (simply drop the Y component)
		LTVector vXZVec(vUnitVector.x, 0.0f, vUnitVector.z);

		//see if there is any value in this plane
		float fXZMag = vXZVec.Mag();
		if(fXZMag > 0.001f)
		{
			//there is enough of a magnitude to get a directional component, so normalize
			vXZVec /= fXZMag;

			//get the angle relative to the Z axis
			x = acosf(LTCLAMP(vXZVec.z, -1.0f, 1.0f));

			//see if it is in the negative hemisphere
			if(vXZVec.x < 0.0f)
				x = -x;
		}
		else
		{
			x = 0.0f;
		}
	}

	LTVector GetUnitVector( ) const
	{
		//convert the Y from being centered around Z to being centered around Y
		float fYAngle = MATH_HALFPI - y;

		return LTVector( sinf(x) * sinf(fYAngle),
			cosf(fYAngle),
			cosf(x) * sinf(fYAngle));
	}

	//linear interpolation between two polarcoords. 0 is the current value, 1 is the passed in value
	LTPolarCoord Lerp(const LTPolarCoord& b, float fT) const
	{
		return LTPolarCoord(x + (b.x - x) * fT, y + (b.y - y) * fT);
	}


	//--------------------------------------------------
	//Operators
	const float& operator [] ( const int32 i ) const
	{
		return *((&x) + i);
	}
	float& operator [] ( const int32 i )
	{
		return *((&x) + i);
	}
	bool operator == ( const LTPolarCoord& b ) const
	{
		return (b.x==x && b.y==y);
	}
	bool operator != ( const LTPolarCoord& b ) const
	{
		return !(b == *this);
	}
	const LTPolarCoord& operator = ( const LTPolarCoord& b )
	{
		x = b.x;
		y = b.y;

        return *this;
    }

	operator LTVector( ) const
	{
		return GetUnitVector();
	}
};

//NaN detection support for a vector
inline bool LTIsNaN(const LTPolarCoord& Val)	{ return LTIsNaN(Val.x) || LTIsNaN(Val.y); }

//Serialization functions
inline void LTStream_Read(ILTInStream *pStream, LTPolarCoord &vec)
{
    STREAM_READ(vec.x);
    STREAM_READ(vec.y);
}

inline void LTStream_Write(ILTOutStream *pStream, const LTPolarCoord &vec)
{
    STREAM_WRITE(vec.x);
    STREAM_WRITE(vec.y);
}

//handles expanding a vector into comma separated components
#define POLORCOORD_EXPAND(v)					(v).x, (v).y

#endif//doxygen
#endif
