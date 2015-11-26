#ifndef __STDAFX_H__
#define __STDAFX_H__

#include "platform.h"

#include "ltbasedefs.h"
#include "stdio.h"

#if defined(PLATFORM_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <WinSock2.h>
	#include <Mmsystem.h>
#elif defined(PLATFORM_LINUX)
	#include <pthread.h>
	#include <sys/types.h>
	#include <sys/socket.h>
#endif

#endif // __STDAFX_H__
