//-----------------------------------------------------------------------------------
// LTMath.h
// 
// This file provides the definitions for the math operations used by the engine and
// game code. The purpose of not using the underlying math operations directly is to
// allow for better performance and platform independance. Certain math operations are
// not ANSI standard, and also there are different function names for operations depending
// upon the type of the operation, which makes the operations hard to templatize. In
// addition this allows the use of the most efficient functions for each platform and
// data type.
//
//-----------------------------------------------------------------------------------
#ifndef __LTMATH_H__
#define __LTMATH_H__

#include <stdlib.h>
#include <math.h>
#include <float.h>

#ifndef __LTINTEGER_H__
#	include "ltinteger.h"
#endif

#ifndef __PLATFORM_H__
#	include "platform.h"
#endif

#ifndef __LTASSERT_H__
#	include "ltassert.h"
#endif

//this function handles determination of if the provided value is Not a Number, indicating that
//it has undergone some mathematical operation that has resulted in a number that cannot be
//accurately represented (divide by zero, op with another NaN, imaginary number, etc)
#if defined(PLATFORM_WIN32) || defined (PLATFORM_XENON)

	inline bool LTIsNaN(float fVal)		{ return _finite((double)fVal) == 0; }
	inline bool LTIsNaN(double fVal)	{ return _finite(fVal) == 0; }

#elif defined(PLATFORM_LINUX)

	inline bool LTIsNaN(float fVal)		{ return isnan((double)fVal) != 0; }
	inline bool LTIsNaN(double fVal)	{ return isnan(fVal) != 0; }

#endif

//this function will throw an assert if the provided value is Not a Number. See the above function
//for more information on this. This is intended only for debugging purposes and therefore does not
//return the status of the evaluation to prevent improper usage of it at runtime. This is defined
//as a template in order to allow for other types to define their own LTIsNaN functions
template <typename T>
inline void LTCheckNaN(const T& Val)	{ LTASSERT(!LTIsNaN(Val), "Warning: NaN encountered"); }

//provides functionality for square root operations. This will throw an assert if the provided value
//is < 0 as this will generate undefined results
inline float	LTSqrt(float fVal)		{ LTASSERT(fVal >= 0.0f, "Invalid input to square root"); return sqrtf(fVal); }
inline double	LTSqrt(double fVal)		{ LTASSERT(fVal >= 0.0, "Invalid input to square root"); return sqrt(fVal); }

//provides support for the standard trigonometric operations. These all operate in radians and
//have no invalid input
inline float	LTSin(float fVal)		{ return sinf(fVal); }
inline double	LTSin(double fVal)		{ return sin(fVal); }

inline float	LTCos(float fVal)		{ return cosf(fVal); }
inline double	LTCos(double fVal)		{ return cos(fVal); }

inline float	LTTan(float fVal)		{ return tanf(fVal); }
inline double	LTTan(double fVal)		{ return tan(fVal); }

//provides support for the inverse sine and inverse cosine functionalities. These will assert
//if the input is outside of the range of [-1..1] as this is the only defined range for these
//functions. The results returned are in quadrants I and IV for ArcSin, and I and II for ArcCos
inline float	LTArcSin(float fVal)	{ LTASSERT(fVal >= -1.0f && fVal <= 1.0f, "Invalid input to ArcSin"); return asinf(fVal); }
inline double	LTArcSin(double fVal)	{ LTASSERT(fVal >= -1.0 && fVal <= 1.0, "Invalid input to ArcSin"); return asin(fVal); }

inline float	LTArcCos(float fVal)	{ LTASSERT(fVal >= -1.0f && fVal <= 1.0f, "Invalid input to ArcCos"); return acosf(fVal); }
inline double	LTArcCos(double fVal)	{ LTASSERT(fVal >= -1.0 && fVal <= 1.0, "Invalid input to ArcCos"); return acos(fVal); }

//provides support for the arc tangent functionality. This takes the Y and X values in order to 
//ensure that it is properly defined even if X equals zero. The only degenerate case is when both
//Y and X equals zero, which will trigger an assert. 
inline float	LTArcTan(float fY, float fX)	{ LTASSERT(fY != 0.0f || fX != 0.0f, "Invalid input to ArcTan"); return atan2f(fY, fX); }
inline double	LTArcTan(double fY, double fX)	{ LTASSERT(fY != 0.0 || fX != 0.0, "Invalid input to ArcTan"); return atan2(fY, fX); }

