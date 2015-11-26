// *********************************************************************** //
//
// MODULE  : win32_ltcriticalsection.h
//
// PURPOSE : Win32 implementation class for critical sections.
//
// CREATED : 05/25/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __WIN32_LTCRITICALSECTION_H__
#define __WIN32_LTCRITICALSECTION_H__

#include <windows.h>
#include "iltcriticalsection.h"

class CWin32_LTCriticalSection : protected ILTCriticalSection
{
public:

	CWin32_LTCriticalSection();
	~CWin32_LTCriticalSection();

	// lock the critical section
	virtual void Enter();

	// unlock the critical section
	virtual void Leave();

private:

	// Win32 critical section object
	CRITICAL_SECTION m_criticalSection;
};

inline CWin32_LTCriticalSection::CWin32_LTCriticalSection()
{
	InitializeCriticalSection(&m_criticalSection);
}

inline CWin32_LTCriticalSection::~CWin32_LTCriticalSection()
{
	DeleteCriticalSection(&m_criticalSection);
}

inline void CWin32_LTCriticalSection::Enter()
{
	EnterCriticalSection(&m_criticalSection);
}

inline void CWin32_LTCriticalSection::Leave()
{
	LeaveCriticalSection(&m_criticalSection);
}

#endif // __WIN32_LTCRITICALSECTION_H__
