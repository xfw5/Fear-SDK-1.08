// *********************************************************************** //
//
// MODULE  : ltcriticalsection.h
//
// PURPOSE : Main header file for LTCriticalSection interface.  Selects appropriate
//			 class based on platform.
//
// CREATED : 05/25/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTCRITICALSECTION_H__
#define __LTCRITICALSECTION_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#if defined(PLATFORM_WIN32)

	#include "sys/win32/win32_ltcriticalsection.h"
	typedef CWin32_LTCriticalSection		  CLTCriticalSection;

#elif defined(PLATFORM_XENON)

	#include "sys/xenon/xenon_ltcriticalsection.h"
	typedef CXenon_LTCriticalSection		  CLTCriticalSection;

#elif defined(PLATFORM_LINUX)

	#include "sys/linux/linux_ltcriticalsection.h"
	typedef CLinux_LTCriticalSection 		  CLTCriticalSection;

#endif

#endif // __LTTHREADS_H__
