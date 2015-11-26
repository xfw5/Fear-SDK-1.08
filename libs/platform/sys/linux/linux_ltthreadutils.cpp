// *********************************************************************** //
//
// MODULE  : linux_ltthreadutils.cpp
//
// PURPOSE : Linux implementation of LTThreadUtils.
//
// CREATED : 06/01/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltthreadutils.h"
#include <unistd.h>

void LTThreadUtils::Sleep(uint32 nSleepMS)
{
	::usleep(nSleepMS * 1000);
}

void LTThreadUtils::RelinquishTimeslice()
{
	::sched_yield();
}

void LTThreadUtils::SetThreadDebugName(const char* pszDebugName)
{
	LTUNREFERENCED_PARAMETER(pszDebugName);
}