//performs standard division. This will throw an assert if the divisor is zero.
inline int32	LTDiv(int32 nNumerator, int32 nDenominator)		{ LTASSERT(nDenominator != 0, "Divide by zero"); return nNumerator / nDenominator; }
inline uint32	LTDiv(uint32 nNumerator, uint32 nDenominator)	{ LTASSERT(nDenominator != 0, "Divide by zero"); return nNumerator / nDenominator; }
inline float	LTDiv(float fNumerator, float fDenominator)		{ LTASSERT(fDenominator != 0.0f, "Divide by zero"); return fNumerator / fDenominator; }
inline double	LTDiv(double fNumerator, double fDenominator)	{ LTASSERT(fDenominator != 0.0, "Divide by zero"); return fNumerator / fDenominator; }

//provides absolute value support for the normal types
inline int32	LTAbs(int32 nVal)			{ return abs(nVal); }
inline float	LTAbs(float fVal)			{ return fabsf(fVal); }
inline double	LTAbs(double fVal)			{ return fabs(fVal); }

//provides support for determining if a number is nearly equal to another number within the provided epsilon
inline bool		LTNearlyEquals(float fA, float fB, float fEpsilon)		{ return LTAbs(fA - fB) <= fEpsilon; }
inline bool		LTNearlyEquals(double fA, double fB, double fEpsilon)	{ return LTAbs(fA - fB) <= fEpsilon; }

//this templatized function will handle squaring the provided value using the * operator. This will
//work with all types that provide this operator and is useful for when an expensive expression needs
//to be squared
template <typename T>
inline T LTSqr(const T& Val)				{ return Val * Val; }

//given two values, this will return the difference between the two values using the < and - operators
template<typename T>
inline T LTDIFF_I(T a, T b)					{ return ((a < b) ? (b - a) : (a - b)); }
template<typename T, typename TB>
inline T LTDIFF(T a, TB b)					{ return LTDIFF_I<T>(a, b); }

//given two values, this will determine the smaller of the two values using the < operator
template<typename T>
inline T LTMIN_I(T a, T b)					{ return ((a < b) ? a : b); }
template<typename T, typename TB>
inline T LTMIN(T a, TB b)					{ return LTMIN_I<T>(a, b); }

//given two values this will return the larger of the two values using the < operator
template<typename T>
inline T LTMAX_I(T a, T b)					{ return ((a < b) ? b : a); }
template<typename T, typename TB>
inline T LTMAX(T a, TB b)					{ return LTMAX_I<T>(a, b); }

//given a value and two extents, this will ensure that min <= val <= max using the < operator
template<typename T>
inline T LTCLAMP_I(T a, T min, T max)		{ return ((a < min) ? min : ((a < max) ? a : max)); }
template<typename T, typename TB, typename TC>
inline T LTCLAMP(T a, TB min, TC max)		{ return LTCLAMP_I<T>(a, min, max); }

//given two values and an interpolant, this will interpolate between the values. Values outside of
//[0..1] will cause extrapolation. This uses the +, - and * operators
template<typename T, typename TINTERP>
inline T LTLERP_I(T min, T max, TINTERP t)	{ return (min + ((max - min) * t)); }
template<typename T, typename TMAX, typename TINTERP>
inline T LTLERP(T min, TMAX max, TINTERP t)	{ return LTLERP_I<T, TINTERP>(min, max, t); }

//constants for definition of circles
#define MATH_PI             3.14159265358979323846f
#define MATH_HALFPI			(MATH_PI * 0.5f)
#define MATH_CIRCLE			(MATH_PI * 2.0f)
#define MATH_TWOPI			(MATH_CIRCLE)
#define MATH_ONE_OVER_PI	(1.0f / MATH_PI)

//standard constants
#define MATH_EPSILON        0.00000000001f
#define MATH_ONE_OVER_255	(1.0f / 255.0f)
#define MATH_ONE_OVER_128	(1.0f / 128.0f)
#define MATH_SQRT2			1.41421356237309504880f

//handles conversion between degrees and radians
inline float	MATH_DEGREES_TO_RADIANS(float fVal)		{ return fVal * (MATH_PI / 180.0f); }
inline double	MATH_DEGREES_TO_RADIANS(double fVal)	{ return fVal * (double)(MATH_PI / 180.0f); }

inline float	MATH_RADIANS_TO_DEGREES(float fVal)		{ return fVal * (180.0f / MATH_PI); }
inline double	MATH_RADIANS_TO_DEGREES(double fVal)	{ return fVal * (double)(180.0f / MATH_PI); }

#endif

