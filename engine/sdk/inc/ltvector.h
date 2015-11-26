#ifndef __LTVECTOR_H__
#define __LTVECTOR_H__

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
template<class T>
struct TVector2 {
    T x, y;

	typedef typename TFloatType<T>::Type float_T;
 
	TVector2()
	{}
	TVector2( const T x, const T y )
		:	x(x), y(y)
	{}

	//called to get an identity version of this primitive
	static TVector2	GetIdentity()	{ return TVector2<T>(0, 0); }

	//initializes all values to 0
	void Init()
	{
		x = 0;
		y = 0;
	}

	//initializes all values
	void Init( T a, T b )
	{
		x = a;
		y = b;
	}

	//dot product operator
	T Dot( const TVector2& b ) const
	{
		return x*b.x + y*b.y;
	}

	//magnitude of this vector squared
	T MagSqr() const
	{
		return x*x + y*y;
	}

	//magnitude of this vector
	T Mag() const
	{
		return (T)LTSqrt(MagSqr());
	}

	//returns normalized version of this vector
	const TVector2 GetUnit() const
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		return (*this) / Mag;
	}

	//normalize to unit length
	void Normalize()
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		(*this) /= Mag;
    }

	//comparison within a passed in threshold, done as distance, not per component
    bool NearlyEquals(const TVector2& b, const T r = 0) const
    {
        //within a tolerance
        const TVector2 t = *this - b;//difference
        return t.Dot(t) <= r*r;//radius
 
	}

	//distance to a point
    T Dist(const TVector2& b) const
    {
        return (*this - b).Mag();
    }

	//distance to a point squared
    T DistSqr(const TVector2& b) const
    {
        return (*this - b).MagSqr();
    }

	//linear interpolation between two vectors. 0 is the current vector, 1 is the passed in vector
	TVector2 Lerp(const TVector2& b, float_T fT) const
	{
		return TVector2(x + (b.x - x) * fT, y + (b.y - y) * fT);
	}

	//each component will be set to the lower of the current value or the passed in value
	void Min(T f)
	{
		Init(LTMIN(x, f), LTMIN(y, f));
	}

	//each component will be set to the lower of the current value or the passed in value
	void Min(const TVector2& v)
	{
		Init(LTMIN(x, v.x), LTMIN(y, v.y));
	}

	//each component will be set to the larger of the current value or the passed in value
	void Max(T f)
	{
		Init(LTMAX(x, f), LTMAX(y, f));
	}

	//each component will be set to the larger of the current value or the passed in value
	void Max(const TVector2& v)
	{
		Init(LTMAX(x, v.x), LTMAX(y, v.y));
	}

	//each component will be set to the lower of the current value or the passed in value
	TVector2 GetMin(T f) const
	{
		return TVector2(LTMIN(x, f), LTMIN(y, f));
	}

	//each component will be set to the lower of the current value or the passed in value
	TVector2 GetMin(const TVector2& v) const
	{
		return TVector2(LTMIN(x, v.x), LTMIN(y, v.y));
	}

	//each component will be set to the larger of the current value or the passed in value
	TVector2 GetMax(T f) const
	{
		return TVector2(LTMAX(x, f), LTMAX(y, f));
	}

	//each component will be set to the larger of the current value or the passed in value
	TVector2 GetMax(const TVector2& v) const
	{
		return TVector2(LTMAX(x, v.x), LTMAX(y, v.y));
	}

	//clamp each component in the vector to the specified values. Assumes that Min <= Max
	void Clamp(T Min, T Max)
	{
		Init(LTCLAMP(x, Min, Max), LTCLAMP(y, Min, Max));
	}

	//clamp each component in the vector to the specified vector min and max. Assumes that Min <= Max
	void Clamp(const TVector2& vMin, const TVector2& vMax)
	{
		Init(LTCLAMP(x, vMin.x, vMax.x), LTCLAMP(y, vMin.y, vMax.y));
	}

	//returns the clamped version of this vector
	TVector2 GetClamped(T Min, T Max) const
	{
		return TVector2(LTCLAMP(x, Min, Max), LTCLAMP(y, Min, Max));
	}

	//returns the clamped version of this vector
	TVector2 GetClamped(const TVector2& vMin, const TVector2& vMax) const
	{
		return TVector2(LTCLAMP(x, vMin.x, vMax.x), LTCLAMP(y, vMin.y, vMax.y));
	}

	//normalize the vector to the specified length
    void SetMagnitude(T nVal)
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		(*this) *= nVal / Mag;
	}

	//--------------------------------------------------
	//Operators
	const T& operator [] ( const int32 i ) const
	{
		return *((&x) + i);
	}
	T& operator [] ( const int32 i )
	{
		return *((&x) + i);
	}
	bool operator == ( const TVector2& b ) const
	{
		return (b.x==x && b.y==y);
	}
	bool operator != ( const TVector2& b ) const
	{
		return !(b == *this);
	}
	const TVector2 operator - () const
	{
		return TVector2( -x, -y );
	}
	const TVector2& operator = ( const TVector2& b )
	{
		x = b.x;
		y = b.y;

        return *this;
    }
    const TVector2& operator += (const T s) 
    {
        x += s;
        y += s;

        return *this;
    } 
    const TVector2& operator += (const TVector2& b) 
    {
        x += b.x;
        y += b.y;

        return *this;
    } 
    const TVector2& operator -= (const T s) 
    {
        x -= s;
        y -= s;

        return *this;
    } 
    const TVector2& operator -= (const TVector2& b) 
    {
        x -= b.x;
        y -= b.y;

        return *this;
    } 
    const TVector2& operator *= (const T s)
    {
        x *= s;
        y *= s;

        return *this;
    }
    const TVector2& operator *= (const TVector2& b)
    {
        x *= b.x;
        y *= b.y;

        return *this;
    }
    const TVector2& operator /= (const T s)
    {
        const T r = 1 / s;//reciprocal

        x *= r;
        y *= r;

        return *this;
    }
    const TVector2& operator /= (const TVector2& b)
    {
        x /= b.x;
        y /= b.y;

        return *this;
    }
    const TVector2 operator + (const T s) const
    {
        return TVector2(x + s, y + s);
    }
    const TVector2 operator + (const TVector2& b) const
    {
        return TVector2(x + b.x, y + b.y);
    }
    friend inline const TVector2 operator + (const T s, const TVector2& v)
    {
        return v + s;
    }
    const TVector2 operator - (const T s) const
    {
        return TVector2(x - s, y - s);
    }
    const TVector2 operator - (const TVector2& b) const
    {
        return TVector2(x - b.x, y - b.y);
    }
    friend inline const TVector2 operator - (const T s, const TVector2& v)
    {
        return -v + s;
    }
    const TVector2 operator * (const T s) const
    {
        return TVector2(x*s, y*s);
    }
    const TVector2 operator * (const TVector2& b) const
    {
        return TVector2(x*b.x, y*b.y);
    }
    friend inline const TVector2 operator * (const T s, const TVector2& v)
    {
        return v * s;
    }
    const TVector2 operator / (const T s) const
    {
        const T r = 1 / s;//reciprocal

        return TVector2(x*r, y*r);
    }
    const TVector2 operator / (const TVector2& b) const
    {
        return TVector2(x/b.x, y/b.y);
    }
    friend inline const TVector2 operator / (const T s, const TVector2& v)
    {
        return TVector2(s/v.x, s/v.y);
    }
    bool operator > (const TVector2 &other) const { return ((x>other.x) && (y>other.y)); }
    bool operator < (const TVector2 &other) const { return ((x<other.x) && (y<other.y)); }
    bool operator >= (const TVector2 &other) const { return ((x>=other.x) && (y>=other.y)); }
    bool operator <= (const TVector2 &other) const { return ((x<=other.x) && (y<=other.y)); }
};

