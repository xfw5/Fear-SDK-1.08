// *********************************************************************** //
//
// MODULE  : ltfileread.h
//
// PURPOSE : Main header file for LTFileRead interface.  Selects appropriate
//			 class based on platform.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTFILEREAD_H__
#define __LTFILEREAD_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

// Win32 class typedefs
#if defined(PLATFORM_WIN32)

	#include "sys/win32/win32_ltfileread.h"
	typedef CWin32_LTFileRead  CLTFileRead;

// Xenon class typedefs
#elif defined(PLATFORM_XENON)

	#include "sys/xenon/xenon_ltfileread.h"
	typedef CXenon_LTFileRead  CLTFileRead;

// Linux class typedefs
#elif defined(PLATFORM_LINUX)

	#include "sys/linux/linux_ltfileread.h"
	typedef CLinux_LTFileRead  CLTFileRead;

#endif

#endif // __LTFILEREAD_H__
