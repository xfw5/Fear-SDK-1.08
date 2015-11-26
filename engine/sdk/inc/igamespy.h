//////////////////////////////////////////////////////////////////////////////
// Gamespy game interfaces.

#ifndef __IGAMESPY_H__
#define __IGAMESPY_H__

#if defined(PLATFORM_LINUX)
#include <sys/socket.h>
#elif defined(PLATFORM_WIN32)
#include <winsock.h>
#else 
#define SOCKET uint32
#endif

#include <string>

class IGameSpyServer
{
public:
	IGameSpyServer( ) { };
	virtual ~IGameSpyServer() {}

// Types
public:

	// Contains initialization information about the game.
	struct StartupInfo
	{
		StartupInfo( )
		{
			m_nPort = 0;
			m_Socket = (SOCKET)0;
			m_bPublic = true;
		}

		// IP Address to bind to.
		std::string		m_sIpAddress;
		// Port to bind to.
		uint16			m_nPort;
		// Socket to do comm on.  NULL if not to do shared socket.
		SOCKET			m_Socket;
		// Version of the game running.
		std::string		m_sVersion;
		// Public game.
		bool			m_bPublic;
	};

	// Status of published stated.
	enum EPublishStatus
	{
		ePublishStatus_Unpublished,
		ePublishStatus_Published,
		ePublishStatus_Error,
	};

	// Status of the cdkey challenge response.
	enum EChallengeResponse
	{
		eChallengeResponse_Invalid,
		eChallengeResponse_Processing,
		eChallengeResponse_Accepted,
		eChallengeResponse_Accepted_Timeout,
		eChallengeResponse_Rejected,
	};

// Functions
public:

	//////////////////////////////////////////////////////////////////////////////
	// Commands

	// Publishes the server to the Master Server.
	virtual bool Publish( ) = 0;
	// Removes server from Master Server.
	virtual bool Unpublish( ) = 0;
	// Check if we're published.
	virtual EPublishStatus GetPublishStatus( ) const = 0;

	//////////////////////////////////////////////////////////////////////////////
	// Server information.

	// Registers key with Master Server.  pszKey must NOT include player
	// or team index. (e.g. "player_", "team_t").
	// Keys must be registered in the same order on client and 
	// server.
	virtual bool RegisterKey( char const* pszKey ) = 0;

	// Get server property value.  Key string must include player/team index if applicable
	// (e.g. "player_0", "team_t0" ).
	virtual bool GetServerProperty( char const* pszKey, char* pszValue, uint32 nValueSize ) const = 0;
	// Sets server property value.  Key string must include player/team index if applicable
	// (e.g. "player_0", "team_t0" ).
	virtual bool SetServerProperty( char const* pszKey, char const* pszValue ) = 0;

	// Sets the number of players currently connected.
	virtual void SetNumPlayers( uint32 nNumPlayers ) = 0;
	// Sets the number of teams currently in game.
	virtual void SetNumTeams( uint32 nNumTeams ) = 0;

	// Get our public IP and Port.  Returns false if not available yet.
	virtual bool GetPublicIPandPort( char* pszPublicIP, uint32 nPublicIPSize, uint16& nPublicPort ) = 0;

	//////////////////////////////////////////////////////////////////////////////
	// CDKey Functions

	// Get the cdkeychallenge
	virtual bool GetCDKeyChallenge( char* szChallenge, uint32& nChallengeSize ) = 0;

	// Called when challenge response is received from client.
	virtual bool ProcessCDKeyChallengeResponse( uint32 nClientId, char const* pszIPAddr, 
		char const* pszChallenge, char const* szChallengeResponse1, char const* szChallengeResponse2 ) = 0;

	// Poll to see if client's challenge response was accepted.
	virtual bool GetChallengeResponse( uint32 nClientId, IGameSpyServer::EChallengeResponse& eChallengeResponse1,
		IGameSpyServer::EChallengeResponse& eChallengeResponse2 ) const = 0;

	// Tells the cdkey server to release any lock on the cdkey.  Called when client disconnects.
	virtual bool ReleaseCDKey( uint32 nClientId ) = 0;

