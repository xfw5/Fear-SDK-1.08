// *********************************************************************** //
//
// MODULE  : ltthreadevent.h
//
// PURPOSE : Main header file for LTThreadEvent objects.  Selects appropriate
//			 class based on platform.
//
// CREATED : 05/25/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTTHREADEVENT_H__
#define __LTTHREADEVENT_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#if defined(PLATFORM_WIN32)

	#include "sys/win32/win32_ltthreadevent.h"
	typedef CWin32_LTThreadEvent<AutoReset>	  CLTThreadEvent;
	typedef CWin32_LTThreadEvent<ManualReset> CLTThreadEventManual;

#elif defined(PLATFORM_XENON)

	#include "sys/xenon/xenon_ltthreadevent.h"
	typedef CXenon_LTThreadEvent<AutoReset>	  CLTThreadEvent;
	typedef CXenon_LTThreadEvent<ManualReset> CLTThreadEventManual;

#elif defined(PLATFORM_LINUX)

	#include "sys/linux/linux_ltthreadevent.h"
	typedef CLinux_LTThreadEvent<AutoReset>	  CLTThreadEvent;
	typedef CLinux_LTThreadEvent<ManualReset> CLTThreadEventManual;

#endif

#endif // __LTTHREADS_H__
