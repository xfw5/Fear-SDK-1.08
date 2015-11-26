// *********************************************************************** //
//
// MODULE  : win32_lttimeutils.cpp
//
// PURPOSE : Win32 implementation of LTTimeUtils.
//
// CREATED : 05/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "lttimeutils.h"

// Internal class to initialize the timer resolution and base time
class CWin32_TimeInit
{
public:

	CWin32_TimeInit()
	{
		// get the OS platform ID
		DWORD nPlatformID = GetPlatformId();

		// if we're on an NT kernel, initialize timer resolution to 1ms
		if (nPlatformID == VER_PLATFORM_WIN32_NT)
		{
			::timeBeginPeriod(1); 
		}
		
		// initialize the base time
		m_nBaseTime = ::timeGetTime();

		//determine our precision scales for the units we support
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		m_fPrecisionScaleS  = (Frequency.QuadPart == 0) ? 1.0 : 1.0 / (double)Frequency.QuadPart;
		m_fPrecisionScaleMS = (Frequency.QuadPart == 0) ? 1.0 : 1000.0 / (double)(Frequency.QuadPart);
	}

	~CWin32_TimeInit()
	{
		// get the OS platform ID
		DWORD nPlatformID = GetPlatformId();

		// if we're on an NT kernel, reset the timer resolution
		if (nPlatformID == VER_PLATFORM_WIN32_NT)
		{
			::timeEndPeriod(1);
		}
	}

	uint32 GetBaseTime() const			{ return m_nBaseTime; }

	//called to get the divisor to use for the precision timings
	double GetPrecisionScaleS() const	{ return m_fPrecisionScaleS; }
	double GetPrecisionScaleMS() const	{ return m_fPrecisionScaleMS; }

private:
	
	// helper to retrieve the platform ID
	DWORD GetPlatformId() 
	{
		OSVERSIONINFO osVersionInfo;
		ZeroMemory(&osVersionInfo, sizeof(osVersionInfo));
		osVersionInfo.dwOSVersionInfoSize = sizeof(osVersionInfo);    
		GetVersionEx(&osVersionInfo);
		return osVersionInfo.dwPlatformId;
	}

	// base time - initialized when this module is loaded
	uint32 m_nBaseTime;

	//1.0 / QueryPerformanceFrequency, used for fast scaling of precision times
	double m_fPrecisionScaleS;
	double m_fPrecisionScaleMS;
};

// global for time initialization
static CWin32_TimeInit g_Win32_TimeInit;

float LTTimeUtils::GetTimeS()
{
	return (float)(::timeGetTime() - g_Win32_TimeInit.GetBaseTime()) / 1000.0f;
}

uint32 LTTimeUtils::GetTimeMS()
{
	return (::timeGetTime() - g_Win32_TimeInit.GetBaseTime());
}

TLTPrecisionTime LTTimeUtils::GetPrecisionTime()
{
	LARGE_INTEGER CurrTicks;
	QueryPerformanceCounter(&CurrTicks);
	return (TLTPrecisionTime)CurrTicks.QuadPart;	
}

double LTTimeUtils::GetPrecisionTimeIntervalMS(TLTPrecisionTime StartTime, TLTPrecisionTime EndTime)
{
	return (EndTime - StartTime) * g_Win32_TimeInit.GetPrecisionScaleMS();	
}

double LTTimeUtils::GetPrecisionTimeIntervalS(TLTPrecisionTime StartTime, TLTPrecisionTime EndTime)
{
	return (EndTime - StartTime) * g_Win32_TimeInit.GetPrecisionScaleS();	
}


