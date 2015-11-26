#ifndef __LTINTEGER_H__
#define __LTINTEGER_H__

#include "platform.h"

/*!
Portable integer types. 
*/
typedef unsigned int        uint; // This is at least 32 bits

//8 bit
typedef char				int8;
typedef unsigned char		uint8;

//16 bit
typedef short int			int16;
typedef unsigned short int	uint16;

//32 bit
typedef int					int32;
typedef unsigned int		uint32;

//64 bit
#if defined(PLATFORM_LINUX)
	typedef long long			int64;
	typedef unsigned long long  uint64;
#else
	typedef __int64				int64;
	typedef unsigned __int64    uint64;
#endif


#endif
