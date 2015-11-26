//------------------------------------------------------------------------------------------------
// iltgameutil.h
// 
// Provides interface to game utilties.  These utilities are game components
// that cannot exist in the game source.  The game code source is sometimes
// released to the public.  Some game code will need to be hidden so that 
// IP can be protected and prevent cheating in multiplayer.
//
//------------------------------------------------------------------------------------------------

#ifndef __ILTGAMEUTIL_H__
#define __ILTGAMEUTIL_H__

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif // __LTBASETYPES_H__

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif // __LTMODULE_H__

#ifndef __IGAMESPY_H__
#include "igamespy.h"
#endif // __IGAMESPY_H__

#ifndef __IPUNKBUSTER_H__
#include "ipunkbuster.h"
#endif // __IPUNKBUSTER_H__

class ILTMessage_Write;
class ILTMessage_Read;

class ILTGameUtil : public IBase
{
public:
    interface_version(ILTGameUtil, 0);

	// Creates a IGameSpyServer object.
	virtual IGameSpyServer* CreateGameSpyServer( IGameSpyServer::StartupInfo const& startupInfo ) = 0;

	// Destroys a IGameSpyServer object.
	virtual void DestroyGameSpyServer( IGameSpyServer* pGameSpyServer ) = 0;

#if !defined(PLATFORM_LINUX)
	// Creates a IGameSpyBrowser object.
	virtual IGameSpyBrowser* CreateGameSpyBrowser( IGameSpyBrowser::StartupInfo const& startupInfo ) = 0;

	// Destroys a IGameSpyBrowser object.
	virtual void DestroyGameSpyBrowser( IGameSpyBrowser* pGameSpyBrowser ) = 0;

	// Creates a IGameSpyPatchInfo object.
	virtual IGameSpyPatchInfo* CreateGameSpyPatchInfo( IGameSpyPatchInfo::StartupInfo const& startupInfo ) = 0;

	// Destroys a IGameSpyPatchInfo object.
	virtual void DestroyGameSpyPatchInfo( IGameSpyPatchInfo* pGameSpyPatchInfo ) = 0;
		
	// Checks if a cdkey is valid.
	virtual bool IsCDKeyValid( char const* pszCDKey ) = 0;

	// ----------------------------------------------------------------------- //
	// Function name   : CDKeyComputeChallengeResponse
	// Description     : Calculates the challenge response for the client.
	// Return type     : bool - true if message handled.
	// Argument        : char const* pszCDKey - cdkey to use.
	// Argument        : char const* pszChallenge - the challenge given by server.
	// Argument        : char* pszChallengeResponse - the response to send to server.
	// Argument        : uint32& nChallengeResponseLen - Length of response string.
	// ----------------------------------------------------------------------- //
	virtual bool CDKeyComputeChallengeResponse( char const* pszCDKey, char const* pszChallenge, 
		char* pszChallengeResponse, uint32& nChallengeResponseLen ) = 0;

#endif // PLATFORM_LINUX

	// Write/Read handshaking version number.
	virtual void WriteHandshakingVersion( ILTMessage_Write& msg, uint16 nHandshakingVersion, uint16 nHandshakingVersionMask ) = 0;
	virtual uint16 ReadHandshakingVersion( ILTMessage_Read& msg, uint16 nHandshakingVersionMask ) = 0;

	// Write/Read clientkey1.
	virtual void WriteClientKey1( ILTMessage_Write& msg, uint32 nClientKey1 ) = 0;
	virtual uint32 ReadClientKey1( ILTMessage_Read& msg ) = 0;

	// Write/Read serverkey.
	virtual void WriteServerKey( ILTMessage_Write& msg, uint32 nClientKey1, LTGUID const& guid, uint32 nServerKey ) = 0;
	virtual uint32 ReadServerKey( ILTMessage_Read& msg, uint32 nClientKey1, LTGUID const& guid ) = 0;

	// Write/Read CRC message.
	virtual bool WriteCRCMessage( ILTMessage_Write& msg, LTGUID const& guid, uint32 nServerKey, void* pGameClient, char const* pszDBFile ) = 0;
	virtual bool ReadCRCMessage( ILTMessage_Read& msg, LTGUID const& guid, uint32 nServerKey, char const* pszDBFile ) = 0;

	// Write/Read worldcrc.
	virtual void WriteServerKeyData( ILTMessage_Write& msg, LTGUID const& guid, uint32 nServerKey, uint32 nData ) = 0;
	virtual uint32 ReadServerKeyData( ILTMessage_Read& msg, LTGUID const& guid, uint32 nServerKey ) = 0;

	// Write/Read PunkBuster state
	virtual void WritePunkBusterState( ILTMessage_Write& msg, LTGUID const& guid, uint32 nServerKey, IPunkBusterClient* pPunkBusterClient) = 0;
	virtual bool ReadPunkBusterState( ILTMessage_Read& msg, LTGUID const& guid, uint32 nServerKey) = 0;

	// Creates a IPunkBusterServer object.
	virtual IPunkBusterServer* CreatePunkBusterServer( IPunkBusterServer::StartupInfo const& startupInfo ) = 0;

	// Destroys a IPunkBusterServer object.
	virtual void DestroyPunkBusterServer( IPunkBusterServer* pPunkBusterServer ) = 0;

	// Creates a IPunkBusterClient object.
	virtual IPunkBusterClient* CreatePunkBusterClient( IPunkBusterClient::StartupInfo const& startupInfo ) = 0;

	// Destroys a IPunkBusterClient object.
	virtual void DestroyPunkBusterClient( IPunkBusterClient* pPunkBusterClient ) = 0;

};

#endif  //! __ILTGAMEUTIL_H__
