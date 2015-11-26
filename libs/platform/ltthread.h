// *********************************************************************** //
//
// MODULE  : ltthread.h
//
// PURPOSE : Main header file for LTThread object.  Selects appropriate
//			 classes based on platform.
//
// CREATED : 05/25/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTTHREAD_H__
#define __LTTHREAD_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#if defined(PLATFORM_WIN32)

	#include "sys/win32/win32_ltthread.h"
	typedef CWin32_LTThread	CLTThread;

#elif defined(PLATFORM_XENON)

	#include "sys/xenon/xenon_ltthread.h"
	typedef CXenon_LTThread	CLTThread;

#elif defined(PLATFORM_LINUX)

	#include "sys/linux/linux_ltthread.h"
	typedef CLinux_LTThread	CLTThread;

#endif

#endif // __LTTHREADS_H__