//The most common use of this template, as a floating point vector
typedef TVector2<float> LTVector2;

//useful for interface functions
typedef TVector2<int32> LTVector2n;

//NaN detection support for a vector
template <typename T>
inline bool LTIsNaN(const TVector2<T>& Val)	{ return LTIsNaN(Val.x) || LTIsNaN(Val.y); }

//Serialization functions
template<class T>
inline void LTStream_Read(ILTInStream *pStream, TVector2<T> &vec)
{
    STREAM_READ(vec.x);
    STREAM_READ(vec.y);
}

template<class T>
inline void LTStream_Write(ILTOutStream *pStream, const TVector2<T> &vec)
{
    STREAM_WRITE(vec.x);
    STREAM_WRITE(vec.y);
}

//handles expanding a vector into comma separated components
#define VEC2_EXPAND(v)					(v).x, (v).y

//---------------------------------------------------------------------------//
template<class T>
struct TVector3 {
    T x, y, z;

	typedef typename TFloatType<T>::Type float_T;
 
	TVector3()
	{}
	TVector3( const T x, const T y, const T z )
		:	x(x), y(y), z(z)
	{}
	TVector3( const TVector2<T> &v, const T z )
		:	x(v.x), y(v.y), z(z)
	{}

	//called to get an identity version of this primitive
	static TVector3	GetIdentity()	{ return TVector3<T>(0, 0, 0); }

