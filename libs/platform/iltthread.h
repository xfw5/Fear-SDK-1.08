// *********************************************************************** //
//
// MODULE  : iltthread.h
//
// PURPOSE : Interface class for threads.
//
// CREATED : 05/26/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTTHREAD_H__
#define __ILTTHREAD_H__

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

class ILTThread
{
public:

	// thread function type
	typedef uint32 (*UserThreadFunction)(void*);

	//create and start the thread. This takes an optional processor affinity that can be used
	//to make the created thread run more frequently on the specified processor. This processor
	//value is platform specific. -1 indicates no affinity:
	//PC/Linux: Currently ignored
	//Xenon: [0..5], where 0,1 are hardware threads on core 1, and so on through core 3.
	virtual void Create(UserThreadFunction pUserThreadFunction, void* pArgument, uint32 nProcessorAffinity = (uint32)-1) = 0;

	// wait for the thread to exit - note that this is typically used in 
	// conjunction with a thread event to tell the thread to shutdown
	virtual void WaitForExit() = 0;

	// check to see if the thread has been created
	virtual bool IsCreated() = 0;

};


#endif // __ILTTHREAD_H__
