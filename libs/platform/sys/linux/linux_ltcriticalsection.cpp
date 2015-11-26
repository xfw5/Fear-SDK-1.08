// *********************************************************************** //
//
// MODULE  : linux_ltcriticalsection.h
//
// PURPOSE : Linux implementation class for critical sections.
//
// CREATED : 05/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include "linux_ltcriticalsection.h"

CLinux_LTCriticalSection::CLinux_LTCriticalSection()
{
	::pthread_mutexattr_init(&m_mutexAttr);
	::pthread_mutexattr_settype(&m_mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
	::pthread_mutex_init(&m_mutex, &m_mutexAttr);
}

CLinux_LTCriticalSection::~CLinux_LTCriticalSection()
{
	::pthread_mutex_destroy(&m_mutex);
	::pthread_mutexattr_destroy(&m_mutexAttr);
}

void CLinux_LTCriticalSection::Enter()
{
	::pthread_mutex_lock(&m_mutex);
}

void CLinux_LTCriticalSection::Leave()
{
	::pthread_mutex_unlock(&m_mutex);
}

