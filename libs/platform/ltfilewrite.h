// *********************************************************************** //
//
// MODULE  : ltfilewrite.h
//
// PURPOSE : Main header file for LTFileWrite interface.  Selects appropriate
//			 class based on platform.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTFILEWRITE_H__
#define __LTFILEWRITE_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

// Win32 class typedefs
#if defined(PLATFORM_WIN32)

	#include "sys/win32/win32_ltfilewrite.h"
	typedef CWin32_LTFileWrite CLTFileWrite;

// Xenon class typedefs
#elif defined(PLATFORM_XENON)

	#include "sys/xenon/xenon_ltfilewrite.h"
	typedef CXenon_LTFileWrite CLTFileWrite;

// Linux class typedefs
#elif defined(PLATFORM_LINUX)

	#include "sys/linux/linux_ltfilewrite.h"
	typedef CLinux_LTFileWrite CLTFileWrite;

#endif

#endif // __LTFILEWRITE_H__
