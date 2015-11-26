// *********************************************************************** //
//
// MODULE  : ltthreadutils.h
//
// PURPOSE : Helpers for thread operations
//
// CREATED : 06/01/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTTHREADUTILS_H__
#define __LTTHREADUTILS_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

class LTThreadUtils
{
public:

	// put the current thread to sleep for the specified number of ms
	static void Sleep(uint32 nSleepMS);
	
	// relinquish the remainder of this thread's timeslice
	static void RelinquishTimeslice();

	//called to set the name of the current thread for debugging purposes. This does not do anything
	//in release or on certain platforms.
	static void SetThreadDebugName(const char* pszDebugName);
};


#endif // __LTTHREADUTILS_H__
