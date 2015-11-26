//////////////////////////////////////////////////////////////////////////////
// PunkBuster game interfaces.

#ifndef __IPUNKBUSTER_H__
#define __IPUNKBUSTER_H__

#if defined(PLATFORM_LINUX)
#include <sys/socket.h>
#elif defined(PLATFORM_WIN32)
#include <winsock.h>
#else 
#define SOCKET uint32
#endif

struct PBServerCallBacks {
	void   (*GetClientInfo) ( const int index, char *name, int namelen, char *guid, int guidlen, char *ip, int iplen  ) ;
	void   (*GetServerAddr) ( char* buffer, int buflen ) ;
	void   (*GetVersion) ( char* buffer, int buflen ) ;
	void   (*GetClientStats) ( const int index, char* buffer, int buflen ) ;
	void   (*GetMaxClients) ( int& clients ) ;
	void   (*DropClient) ( const int index , const char* reason ) ;
	void   (*GetSocket) ( SOCKET *sock ) ;
	void   (*SendGameMessage) (const int index, char *data, int datalen ) ;
	void   (*DisplayMessage) ( char *data, int datalen ) ;
	void   (*GetMapName) ( char* buffer, int buflen ) ;
	void   (*GetServerHostName) ( char* buffer, int buflen ) ;
	void   (*GetGameName) ( char* buffer, int buflen ) ;
	void   (*GetCvarValue) ( const char *var_name, char* buffer, int buflen ) ;
	void   (*RestartMap) () ;
	void   (*ChangeMap) ( const char *mapname ) ;
	void   (*HandleAdminCommand) ( const char *cmd ) ;

	PBServerCallBacks( )
	{
		memset( this, 0, sizeof( PBServerCallBacks ));
	}
} ;

struct PBClientCallBacks {
	void   (*GetVersion) ( char* buffer, int buflen ) ;
	void   (*GetServerAddr) ( char* buffer, int buflen ) ;
	void   (*GetSocket) ( SOCKET *sock ) ;
	void   (*SendGameMessage) ( char *data, int datalen ) ;
	void   (*DisplayMessage) ( char *data, int datalen, int skipnotify ) ;
	void   (*GetMapName) ( char* buffer, int buflen ) ;
	void   (*GetServerHostName) ( char* buffer, int buflen ) ;
	void   (*GetGameName) ( char* buffer, int buflen ) ;
	void   (*GetPlayerName) ( char* buffer, int buflen ) ;
	void   (*GetCvarValue) ( const char *var_name, char* buffer, int buflen ) ;

	PBClientCallBacks( )
	{
		memset( this, 0, sizeof( PBClientCallBacks ));
	}
} ;

class IPunkBusterServer
{
public:
	IPunkBusterServer() { };
	virtual ~IPunkBusterServer() {}

// Types
public:

	// Contains initialization information about the game.
	struct StartupInfo
	{

		PBServerCallBacks m_PBServerCallBacks ;

	};

// Functions
public:

	//////////////////////////////////////////////////////////////////////////////
	// Commands

	// Initialize the PunkBuster SDK
	virtual void Initialize() = 0;
	virtual void SetPointers() = 0;
	virtual bool IsEnabled() = 0;
	virtual bool IsEnabledRequested() = 0;
	virtual bool Enable() = 0;
	virtual bool Disable() = 0;
	virtual void ProcessEvents() = 0;
	virtual bool HandleNetMessage( uint8* pData, uint32 nDataLen, uint8 senderAddr[4], uint16 nPort) = 0;
	virtual void ProcessCmd( char *cmd ) = 0;
	virtual void ClearCallbacks() = 0;
	virtual char *IsClientBanned( char *fromAddr , int cl_pb , const char *cl_guid ) = 0;
	virtual void CaptureConsoleOutput( char *msg, int msglen ) = 0;

private:

	PREVENT_OBJECT_COPYING( IPunkBusterServer );
};


class IPunkBusterClient
{
public:

	IPunkBusterClient( ) { };
	virtual ~IPunkBusterClient() {}

// Types
public:

	// Contains initialization information about the game.
	struct StartupInfo
	{

		// Callback Functions for PunkBuster Integration Hooks
		PBClientCallBacks m_PBClientCallBacks ;

	};

// Functions
public:

	//////////////////////////////////////////////////////////////////////////////
	// Commands

	// Initialize the PunkBuster SDK
	virtual void Initialize() = 0;
	virtual void SetPointers() = 0;
	virtual bool IsEnabled() = 0;
	virtual bool IsEnabledRequested() = 0;
	virtual bool Enable() = 0;
	virtual bool Disable() = 0;
	virtual void ProcessEvents() = 0;
	virtual bool HandleNetMessage( uint8* pData, uint32 nDataLen, uint8 senderAddr[4], uint16 nPort) = 0;
	virtual void ClearCallbacks() = 0;
	virtual void ProcessCmd( char *cmd ) = 0;
	virtual void CaptureConsoleOutput( char *msg, int msglen ) = 0;

private:

	PREVENT_OBJECT_COPYING( IPunkBusterClient );

};


#endif //__IPUNKBUSTER_H__
