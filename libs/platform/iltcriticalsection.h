// *********************************************************************** //
//
// MODULE  : iltcriticalsection.h
//
// PURPOSE : Interface class for critical sections.
//
// CREATED : 05/25/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTCRITICALSECTION_H__
#define __ILTCRITICALSECTION_H__

class ILTCriticalSection
{
public:

	// lock the critical section
	virtual void Enter() = 0;

	// unlock the critical section
	virtual void Leave() = 0;

};

#endif // __ILTCRITICALSECTION_H__
