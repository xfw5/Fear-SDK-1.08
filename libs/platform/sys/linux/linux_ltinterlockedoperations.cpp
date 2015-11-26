// *********************************************************************** //
//
// MODULE  : linux_ltinterlockedoperations.cpp
//
// PURPOSE : Linux implementation of LTInterlockedOperations.
//
// CREATED : 05/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <pthread.h>
#include "ltinterlockedoperations.h"

static pthread_mutex_t g_hInterlockedMutex = PTHREAD_MUTEX_INITIALIZER;

void LTInterlockedOperations::InterlockedIncrement(uint32* pAddend)
{
	::pthread_mutex_lock(&g_hInterlockedMutex);
	++(*pAddend);
	::pthread_mutex_unlock(&g_hInterlockedMutex);
}

uint32 LTInterlockedOperations::InterlockedDecrement(uint32* pAddend)
{
	::pthread_mutex_lock(&g_hInterlockedMutex);
	uint32 nRV = --(*pAddend);
	::pthread_mutex_unlock(&g_hInterlockedMutex);

	return nRV;
}


