//---------------------------------------------------------------------------------------------
// IPerformanceMonitor.h
//
// This header includes the definition for the IPerformanceMonitor interface which is the
// client system's means of communicating with the performance monitoring system, and also
// includes several utility functions and classes which simplify the usage of the performance
// monitoring system.
//---------------------------------------------------------------------------------------------

#ifndef __IPERFORMANCEMONITOR_H__
#define __IPERFORMANCEMONITOR_H__

#ifndef __LTBASEDEFS_H__
#	include "ltbasedefs.h"
#endif

//this define is used to control whether or not performance monitoring should be used. By
//setting this define in the client system, all performance monitoring calls will attempt
//to be replaced with as simple of functionality as possible so as not to influence performance,
//memory, or code size
#ifdef _FINAL
#define DISABLE_PERFORMANCE_MONITORING
#endif

//---------------------------------------------
// IPerformanceMonitor
//
// This interface is used to coordinate with and control the performance monitor system

//this type is used to reference a system that has been registered in order to ensure that
//the timing has as little cost as possible
class CSystemPerformanceInfo;
typedef CSystemPerformanceInfo*	HPERFMONSYSTEM;

class IPerformanceMonitor
{
public:

	IPerformanceMonitor()			{}
	virtual ~IPerformanceMonitor()	{}


	//-------------------------------
	// System registration and timing

	//this function should be used to register a system with the performance monitoring system. This
	//will add the system as long as a system with a matching name does not already exist. If it does
	//the call will fail and NULL will be returned. If the system has no parent, NULL or an empty string
	//can be provided for the parent string.
	virtual HPERFMONSYSTEM RegisterSystem(const char* pszSystemName, const char* pszSystemParent) = 0;

	//given a valid performance system handle, this will start timing that system. Note that it is expected
	//that the timing is done in a stack like manner, so if the timings are started in the order of
	//A, B, C, they need to be stopped in the order of C, B, A.
	virtual void StartTiming(HPERFMONSYSTEM hSystem) = 0;

	//given a valid performance system handle that has been added to the system, this will stop the timing.
	//Note that this must be done in reverse stack order to avoid corrupting other timing results.
	virtual void StopTiming(HPERFMONSYSTEM hSystem) = 0;

	//-------------------------------
	// Tracking system events

	//called to notify the tracking system that a frame event has encountered so that it should update
	//the fram histories and reset per frame values
	virtual void HandleFrameEvent() = 0;

	//called to clear all of the data associated with the tracking system
	virtual void ResetTimings() = 0;

	//-------------------------------
	// System enumeration

	//called to get the number of systems that are currently registered
	virtual uint32 GetNumSystems() = 0;

	//called to get the system name at the specified index, this can return NULL if the
	//provided index is out of range. This pointer is valid and constant until the unloading of this DLL.
	virtual const char* GetSystem(uint32 nSystem) = 0;

	//--------------------------------
	// Stats querying

	//given the name of a system, this will retrieve the information associated with the system. NULL
	//can be provided for any fields that are not needed. False will be returned if the system cannot
	//be found and in such a case none of the fields will be moidified. All timings are in double precision
	//milliseconds.
	virtual bool GetSystemFrameStats(const char* pszSystem, double* pFrameTime, double* pNestedTime, uint64* pEntryCount) = 0;
	virtual bool GetSystemAverageStats(const char* pszSystem, double* pFrameTime, double* pNestedTime, double* pEntryCount) = 0;
	virtual bool GetSystemMaxStats(const char* pszSystem, double* pFrameTime, double* pNestedTime, uint64* pEntryCount) = 0;
	virtual bool GetSystemTotalStats(const char* pszSystem, double* pTotalFrame, double* pTotalNested, uint64* pTotalEntry) = 0;

	//--------------------------------
	// Reporting functionality

	//called to set the output function that will be used for reporting output (note that this does not
	//include logging as that is written directly to the file. Only a single output function may be set
	//at once, and it is valid to pass in a NULL function or user data
	typedef void (*TDisplayFunction)(const char* pszString, void* pUser);
	virtual void SetDisplayFunction(TDisplayFunction pfnOutput, void* pUserData) = 0;

	//called to handle a report command. The first parameter indicates the number of strings, and the
	//second parameter is a listing of those strings. For example, to produce a log to a file, the
	//parameters could be 2, { "Log", "c:\SomeLog.csv" }.
	virtual void HandleReportCommand(uint32 nNumParams, const char* const* pszParams) = 0;

	//called to notify the report system that a frame event has been encountered so it should update
	//any tracking reports or logs that it has opened
	virtual void HandleReportFrameEvent() = 0;
};

#if !defined(DISABLE_PERFORMANCE_MONITORING)

	//this block provides the implementations of utility classes used to simplify interactions with the
	//performance monitor library

	//this function provides access to the IPerformanceMonitor interface. This can return NULL on failure,
	//and requires that performancemonitor.cpp be linked into the application in some way.
	IPerformanceMonitor*	GetPerformanceMonitor();

	//-------------------------------------
	// CTimedSystem
	//
	// This class is used to represent a system that uses timings. The constructor takes in the
	// system name and an optional parent (can be either NULL or empty), and will then register the
	// system with the performance library. Typically this should be used as a global variable.

	class CTimedSystem
	{
	public:
		CTimedSystem(const char* pszSystemName, const char* pszParent)
		{
			//register this system with the performance monitor
			IPerformanceMonitor* pIPerfMon = GetPerformanceMonitor();
			if(pIPerfMon)
				m_hPerfSystem = pIPerfMon->RegisterSystem(pszSystemName, pszParent);
		}

		~CTimedSystem()
		{
			//we can't free our systems, so just make sure our handle is invalid
			m_hPerfSystem = NULL;
		}

		//provide access to the performance monitor system. Note that this can be NULL
		HPERFMONSYSTEM GetSystem()			{ return m_hPerfSystem; }

	private:
		//the handle to our performance monitoring system
		HPERFMONSYSTEM	m_hPerfSystem;
	};

	//-------------------------------------
	// CTimedSystemBlock
	// 
	// This utility can be used just like a variable that will handle starting and stopping the
	// timer associated with a system. 

	class CTimedSystemBlock
	{
	public:

		//upon construction we need to start the timing of the system
		CTimedSystemBlock(CTimedSystem& System) :
			m_System(System)
		{
			if(m_System.GetSystem())
			{
				IPerformanceMonitor* pIPerfMon = GetPerformanceMonitor();
				if(pIPerfMon)
					pIPerfMon->StartTiming(m_System.GetSystem());
			}
		}

		//and upon destruction we need to stop the timing
		~CTimedSystemBlock()
		{
			if(m_System.GetSystem())
			{
				IPerformanceMonitor* pIPerfMon = GetPerformanceMonitor();
				if(pIPerfMon)
					pIPerfMon->StopTiming(m_System.GetSystem());
			}
		}

	private:

		//the system that we need to deactivate at the end
		CTimedSystem&	m_System;
	};

#else

	//this section contains stubbed out implementation of the above classes to prevent the performance
	//monitoring system from creating any overhead
	inline IPerformanceMonitor*	GetPerformanceMonitor()			{ return NULL; }

	class CTimedSystem
	{
	public:
		CTimedSystem(const char* pszSystemName, const char* pszParent)		{}
		~CTimedSystem()														{}
		HPERFMONSYSTEM GetSystem()											{ return NULL; }
	};

	class CTimedSystemBlock
	{
	public:
		CTimedSystemBlock(CTimedSystem& System)		{}
		~CTimedSystemBlock()						{}
	};

#endif


#endif
