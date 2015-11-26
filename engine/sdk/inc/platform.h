/*!
Platform configuration header
*/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

//----------------------------------------------------------------------------
// Basic system.  These defines are mutually exclusive.
//----------------------------------------------------------------------------

#if defined( _XBOX )

	#define PLATFORM_XENON

#elif defined ( _LINUX )

#define PLATFORM_LINUX

#elif defined( _WIN32 )

#define PLATFORM_WIN32

#endif


//----------------------------------------------------------------------------
// Modularity of the platform.  These defines are mutually exclusive.
//----------------------------------------------------------------------------

#if defined( _SEM )

	// Single execution module
	#define PLATFORM_SEM

#else

	// Multiple execution modules
	#define PLATFORM_MODULAR

#endif

//----------------------------------------------------------------------------
// System-specific defines.
//----------------------------------------------------------------------------

// Linux
#if defined(PLATFORM_LINUX)

#include <linux/limits.h>

#define MODULE_EXPORT
#define MODULE_IMPORT
#define FILE_PATH_SEPARATOR "/"
#define FILE_NEWLINE "\n"
#define _MAX_PATH PATH_MAX
#define MAX_PATH _MAX_PATH
#define _MAX_FNAME 256
#define _MAX_DIR 256
#define _MAX_EXT 256
#define _MAX_DRIVE 0
#define FALSE 0
#define TRUE 1
#define NO_VTABLE
#define NO_INLINE
#define FORCE_INLINE
#define INFINITE (uint32)-1
#define _S_IFDIR S_IFDIR
#define INVALID_SOCKET (SOCKET)-1
#define SOCKET_ERROR -1

typedef unsigned char BYTE;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef unsigned char UCHAR;
typedef long HRESULT;
typedef unsigned int SOCKET;
typedef unsigned short int WORD;
typedef unsigned long DWORD;
typedef int INT;

// Win32
#elif defined(PLATFORM_WIN32)

#define FILE_PATH_SEPARATOR "\\"
#define FILE_NEWLINE "\r\n"

#define NO_VTABLE __declspec(novtable)
#define NO_INLINE __declspec(noinline)
#define FORCE_INLINE __forceinline
#define MODULE_EXPORT __declspec(dllexport)
#define MODULE_IMPORT __declspec(dllimport)

// Enable Win2K+ feature set
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif						

#endif

#endif //!__PLATFORM_H__
