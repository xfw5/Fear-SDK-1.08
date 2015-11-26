//-------------------------------------------------------------------------------
// ILTTimer.h
//
// Defines the engine interface that is responsible for handling creation, setup,
// and usage of timer objects. These timer objects are used to control the flow
// of time through the various systems and game objects. They are not distributed
// from the server to the client, and each server and client has a unique simulation
// timer that is used to control the main engine systems.
//-------------------------------------------------------------------------------
#ifndef __ILTTIMER_H__
#define __ILTTIMER_H__

//-------------------------------------------------------
// Includes
//-------------------------------------------------------

#ifndef __LTBASEDEFS_H__
#	include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

//-------------------------------------------------------
// Handles and Types
//-------------------------------------------------------

//this handle type represents an internal engine timer and is used to access properties
//and operations associated with that timer
class CEngineTimer;
typedef CEngineTimer* HENGINETIMER;

//-------------------------------------------------------
// ILTTimer
//-------------------------------------------------------
class ILTTimer:
	public IBase
{
public:

	//Module support
	interface_version(ILTTimer, 0);
	virtual ~ILTTimer() {}

	//------------------------------
	// Timer Lifetime
	//------------------------------

	//called to create a new engine timer object. This will default to having a time scale of
	//1.0 and with no clamping performed. This will return NULL if the timer could not be successfully
	//created. This created timer must have a corresponding release call made on it to avoid resource leaks.
	virtual HENGINETIMER	CreateTimer() = 0;

	//called to add a reference to an existing timer object. There must be a corresponding release call
	//made to avoid resource leaks.
	virtual LTRESULT		AddTimerRef(HENGINETIMER hTimer) = 0;

	//called to release a reference to a timer. The provided handle should not be used after this call
	//has been made.
	virtual LTRESULT		ReleaseTimer(HENGINETIMER hTimer) = 0;

	//------------------------------
	// Timer Hierarchy
	//------------------------------

	//called to assign a timer as the child of another timer so that it will receive timing events
	//from the parent timer. Note that nothing will be done if either timer is invalid.
	virtual LTRESULT		AddChildTimer(HENGINETIMER hParent, HENGINETIMER hChild) = 0;

	//called to remove a timer from the hierarchy so that it and it's children will no longer be updated
	virtual LTRESULT		DetachTimer(HENGINETIMER hTimer) = 0;

	//called to set the parent of this timer to be the system timer so that it will receive time updates
	//when the system time is updated
	virtual LTRESULT		AddSystemChildTimer(HENGINETIMER hChild) = 0;

	//------------------------------
	// Simulation Timer
	//------------------------------

	//called to access the simulation timer. The returned timer must have release called on it to
	//avoid a resource leak
	virtual HENGINETIMER	GetSimulationTimer() = 0;

	//------------------------------
	// Object Timers
	//------------------------------

	//called to set the timer for the provided object to the specified timer. This timer will then
	//be used to control the update delta and frequency for the object. If no timer is specified,
	//the object will just use the simulation timer
	virtual LTRESULT		SetObjectTimer(HOBJECT hObject, HENGINETIMER hTimer) = 0;

	//called to get the timer associated with the object. This will return NULL if no timer is associated
	//with the object, in which case it is using the simulation timer. The returned timer must be released
	//in order to avoid a resource leak.
	virtual HENGINETIMER	GetObjectTimer(HOBJECT hObject) = 0;


	//------------------------------
	// Timer Properties
	//------------------------------

	//called to set the time scale associated with the timer. This is specified as an integer fraction,
	//so for 50%, 1/2 could be used, for 200% 2/1 can be used, and so on. This value will be used to
	//control the speed that systems are updated at. These provided fractional values should each be less
	//than 65k
	virtual LTRESULT		SetTimerTimeScale(HENGINETIMER hTimer, uint32 nNumerator, uint32 nDenominator) = 0;

	//called to get the time scale from the associated timer. If the timer is invalid, the default will
	//just be 0/1
	virtual LTRESULT		GetTimerTimeScale(HENGINETIMER hTimer, uint32& nNumerator, uint32& nDenominator) = 0;

	//called to set the range of times that a single update can be within. This will clamp the update time
	//to be within this range. This is specified in integer milliseconds.
	virtual LTRESULT		SetTimerUpdateRange(HENGINETIMER hTimer, uint32 nMinMS, uint32 nMaxMS) = 0;

	//called to get the range from the timer. If the timer is invalid, the values will default to
	//both be zero.
	virtual LTRESULT		GetTimerUpdateRange(HENGINETIMER hTimer, uint32& nMinMS, uint32& nMaxMS) = 0; 

	//called to get the amount of time that has elapsed for this update interval for this timer in seconds.
	//If the timer is invalid, it will default the value to 0
	virtual LTRESULT		GetTimerElapsedS(HENGINETIMER hTimer, float& fElapsedS) = 0;

	//called to get the amount of time that has elapsed for this update interval for this timer in milliseconds.
	//If the timer is invalid, it will default the value to 0
	virtual LTRESULT		GetTimerElapsedMS(HENGINETIMER hTimer, uint32& nElapsedMS) = 0;

	//called to get the length of time that this timer has been running in seconds.
	//If the timer is invalid, it will default the value to 0
	virtual LTRESULT		GetTimerAccumulatedS(HENGINETIMER hTimer, double& fAccumulatedS) = 0;

	//called to get the length of time that this timer has been running in milliseconds.
	//If the timer is invalid, it will default the value to 0
	virtual LTRESULT		GetTimerAccumulatedMS(HENGINETIMER hTimer, uint32& nAccumulatedMS) = 0;

	//Pause/Unpause the timer.
	virtual LTRESULT		PauseTimer(HENGINETIMER hTimer, bool bPause) = 0;

	//Get the current pause state of the timer.
	virtual LTRESULT		IsTimerPaused(HENGINETIMER hTimer, bool& bPaused) = 0;


};



#endif