	// Retrieves the user's unique CDKey hash
	virtual bool GetUserCDKeyHash( uint32 nClientId, const char*& pszUserCDKeyHash ) = 0;

	//////////////////////////////////////////////////////////////////////////////
	// Net message handling

	// Handle an incoming net message
	// Returns false if the message was not recognized
	// Note : Remove all external headers before calling this function
	virtual bool HandleNetMessage( uint8* pData, uint32 nDataLen, uint8 senderAddr[4], uint16 nPort) = 0;

private:

	PREVENT_OBJECT_COPYING( IGameSpyServer );
};


class IGameSpyBrowser
{
public:

	IGameSpyBrowser( ) { };
	virtual ~IGameSpyBrowser() {}

// Types
public:

	// Type used to identify a game server in the ServerInfoCallback.  Used to 
	// call back into the gamespybrowser to retrieve information.
	typedef void* HGAMESERVER;

	enum EServerInfoCallbackReason
	{
		eServerinfoCallbackReason_Keys,
		eServerinfoCallbackReason_Ping,
	};

	// Callback to register to receive server information.
	// hServer is temporary and should not be held onto.
	// bDetailsRequest is true if this is in response to a request for server details.
	// pUserData is the user data value passed into StartupInfo.
	// nPing will be kPingError on error.
	typedef void ( *ServerInfoCallback )( IGameSpyBrowser& gameSpyBrowser, 
		HGAMESERVER hServer, void* pUserData, 
		EServerInfoCallbackReason eReason, uint16 nPing );

	// Contains initialization information about the game.
	struct StartupInfo
	{
	};

	// Status of browsing state.
	enum EBrowserStatus
	{
		eBrowserStatus_Idle,
		eBrowserStatus_Processing,
		eBrowserStatus_Complete,
		eBrowserStatus_Error,
	};

	enum ENatNegotiationResult
	{
		eNatNegotiationResult_Invalid,
		eNatNegotiationResult_Error,
		eNatNegotiationResult_TimedOut,
		eNatNegotiationResult_Success,
	};

	enum
	{
		kPingError = ( uint16 )-1,
	};

	typedef void (*RequestURLDataCallback)
		(
		const char*	 pBuffer,		// pointer to the data
		const uint32 nBufferLen,		// length of the data
		void*	     pCallbackParam	// parameter that was passed in the call to RequestURLData()
		);

// Functions
public:

	//////////////////////////////////////////////////////////////////////////////
	// Commands

	// Sets the callback information for server info requests.
	virtual void SetServerInfoCB( ServerInfoCallback cbServerInfo, void* pUserData ) = 0;

	// Requests list of servers from Master Server. pszFilter uses SQL like 
	// notation (e.g. pszFilter = "fraglimit > 0 && numplayers > 0".
	// Complete when GetBrowsingStatus returns EBrowserStatus_Complete.
	// Callback is called for each server found.
	// pUserData is sent as parameter in the callback call.
	// nPort is the LAN ports to scan.
	virtual bool RequestServerList( bool bPublic, uint16 nPort, char const* pszFilter ) = 0;
	// Requests detail information on specific server.
	// Complete when GetBrowsingStatus returns EBrowserStatus_Complete.
	// Callback is called for each server found.
	// pUserData is sent as parameter in the callback call.
	virtual bool RequestServerDetails( char const* pszAddress, uint16 nPort, bool bDirectConnection, bool bFullDetails ) = 0;
	// Requests NAT negotiations with server pszAddress:nPort on socket hSocket.
	virtual bool RequestNatNegotiation( SOCKET hSocket, char const* pszAddress, uint16 nPort ) = 0;
	// Stops all requests.
	virtual bool HaltRequest( ) = 0;
	// Gets the current status of the browser.
	virtual EBrowserStatus GetBrowserStatus( ) const = 0;
	// Gets the results of a NatNeg request.
	virtual ENatNegotiationResult GetNatNegotiationResult( sockaddr* pSockAddr ) const = 0;
	// Requests retrieval data from of a URL, which is returned in the supplied buffer.
	virtual bool RequestURLData(const char* pszURL, RequestURLDataCallback cbRequestURLData, void* pCallbackParam) = 0;

