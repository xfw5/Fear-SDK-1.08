//------------------------------------------------------------------------------
// ILTLoadingProgress.h
//
// Provides support for registering listeners that want to be notified about
// loading events, and also triggering events to those listeners.
//
// (c) 1999-2004 Monolith Productions, Inc.  All Rights Reserved
//
//------------------------------------------------------------------------------
#ifndef __ILTLOADINGPROGRESS_H__
#define __ILTLOADINGPROGRESS_H__

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

//this is the list of currently supported task. The first set is used by the engine and
//should only be triggered by the engine. The rest are free for game code systems to use
//for whatever purposes that they determine necessary
enum ELoadingProgressTask
{
    //loading of the world geometry
	eLoadingProgressTask_WorldBsp,
	//loading and setup of objects
	eLoadingProgressTask_Objects,
	//loading of global assets associated with this level
	eLoadingProgressTask_Assets,

	//user defined groups that the game code can use for whatever tasks it needs
	eLoadingProgressTask_User0,
	eLoadingProgressTask_User1,
	eLoadingProgressTask_User2,
	eLoadingProgressTask_User3,
	eLoadingProgressTask_User4,
	eLoadingProgressTask_User5,
	eLoadingProgressTask_User6,
	eLoadingProgressTask_User7,

	//the total number of tasks supported, this must come last to have the correct value
	eLoadingProgressTask_NumTasks,
};

//this is the prototype for a listener to the loading system. Callback functions must
//match this prototype. This will be provided with the current task that is being performed,
//the progress of the task, and also the user data that was registered with this listener
typedef void (*TLoadingProgressCallbackFn)(ELoadingProgressTask eTask, float fProgress, void* pUser);

class ILTLoadingProgress :
	public IBase
{
public:

	//module support
	interface_version(ILTLoadingProgress, 0);

	virtual ~ILTLoadingProgress()	{}

	//called to set the listener to the loader progress events. This function will 
	//be called for each event and provided with the user data, the event type, and
	//the progress for that event. Note that calling this will remove any existing listeners.
	//NULL can be safely provided for the listener or user data.
	virtual void	SetLoadingCallback(TLoadingProgressCallbackFn pfnCB, void* pUser) = 0;

	//called to trigger a loading callback. This will trigger the currently registered
	//loading listener providing the specified task and the progress. This progress will
	//be clamped to the range of [0..1]. This will do nothing if no listener is subscribed
	virtual void	TriggerLoadingCallback(ELoadingProgressTask eTask, float fProgress) = 0;	

	//called to service the network and message queue, and trigger the loading callback.
	virtual void	UpdateProgress(ELoadingProgressTask eTask, float fProgress) = 0;

};

#endif