	//initializes all values to 0
	void Init()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	//initializes all values
	void Init( T a, T b, T c)
	{
		x = a;
		y = b;
		z = c;
	}

	//dot product operator
	T Dot( const TVector3& b ) const
	{
		return x*b.x + y*b.y + z*b.z;
	}

	//magnitude of this vector squared
	T MagSqr() const
	{
		return x*x + y*y + z*z;
	}

	//magnitude of this vector
	T Mag() const
	{
		return (T)LTSqrt(MagSqr());
	}

	//returns normalized version of this vector
	const TVector3 GetUnit() const
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		return (*this) / Mag;
	}

	//normalize to unit length
	void Normalize()
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		(*this) /= Mag;
    }

	//comparison within a passed in threshold, done as distance, not per component
    bool NearlyEquals(const TVector3& b, const T r = 0) const
    {
        //within a tolerance
        const TVector3 t = *this - b;//difference
        return t.Dot(t) <= r*r;//radius
 
	}

	//cross product operator
 	const TVector3 Cross( const TVector3& b ) const
	{
		return TVector3( b.y*z - b.z*y, b.z*x - b.x*z, b.x*y - b.y*x );
	}

	//distance to a point
    T Dist(const TVector3& b) const
    {
        return (*this - b).Mag();
    }

	//distance to a point squared
    T DistSqr(const TVector3& b) const
    {
        return (*this - b).MagSqr();
    }

	//linear interpolation between two vectors. 0 is the current vector, 1 is the passed in vector
	TVector3 Lerp(const TVector3& b, float_T fT) const
	{
		return TVector3(x + (b.x - x) * fT, y + (b.y - y) * fT, z + (b.z - z) * fT);
	}

	//each component will be set to the lower of the current value or the passed in value
	void Min(T f)
	{
		Init(LTMIN(x, f), LTMIN(y, f), LTMIN(z, f));
	}

	//each component will be set to the lower of the current value or the passed in value
	void Min(const TVector3& v)
	{
		Init(LTMIN(x, v.x), LTMIN(y, v.y), LTMIN(z, v.z));
	}

	//each component will be set to the larger of the current value or the passed in value
	void Max(T f)
	{
		Init(LTMAX(x, f), LTMAX(y, f), LTMAX(z, f));
	}

	//each component will be set to the larger of the current value or the passed in value
	void Max(const TVector3& v)
	{
		Init(LTMAX(x, v.x), LTMAX(y, v.y), LTMAX(z, v.z));
	}

	//each component will be set to the lower of the current value or the passed in value
	TVector3 GetMin(T f) const
	{
		return TVector3(LTMIN(x, f), LTMIN(y, f), LTMIN(z, f));
	}

	//each component will be set to the lower of the current value or the passed in value
	TVector3 GetMin(const TVector3& v) const
	{
		return TVector3(LTMIN(x, v.x), LTMIN(y, v.y), LTMIN(z, v.z));
	}

	//each component will be set to the larger of the current value or the passed in value
	TVector3 GetMax(T f) const
	{
		return TVector3(LTMAX(x, f), LTMAX(y, f), LTMAX(z, f));
	}

	//each component will be set to the larger of the current value or the passed in value
	TVector3 GetMax(const TVector3& v) const
	{
		return TVector3(LTMAX(x, v.x), LTMAX(y, v.y), LTMAX(z, v.z));
	}

	//clamp each component in the vector to the specified values. Assumes that Min <= Max
	void Clamp(T Min, T Max)
	{
		Init(LTCLAMP(x, Min, Max), LTCLAMP(y, Min, Max), LTCLAMP(z, Min, Max));
	}

	//clamp each component in the vector to the specified vector min and max. Assumes that Min <= Max
	void Clamp(const TVector3& vMin, const TVector3& vMax)
	{
		Init(LTCLAMP(x, vMin.x, vMax.x), LTCLAMP(y, vMin.y, vMax.y), LTCLAMP(z, vMin.z, vMax.z));
	}

	//returns the clamped version of this vector
	TVector3 GetClamped(T Min, T Max) const
	{
		return TVector3(LTCLAMP(x, Min, Max), LTCLAMP(y, Min, Max), LTCLAMP(z, Min, Max));
	}

	//returns the clamped version of this vector
	TVector3 GetClamped(const TVector3& vMin, const TVector3& vMax) const
	{
		return TVector3(LTCLAMP(x, vMin.x, vMax.x), LTCLAMP(y, vMin.y, vMax.y), LTCLAMP(z, vMin.z, vMax.z));
	}

	//normalize the vector to the specified length
    void SetMagnitude(T nVal)
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		(*this) *= nVal / Mag;
	}

	//given this vector it will generate an orthonormal, which is a vector that is perpindicular
	//to this vector and is of unit length. This can be used to build up a full basis space. This
	//will not work on zero length vectors and works best on unit vectors.
	TVector3 BuildOrthonormal() const
	{
		//the algorithm here is to find the minimum component, set that to 0, swap the remaining
		//two values and negate it. This essentially does a projection onto the smallest axis and
		//then performs a 2d cross product, making this approach very stable numerically
		TVector3 rv;
		TVector3 vAbs((x < 0) ? -x : x, (y < 0) ? -y : y, (z < 0) ? -z : z);

		if((vAbs.x <= vAbs.y) && (vAbs.x <= vAbs.z))
			rv.Init(0, -z, y);
		else if((vAbs.y <= vAbs.x) && (vAbs.y <= vAbs.z))
			rv.Init(-z, 0, x);
		else
			rv.Init(-y, x, 0);

		//and now normalize
		rv.Normalize();
		return rv;
	}
		
	//--------------------------------------------------
	//Operators
	const T& operator [] ( const int32 i ) const
	{
		return *((&x) + i);
	}
	T& operator [] ( const int32 i )
	{
		return *((&x) + i);
	}
	bool operator == ( const TVector3& b ) const
	{
		return (b.x==x && b.y==y && b.z==z);
	}
	bool operator != ( const TVector3& b ) const
	{
		return !(b == *this);
	}
	const TVector3 operator - () const
	{
		return TVector3( -x, -y, -z );
	}
	const TVector3& operator = ( const TVector3& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;

        return *this;
    }
    const TVector3& operator += (const T s) 
    {
        x += s;
        y += s;
        z += s;

        return *this;
    } 
    const TVector3& operator += (const TVector3& b) 
    {
        x += b.x;
        y += b.y;
        z += b.z;

        return *this;
    } 
    const TVector3& operator -= (const T s) 
    {
        x -= s;
        y -= s;
        z -= s;

        return *this;
    } 
    const TVector3& operator -= (const TVector3& b) 
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;

        return *this;
    } 
    const TVector3& operator *= (const T s)
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }
    const TVector3& operator *= (const TVector3& b)
    {
        x *= b.x;
        y *= b.y;
        z *= b.z;

        return *this;
    }
    const TVector3& operator /= (const T s)
    {
        const T r = 1 / s;//reciprocal

        x *= r;
        y *= r;
        z *= r;

        return *this;
    }
    const TVector3& operator /= (const TVector3& b)
    {
        x /= b.x;
        y /= b.y;
        z /= b.z;

        return *this;
    }
    const TVector3 operator + (const T s) const
    {
        return TVector3(x + s, y + s, z + s);
    }
    const TVector3 operator + (const TVector3& b) const
    {
        return TVector3(x + b.x, y + b.y, z + b.z);
    }
    friend inline const TVector3 operator + (const T s, const TVector3& v)
    {
        return v + s;
    }
    const TVector3 operator - (const T s) const
    {
        return TVector3(x - s, y - s, z - s);
    }
    const TVector3 operator - (const TVector3& b) const
    {
        return TVector3(x - b.x, y - b.y, z - b.z);
    }
    friend inline const TVector3 operator - (const T s, const TVector3& v)
    {
        return -v + s;
    }
    const TVector3 operator * (const T s) const
    {
        return TVector3(x*s, y*s, z*s);
    }
    const TVector3 operator * (const TVector3& b) const
    {
        return TVector3(x*b.x, y*b.y, z*b.z);
    }
    friend inline const TVector3 operator * (const T s, const TVector3& v)
    {
        return v * s;
    }
    const TVector3 operator / (const T s) const
    {
        const T r = 1 / s;//reciprocal

        return TVector3(x*r, y*r, z*r);
    }
    const TVector3 operator / (const TVector3& b) const
    {
        return TVector3(x/b.x, y/b.y, z/b.z);
    }
    friend inline const TVector3 operator / (const T s, const TVector3& v)
    {
        return TVector3(s/v.x, s/v.y, s/v.z);
    }
    bool operator > (const TVector3 &other) const { return ((x>other.x) && (y>other.y) && (z>other.z)); }
    bool operator < (const TVector3 &other) const { return ((x<other.x) && (y<other.y) && (z<other.z)); }
    bool operator >= (const TVector3 &other) const { return ((x>=other.x) && (y>=other.y) && (z>=other.z)); }
    bool operator <= (const TVector3 &other) const { return ((x<=other.x) && (y<=other.y) && (z<=other.z)); }
};

