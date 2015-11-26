// ----------------------------------------------------------------------- //
//
// MODULE  : IGameWorldPacker.h
//
// PURPOSE : Declares the IGameWorldPacker interface.  This interface is
//           used to communicate with the game server from the world
//           packer.
//
// CREATED : 12/10/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __IGAMEWORLDPACKER_H__
#define __IGAMEWORLDPACKER_H__

class ILTOutConverter;

class IGameWorldPacker
{
public:
	enum eSeverity
	{
		eSeverity_Message,
		eSeverity_Warning,
		eSeverity_Error,
		eSeverity_DebugMessage,
	};
	typedef void (*PFNERRORCALLBACK)( eSeverity severity, const char* szError, ... );

	virtual ~IGameWorldPacker(){}

	virtual bool NavMeshProcess( /* IN */ const uint8* pPCRawData, /* OUT */ ILTOutConverter& Converter, const LTVector& vWorldOffset, PFNERRORCALLBACK pfnErrorCallback ) = 0;
};

// function prototype
typedef IGameWorldPacker* (*PFNGETIGAMEWORLDPACKER)();

#endif  // __IGAMEWORLDPACKER_H__
