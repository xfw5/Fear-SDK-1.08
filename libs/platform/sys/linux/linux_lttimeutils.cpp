// *********************************************************************** //
//
// MODULE  : linux_lttimeutils.cpp
//
// PURPOSE : Linux implementation of LTTimeUtils.
//
// CREATED : 05/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "lttimeutils.h"
#include <sys/time.h>
#include <ltautocriticalsection.h>

// Internal class to initialize the timer resolution and base time
class CLinux_TimeInit
{
public:

	CLinux_TimeInit()
	{
		// initialize the base time
		struct timeval timeValue;
		::gettimeofday(&timeValue, NULL);
		m_nBaseTime = (timeValue.tv_sec * 1000) + (timeValue.tv_usec / 1000);
	}

	uint32 GetBaseTime() const { return m_nBaseTime; }

private:
		
	// base time - initialized when this module is loaded
	uint32 m_nBaseTime;

};

// global for time initialization
static CLinux_TimeInit g_Linux_TimeInit;

// critical section to protect access to the gettimeofday() system call
static CLTCriticalSection g_cTimeCS;

// previous time value used to make sure the clock always moves forward
static uint32 g_nLastTime = 0;

float LTTimeUtils::GetTimeS()
{
	CLTAutoCriticalSection cTimeAutoCS(g_cTimeCS);
	
	struct timeval timeValue;
	if (::gettimeofday(&timeValue, NULL) == -1)
	{
		return g_nLastTime / 1000;
	}
	
	uint32 nTime = (timeValue.tv_sec * 1000) + (timeValue.tv_usec / 1000) - g_Linux_TimeInit.GetBaseTime();
	if (nTime < g_nLastTime)
	{
		nTime = g_nLastTime;
	}
	else
	{
		g_nLastTime = nTime;
	}
	
	return (float) nTime / 1000;
}

uint32 LTTimeUtils::GetTimeMS()
{
	CLTAutoCriticalSection cTimeAutoCS(g_cTimeCS);
	
	struct timeval timeValue;
	if (::gettimeofday(&timeValue, NULL) == -1)
	{
		return g_nLastTime;
	}
	
	uint32 nTime = (timeValue.tv_sec * 1000) + (timeValue.tv_usec / 1000) - g_Linux_TimeInit.GetBaseTime();
	if (nTime < g_nLastTime)
	{
		nTime = g_nLastTime;
	}
	else
	{
		g_nLastTime = nTime;
	}
	
	return nTime;
}

TLTPrecisionTime LTTimeUtils::GetPrecisionTime()
{
	CLTAutoCriticalSection cTimeAutoCS(g_cTimeCS);
	
	struct timeval timeValue;
	::gettimeofday(&timeValue, NULL);
	return (TLTPrecisionTime)((timeValue.tv_sec * 1000000) + timeValue.tv_usec);
}

double LTTimeUtils::GetPrecisionTimeIntervalMS(TLTPrecisionTime StartTime, TLTPrecisionTime EndTime)
{
	return ((double)EndTime - (double)StartTime) / 1000.0f;
}

double LTTimeUtils::GetPrecisionTimeIntervalS(TLTPrecisionTime StartTime, TLTPrecisionTime EndTime)
{
	return ((double)EndTime - (double)StartTime) / 1000000.0f;
}

void LTTimeUtils::GetCurrentTime(struct timeval& sTimeVal)
{
	CLTAutoCriticalSection cTimeAutoCS(g_cTimeCS);

	::gettimeofday(&sTimeVal, NULL);
}