//The most common use of this template, as a floating point vector
typedef TVector3<float> LTVector;

//NaN detection support for a vector
template <typename T>
inline bool LTIsNaN(const TVector3<T>& Val)	{ return LTIsNaN(Val.x) || LTIsNaN(Val.y) || LTIsNaN(Val.z); }

//Serialization functions
template<class T>
inline void LTStream_Read(ILTInStream *pStream, TVector3<T> &vec)
{
    STREAM_READ(vec.x);
    STREAM_READ(vec.y);
    STREAM_READ(vec.z);
}

template<class T>
inline void LTStream_Write(ILTOutStream *pStream, const TVector3<T> &vec)
{
    STREAM_WRITE(vec.x);
    STREAM_WRITE(vec.y);
    STREAM_WRITE(vec.z);
}

//handles expanding a vector into comma separated components
#define VEC_EXPAND(v)					(v).x, (v).y, (v).z

//---------------------------------------------------------------------------//
template<class T>
struct TVector4 {
    T x, y, z, w;

	typedef typename TFloatType<T>::Type float_T;
 
	TVector4()
	{}
	TVector4( const T x, const T y, const T z, const T w )
		:	x(x), y(y), z(z), w(w)
	{}
	TVector4( const TVector3<T> &v, const T w )
		:	x(v.x), y(v.y), z(v.z), w(w)
	{}

