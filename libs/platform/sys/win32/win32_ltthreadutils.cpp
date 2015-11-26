// *********************************************************************** //
//
// MODULE  : win32_ltthreadutils.cpp
//
// PURPOSE : Win32 implementation of LTThreadUtils.
//
// CREATED : 06/01/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltthreadutils.h"

void LTThreadUtils::Sleep(uint32 nSleepMS)
{
	::Sleep(nSleepMS);
}

void LTThreadUtils::RelinquishTimeslice()
{
	::Sleep(0);
}

//structure used for setting a thread name
struct THREADNAME_INFO
{
	DWORD dwType;		// must be 0x1000
	LPCSTR szName;		// pointer to name (in user addr space)
	DWORD dwThreadID;	// thread ID (-1=caller thread)
	DWORD dwFlags;		// reserved for future use, must be zero
};

void LTThreadUtils::SetThreadDebugName(const char* pszDebugName)
{
#ifdef _DEBUG
	//the exception that we need to raise in order to set our thread name
	const uint32 knExceptionID = 0x406D1388;

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = pszDebugName;
	info.dwThreadID = (DWORD)-1;
	info.dwFlags = 0;

	__try
	{
		RaiseException(knExceptionID, 0, sizeof(info)/sizeof(DWORD), (DWORD*)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
	}
#else
	LTUNREFERENCED_PARAMETER(pszDebugName);
#endif
}
