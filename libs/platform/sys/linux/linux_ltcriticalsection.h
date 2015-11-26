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

#ifndef __LINUX_LTCRITICALSECTION_H__
#define __LINUX_LTCRITICALSECTION_H__

#include "iltcriticalsection.h"
#include <pthread.h>
	
class CLinux_LTCriticalSection : protected ILTCriticalSection
{
public:

	CLinux_LTCriticalSection();
	~CLinux_LTCriticalSection();

	// lock the critical section
	virtual void Enter();

	// unlock the critical section
	virtual void Leave();

private:

	// Linux critical section (mutex) object
	pthread_mutex_t m_mutex;
	
	// attributes for mutex object
	pthread_mutexattr_t m_mutexAttr;
};

#endif // __LINUX_LTCRITICALSECTION_H__

