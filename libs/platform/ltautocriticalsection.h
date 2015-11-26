// *********************************************************************** //
//
// MODULE  : ltautocriticalsection.h
//
// PURPOSE : Ease-of-use class for using critical sections on the stack.
//
// CREATED : 05/26/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTAUTOCRITICALSECTION_H__
#define __LTAUTOCRITICALSECTION_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTCRITICALSECTION_H__
#include "ltcriticalsection.h"
#endif


class CLTAutoCriticalSection
{
public:

	CLTAutoCriticalSection(CLTCriticalSection& criticalSection);
	~CLTAutoCriticalSection();

private:

	CLTCriticalSection& m_criticalSection;

	// prevent copy and assignment
	PREVENT_OBJECT_COPYING(CLTAutoCriticalSection);

};

inline CLTAutoCriticalSection::CLTAutoCriticalSection(CLTCriticalSection& criticalSection)
	: m_criticalSection(criticalSection)
{
	m_criticalSection.CLTCriticalSection::Enter();
}

inline CLTAutoCriticalSection::~CLTAutoCriticalSection()
{
	m_criticalSection.CLTCriticalSection::Leave();
}


#endif // __LTAUTOCRITICALSECTION_H__