	//called to get an identity version of this primitive
	static TVector4	GetIdentity()	{ return TVector4<T>(0, 0, 0, 0); }

	//initializes all values to 0
	void Init()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	//initializes all values
	void Init( T a, T b, T c, T d)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}

	//dot product operator
	T Dot( const TVector4& b ) const
	{
		return x*b.x + y*b.y + z*b.z + w*b.w;
	}

	//magnitude of this vector squared
	T MagSqr() const
	{
		return x*x + y*y + z*z + w*w;
	}

	//magnitude of this vector
	T Mag() const
	{
		return (T)LTSqrt(MagSqr());
	}

	//returns normalized version of this vector
	const TVector4 GetUnit() const
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		return (*this) / Mag;
	}

	//normalize to unit length
	void Normalize()
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		(*this) /= Mag;
    }

	//comparison within a passed in threshold, done as distance, not per component
    bool NearlyEquals(const TVector4& b, const T r = 0) const
    {
        //within a tolerance
        const TVector4 t = *this - b;//difference
        return t.Dot(t) <= r*r;//radius
 
	}

	//distance to a point
    T Dist(const TVector4& b) const
    {
        return (*this - b).Mag();
    }

	//distance to a point squared
    T DistSqr(const TVector4& b) const
    {
        return (*this - b).MagSqr();
    }

	//linear interpolation between two vectors. 0 is the current vector, 1 is the passed in vector
	TVector4 Lerp(const TVector4& b, float_T fT) const
	{
		return TVector4(x + (b.x - x) * fT, y + (b.y - y) * fT, z + (b.z - z) * fT, w + (b.w - w) * fT);
	}

	//each component will be set to the lower of the current value or the passed in value
	void Min(T f)
	{
		Init(LTMIN(x, f), LTMIN(y, f), LTMIN(z, f), LTMIN(w, f));
	}

	//each component will be set to the lower of the current value or the passed in value
	void Min(const TVector4& v)
	{
		Init(LTMIN(x, v.x), LTMIN(y, v.y), LTMIN(z, v.z), LTMIN(w, v.w));
	}

	//each component will be set to the larger of the current value or the passed in value
	void Max(T f)
	{
		Init(LTMAX(x, f), LTMAX(y, f), LTMAX(z, f), LTMAX(w, f));
	}

	//each component will be set to the larger of the current value or the passed in value
	void Max(const TVector4& v)
	{
		Init(LTMAX(x, v.x), LTMAX(y, v.y), LTMAX(z, v.z), LTMAX(w, v.w));
	}

	//each component will be set to the lower of the current value or the passed in value
	TVector4 GetMin(T f) const
	{
		return TVector4(LTMIN(x, f), LTMIN(y, f), LTMIN(z, f), LTMIN(w, f));
	}

	//each component will be set to the lower of the current value or the passed in value
	TVector4 GetMin(const TVector4& v) const
	{
		return TVector4(LTMIN(x, v.x), LTMIN(y, v.y), LTMIN(z, v.z), LTMIN(w, v.w));
	}

	//each component will be set to the larger of the current value or the passed in value
	TVector4 GetMax(T f) const
	{
		return TVector4(LTMAX(x, f), LTMAX(y, f), LTMAX(z, f), LTMAX(w, f));
	}

	//each component will be set to the larger of the current value or the passed in value
	TVector4 GetMax(const TVector4& v) const
	{
		return TVector4(LTMAX(x, v.x), LTMAX(y, v.y), LTMAX(z, v.z), LTMAX(w, v.w));
	}

	//clamp each component in the vector to the specified values. Assumes that Min <= Max
	void Clamp(T Min, T Max)
	{
		Init(LTCLAMP(x, Min, Max), LTCLAMP(y, Min, Max), LTCLAMP(z, Min, Max), LTCLAMP(w, Min, Max));
	}

	//clamp each component in the vector to the specified vector min and max. Assumes that Min <= Max
	void Clamp(const TVector4& vMin, const TVector4& vMax)
	{
		Init(LTCLAMP(x, vMin.x, vMax.x), LTCLAMP(y, vMin.y, vMax.y), LTCLAMP(z, vMin.z, vMax.z), LTCLAMP(w, vMin.w, vMax.w));
	}

	//returns the clamped version of this vector
	TVector4 GetClamped(T Min, T Max) const
	{
		return TVector4(LTCLAMP(x, Min, Max), LTCLAMP(y, Min, Max), LTCLAMP(z, Min, Max), LTCLAMP(w, Min, Max));
	}

	//returns the clamped version of this vector
	TVector4 GetClamped(const TVector4& vMin, const TVector4& vMax) const
	{
		return TVector4(LTCLAMP(x, vMin.x, vMax.x), LTCLAMP(y, vMin.y, vMax.y), LTCLAMP(z, vMin.z, vMax.z), LTCLAMP(w, vMin.w, vMax.w));
	}

	//normalize the vector to the specified length
    void SetMagnitude(T nVal)
	{
		T Mag = this->Mag();
		LTASSERT(Mag > 0, "Warning: Attempting to normalize a vector with very small magnitude");
		(*this) *= nVal / Mag;
	}

	//--------------------------------------------------
	//Operators
	const T& operator [] ( const int32 i ) const
	{
		return *((&x) + i);
	}
	T& operator [] ( const int32 i )
	{
		return *((&x) + i);
	}
	bool operator == ( const TVector4& b ) const
	{
		return (b.x==x && b.y==y && b.z==z && b.w==w);
	}
	bool operator != ( const TVector4<T>& b ) const
	{
		return !(b == *this);
	}
	const TVector4 operator - () const
	{
		return TVector4( -x, -y, -z, -w );
	}
	const TVector4& operator = ( const TVector4& b )
	{
		x = b.x;
		y = b.y;
		z = b.z;
		w = b.w;

        return *this;
    }
    const TVector4& operator += (const T s) 
    {
        x += s;
        y += s;
        z += s;
		w += s;

        return *this;
    } 
    const TVector4& operator += (const TVector4& b) 
    {
        x += b.x;
        y += b.y;
        z += b.z;
		w += b.w;

        return *this;
    } 
    const TVector4& operator -= (const T s) 
    {
        x -= s;
        y -= s;
        z -= s;
		w -= s;

        return *this;
    } 
    const TVector4& operator -= (const TVector4& b) 
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
		w -= b.w;

        return *this;
    } 
    const TVector4& operator *= (const T s)
    {
        x *= s;
        y *= s;
        z *= s;
		w *= s;

        return *this;
    }
    const TVector4& operator *= (const TVector4& b)
    {
        x *= b.x;
        y *= b.y;
        z *= b.z;
		w *= b.w;

        return *this;
    }
    const TVector4& operator /= (const T s)
    {
        const T r = 1 / s;//reciprocal

        x *= r;
        y *= r;
        z *= r;
		w *= r;

        return *this;
    }
    const TVector4& operator /= (const TVector4& b)
    {
        x /= b.x;
        y /= b.y;
        z /= b.z;
        w /= b.w;

        return *this;
    }
    const TVector4 operator + (const T s) const
    {
        return TVector4(x + s, y + s, z + s, w + s);
    }
    const TVector4 operator + (const TVector4& b) const
    {
        return TVector4(x + b.x, y + b.y, z + b.z, w + b.w);
    }
    friend inline const TVector4 operator + (const T s, const TVector4& v)
    {
        return v + s;
    }
    const TVector4 operator - (const T s) const
    {
        return TVector4(x - s, y - s, z - s, w - s);
    }
    const TVector4 operator - (const TVector4& b) const
    {
        return TVector4(x - b.x, y - b.y, z - b.z, w - b.w);
    }
    friend inline const TVector4 operator - (const T s, const TVector4& v)
    {
        return -v + s;
    }
    const TVector4 operator * (const T s) const
    {
        return TVector4(x*s, y*s, z*s, w*s);
    }
    const TVector4 operator * (const TVector4& b) const
    {
        return TVector4(x*b.x, y*b.y, z*b.z, w*b.w);
    }
    friend inline const TVector4 operator * (const T s, const TVector4& v)
    {
        return v * s;
    }
    const TVector4 operator / (const T s) const
    {
        const T r = 1 / s;//reciprocal

        return TVector4(x*r, y*r, z*r, w*r);
    }
    const TVector4 operator / (const TVector4& b) const
    {
        return TVector4(x/b.x, y/b.y, z/b.z, w/b.w);
    }
    friend inline const TVector4 operator / (const T s, const TVector4& v)
    {
        return TVector4(s/v.x, s/v.y, s/v.z, s/v.w);
    }
    bool operator > (const TVector4 &other) const { return ((x>other.x) && (y>other.y) && (z>other.z) && (w>other.w)); }
    bool operator < (const TVector4 &other) const { return ((x<other.x) && (y<other.y) && (z<other.z) && (w<other.w)); }
    bool operator >= (const TVector4 &other) const { return ((x>=other.x) && (y>=other.y) && (z>=other.z) && (w>=other.w)); }
    bool operator <= (const TVector4 &other) const { return ((x<=other.x) && (y<=other.y) && (z<=other.z) && (w<=other.w)); }
};

//The most common use of this template, as a floating point vector
typedef TVector4<float> LTVector4;

//handles expanding a vector into comma separated components
#define VEC4_EXPAND(v)					(v).x, (v).y, (v).z, (v).w

//NaN detection support for a vector
template <typename T>
inline bool LTIsNaN(const TVector4<T>& Val)	{ return LTIsNaN(Val.x) || LTIsNaN(Val.y) || LTIsNaN(Val.z) || LTIsNaN(Val.w); }

//Serialization functions
template<class T>
inline void LTStream_Read(ILTInStream *pStream, TVector4<T> &vec)
{
    STREAM_READ(vec.x);
    STREAM_READ(vec.y);
    STREAM_READ(vec.z);
	STREAM_READ(vec.w);
}

template<class T>
inline void LTStream_Write(ILTOutStream *pStream, const TVector4<T> &vec)
{
    STREAM_WRITE(vec.x);
    STREAM_WRITE(vec.y);
    STREAM_WRITE(vec.z);
    STREAM_WRITE(vec.w);
}

#endif//doxygen
#endif
