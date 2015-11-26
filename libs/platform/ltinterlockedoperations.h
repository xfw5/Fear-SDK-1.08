// *********************************************************************** //
//
// MODULE  : ltinterlockedoperations.h
//
// PURPOSE : Helpers for thread interlocked operations.
//
// CREATED : 05/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTINTERLOCKEDOPERATIONS_H__
#define __LTINTERLOCKEDOPERATIONS_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

class LTInterlockedOperations
{
public:

	// Increments the specified numeric variable.  Both the return value and
	// pAddend contain the incremented value.
	static void InterlockedIncrement(uint32* pAddend);

	// Decrements the specified numeric variable.  Both the return value and
	// pAddend contain the decremented value.
	static uint32 InterlockedDecrement(uint32* pAddend);
};

#if defined(PLATFORM_WIN32) 

// inline the Win32 versions
#include "sys/win32/win32_ltinterlockedoperations.inl"

#elif defined(PLATFORM_XENON)

// inline the Xenon versions
#include "sys/xenon/xenon_ltinterlockedoperations.inl"

#endif // PLATFORM_WIN32

#endif // __LTINTERLOCKEDOPERATIONS_H__

