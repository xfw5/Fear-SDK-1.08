// *********************************************************************** //
//
// MODULE  : lttimeutils.h
//
// PURPOSE : Helpers for time operations.
//
// CREATED : 05/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTTIMEUTILS_H__
#define __LTTIMEUTILS_H__

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

//this type is a platform specific type that is used by the high precision timers
typedef uint64	TLTPrecisionTime;

class LTTimeUtils
{
public:

	// Gets the current time elapsed since execution start in seconds
	static float GetTimeS();

	// Gets the current time elapsed since execution start in milliseconds
	static uint32 GetTimeMS();

	// This obtains a high precision timing value. This cannot be used directly, but
	// elapsed time can be found by using this and the end precision time
	static TLTPrecisionTime GetPrecisionTime();

	// Given a previously taken precision time, this will determine the amount of time
	// that has elapsed since that time and return the delta as a double precision
	// time interval in milliseconds
	static double GetPrecisionTimeIntervalMS(TLTPrecisionTime StartTime, TLTPrecisionTime EndTime);

	// Given a previously taken precision time, this will determine the amount of time
	// that has elapsed since that time and return the delta as a double precision
	// time interval in seconds
	static double GetPrecisionTimeIntervalS(TLTPrecisionTime StartTime, TLTPrecisionTime EndTime);
	
#if defined(PLATFORM_LINUX)
	
	// Gets the current time as timeval structure (from gettimeofday)
	static void GetCurrentTime(struct timeval& sTimeVal);
	
#endif // PLATFORM_LINUX

};

#endif // __LTTIMEUTILS_H__
