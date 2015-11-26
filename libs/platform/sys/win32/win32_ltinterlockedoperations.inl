// *********************************************************************** //
//
// MODULE  : win32_ltinterlockedoperations.inl
//
// PURPOSE : Win32 inline file for LTInterlockedOperations.
//
// CREATED : 05/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include "windows.h"

inline void LTInterlockedOperations::InterlockedIncrement(uint32* pAddend)
{
	::InterlockedIncrement((long volatile*)pAddend);
}

inline uint32 LTInterlockedOperations::InterlockedDecrement(uint32* pAddend)
{
	return ::InterlockedDecrement((long volatile*)pAddend);
}
