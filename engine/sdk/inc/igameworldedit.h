// ----------------------------------------------------------------------- //
//
// MODULE  : igameworldedit.h
//
// PURPOSE : Declares the IGameWorldEdit interface.  This interface is
//           implemented by the game server to communicate with
//           WorldEdit.
//
// CREATED : 01/28/05
//
// (c) 2005 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __IGAMEWORLDEDIT_H__
#define __IGAMEWORLDEDIT_H__

class IGameWorldEdit
{
public:
	// returns the number of surface records in the database
	virtual uint32		GetSurfaceCount() = 0;
	// returns the name of the specified surface
	virtual bool		GetSurfaceName( uint32 nSurface, char* szSurface, uint32 nLength ) = 0;
	// returns the Id of the specified surface
	virtual bool		GetSurfaceId( uint32 nSurface, uint32& nId ) = 0;

	// validates the surface records and returns the first error encountered in the passed in string
	virtual bool		ValidateSurfaceRecords( char* szError, uint32 nLength ) = 0;
};

// function prototype
typedef IGameWorldEdit* (*PFNGETIGAMEWORLDEDIT)();


#endif  // __IGAMEWORLDEDIT_H__
