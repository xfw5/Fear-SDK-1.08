// *********************************************************************** //
//
// MODULE  : linux_ltthread.h
//
// PURPOSE : Linux implementation of thread wrapper class.
//
// CREATED : 05/26/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LINUX_LTTHREAD_H__
#define __LINUX_LTTHREAD_H__

#include "iltthread.h"
#include <pthread.h>
#include "ltassert.h"

class CLinux_LTThread : protected ILTThread
{
public:

	CLinux_LTThread();
	virtual ~CLinux_LTThread();

	// create and start the thread
	virtual void Create(UserThreadFunction pUserThreadFunction, void* pArgument, uint32 nProcessorAffinity = (uint32)-1);

	// wait for the thread to exit - note that this is typically used in 
	// conjunction with a thread event to tell the thread to shutdown
	virtual void WaitForExit();
	
	// check to see if the thread has been created
	virtual bool IsCreated();

	// thread argument structure
	struct InternalThreadArgument
	{
		UserThreadFunction	m_pUserThreadFunction;
		void*				m_pArgument;
	};

private:

	// handle to the thread
	pthread_t m_hThread;

	// argument used with platform-specific thread function
	InternalThreadArgument m_threadArgument;
};

inline CLinux_LTThread::CLinux_LTThread()
	: m_hThread(0)
{
}

inline CLinux_LTThread::~CLinux_LTThread()
{
}

inline bool CLinux_LTThread::IsCreated()
{
	return m_hThread != 0 ? true : false;
}

#endif // __LINUX_LTTHREAD_H__