	//////////////////////////////////////////////////////////////////////////////
	// Key management

	// Registers key with Master Server.  pszKey must NOT include player
	// or team index. (e.g. "player_", "team_t").
	// Keys must be registered in the same order on client and 
	// server.
	virtual bool RegisterKey( char const* pszKey ) = 0;

	// Adds a Summary key that should be reported by a server when
	// RequestServerList called.  Keep the number of summary
	// keys low for optimal performance.
	virtual bool AddSummaryKey( char const* pszKey ) = 0;

	//////////////////////////////////////////////////////////////////////////////
	// Server information.

	// Gets the number of servers still coming down for a query.
	// Returns kError on error.
	virtual uint32 GetNumPendingServers( ) = 0;

	// Gets the number of players currently on server.
	// Returns kError on error.
	virtual uint32 GetNumPlayers( HGAMESERVER hGameServer ) const = 0;
	// Gets the number of teams currently on server.
	// Returns kError on error.
	virtual uint32 GetNumTeams( HGAMESERVER hGameServer ) const = 0;

	// Get server property value.  Key string must include player/team index if applicable
	// (e.g. "player_0", "team_t0" ).
	virtual bool GetServerProperty( HGAMESERVER hGameServer, char const* pszKey, char* pszValue, uint32 nValueSize ) const = 0;

	// Get the ping to a server.
	// Returns kPingError on error.
	virtual uint16 GetServerPing( HGAMESERVER hGameServer ) const = 0;

	// Request a ping for a server.
	virtual bool RequestServerICMPPing( HGAMESERVER hGameServer ) = 0;

	// Check if server has detail info.
	virtual bool HasServerDetails( HGAMESERVER hGameServer ) const = 0;

	// Result struct passed to GetServerConnectionInfo
	struct ServerConnectionInfo
	{
		char szPrivateAddress[16];
		uint16 nPrivatePort;
        char szPublicAddress[16];
		uint16 nPublicPort;
		bool bConnectViaPublic;
		bool bDirectConnect;
	};

	// Gets the address/port to use when connecting.
	virtual bool GetServerConnectInfo( HGAMESERVER hGameServer, ServerConnectionInfo& serverConnectionInfo ) const = 0;

	//////////////////////////////////////////////////////////////////////////////
	// Net message handling

	// Handle an incoming net message
	// Returns false if the message was not recognized
	// Note : Remove all external headers before calling this function
	virtual bool HandleNetMessage( uint8* pData, uint32 nDataLen, uint8 senderAddr[4], uint16 nPort) = 0;

private:

	PREVENT_OBJECT_COPYING( IGameSpyBrowser );
};



class IGameSpyPatchInfo
{
public:

	IGameSpyPatchInfo( ) { };
	virtual ~IGameSpyPatchInfo() {}

// Types
public:

	// Status of patchinfo state.
	enum EPatchInfoStatus
	{
		eBrowserStatus_Idle,
		eBrowserStatus_Processing,
		eBrowserStatus_Complete,
		eBrowserStatus_Error,
	};

	struct PatchInfoResults
	{
		bool m_bNewVersionAvailable;
		bool m_bMandatoryPatch;
		std::string m_sNewVersion;
		std::string m_sURL;
	};

	// Contains initialization information about the game.
	struct StartupInfo
	{
		StartupInfo( )
		{
			m_nPatchInfoDistributionId = 0;
		}

		// Version of the game running.
		std::string		m_sVersion;
		// Distribution ID.
		uint32 m_nPatchInfoDistributionId;
	};

// Functions
public:

	// Requests patch information from GameSpy.
	virtual bool RequestPatchInfo( ) = 0;
	// Stops request.
	virtual bool HaltRequest( ) = 0;
	// Gets the current status of the patchinfo request.
	virtual EPatchInfoStatus GetPatchInfoStatus( ) const = 0;
	// Gets the results.
	virtual bool GetPatchInfoResults( PatchInfoResults& patchInfoResults ) const = 0;

private:

	PREVENT_OBJECT_COPYING( IGameSpyPatchInfo );
};

#endif //__IGAMESPY_H__
