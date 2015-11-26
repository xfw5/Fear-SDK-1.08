// ----------------------------------------------------------------------- //
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
// ----------------------------------------------------------------------- //

#ifndef __ILTCLIENT_H__
#define __ILTCLIENT_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#include "ltbasetypes.h"

#ifndef __ILTCSBASE_H__
#include "iltcsbase.h"
#endif

#ifndef __ILTINFO_H__
#include "iltinfo.h"
#endif

#ifndef __ILTRESOURCEMGR_H__
#include "iltresourcemgr.h"
#endif

#include <vector>

/*!  Music info */

enum
{
    MUSIC_IMMEDIATE =         0,
    MUSIC_NEXTMEASURE =       1,
    MUSIC_NEXTSONG =          2,
    MUSIC_QUEUE =             3
};

class CSoundInstance;

class ILTLightAnim;
class ILTTexInterface;

class ILTClientPhysics;
class ILTModel;
class ILTCursor;
class ILTClientSoundMgr;
class ILTRenderer;
class ILTDrawPrim;  
class ILTFontManager;
class ILTWidgetManager;
class ILTCustomRender;
class ILTVideoTexture;
struct InitSoundInfo;
struct ObjectCreateStruct;
class ILTMessage_Read;
class ILTTextureString;
class ILTGraphMgr;
class ILTTextureMgr;
struct SPerformanceStats;

//The function prototype for a callback function that can be provided when querying for objects
//in a region of space. This callback will be called once for each object found within that region
//of space, provided with the object, and also the user data that is specified.
typedef void (*TQueryRegionObjectsCB)(HOBJECT hObj, void* pUserData);

//The function prototype for a callback function that can be provided when querying for polygons
//in a region of space. This callback will be called once for each polygon found within that region
//of space, provided with the polygon geometry, and also the user data that is specified. Returning
//false will stop the process.
typedef bool (*TQueryRegionPolysCB)(uint32 nNumVerts, const LTVector* pVerts, const LTVector& vNormal, void* pUserData);

/*!  
ILTClient interface. This is what the ClientShells and special
effect plugins use to generate the interface and client objects for
the game.  

Define a holder to get this interface like this:
\code
define_holder(ILTClient, your_var);
\endcode
*/

class ILTClient : public ILTCSBase
{
public:
    interface_version_derived(ILTClient, ILTCSBase, 5);
        
//@}

/*!
File management.
*/

/*!
\param  pFilename       The configuration file.
\param  nNumValues		The number of values to write out, 0 will write all of them
\param  pValues			The names of each value to be saved out. There should be nNumValues of them.
\return \b LT_ERROR if it can't open the file for writing.

Write configuration files. 

Used for: Misc.
*/
    LTRESULT (*WriteConfigFileFields)(const char *pFilename, uint32 nNumValues, const char** pValues);

/*!
\param  pFilename       The configuration file.
\return \b LT_ERROR if it can't open the file for writing.

Write configuration files. 

Used for: Misc.
*/
    LTRESULT (*WriteConfigFile)(const char *pFilename);


/*!
\param  pFilename       The configuration file.
\return \b LT_ERROR if the file doesn't exist or if it can't open the file for writing.

Read and parse a configuration file.

Used for: Misc.
*/
    LTRESULT (*ReadConfigFile)(const char *pFilename);


/*!  
Sound functions.  
*/

/*!
These PlaySound() functions allow the client shell to initiate and control 
sounds without going through the server.  These sounds will only be played 
on the client initiating them. These functions will fill \b m_hSound
with the handle to the client sound if you set the \b PLAYSOUND_GETHANDLE 
flag.  This will also force the sound not to be automatically deleted when 
the sound is done playing.  You must call KillSound(). You must get the 
handle in order to use the other sound functions like SetSoundPosition().
*/

/*!
\param hSound Handle to sound.

\param fTimer Current time of sound.

\return \b LT_FINISHED when sound is not playing (may be delayed due to
collisions), or \b LT_INVALIDPARAMS when \b hSound is not available;
otherwise, returns \b LT_OK.

Get current time in seconds of sound.
fTimer is set only if \b LT_OK is returned.

Used for: Audio.  
*/
    LTRESULT (*GetSoundTimer)(HLTSOUND hSound, LTFLOAT *fTimer);

/*!
\param hSound Handle to sound.

\param pSixteenBitData The data buffer if the buffer is 16 bits per
sample, or \b NULL otherwise.

\param pEightBitData The data buffer if the buffer is 8 bits per
sample, or \b NULL otherwise.

\param dwSamplesPerSecond Samples per second.

\param dwChannels Number of channels. (Samples alternate between each
channel.)

\return \b LT_ERROR when sound is in neither 8- nor 16-bit format, or
\b LT_INVALIDPARAMS if \b hSound is not available; otherwise, returns
\b LT_OK.

Get the sound data and information about the data.  This will force
the sound to be decompressed. fTimer is set only if \b LT_OK is returned.

Used for: Audio.  
*/
    LTRESULT (*GetSoundData)(HLTSOUND hSound,  
        int16 *&pSixteenBitData, int8 *&pEightBitData, 
        uint32 *dwSamplesPerSecond, uint32 *dwChannels);

/*!
\param hSound Handle to sound.

\param dwOffset Current offset into sound buffer (in buffer
increments: bytes if 8 bit, double bytes if 16 bit).

\param dwSize Size of sound buffer (in bytes).

\return \b LT_FINISHED when the sound is not playing
(may be delayed due to collisions, or done), or \b LT_INVALIDPARAMS when
\b hSound is not available; otherwise, returns \b LT_OK.
 
Get current offset into sound buffer of sound.
\b fTimer is set only if \b LT_OK is returned.

Used for: Audio.  
*/
    virtual LTRESULT GetSoundOffset(HLTSOUND hSound, 
        uint32 * dwOffset, uint32 * dwSize = NULL) = 0;

/*!
\param  pSoundHandle    Handle to client only sound.
\return 
\b TRUE     if the sound is either no longer playing, or not found
\b FALSE    otherwise

Check whether a sound is finished playing or whether the object it was
attached to has been removed.
 
Used for: Audio.  
*/
    bool (*IsDone)(HLTSOUND pSoundHandle);

/*!
\return Results of SetListener().

\see SetListener()

Used for: Audio.
*/
    void (*GetListener)(bool *bListenerInClient, LTVector *pPos, LTRotation *pRot);

/*!
Access to the ILTVideoTexture interface.

\return The ILTVideoTexture interface.

Used for: Client Management.
*/
    virtual ILTVideoTexture *GetVideoTexture() = 0;

/*!
Access to the ILTTextureMgr interface.

\return The ILTTextureMgr interface.

Used for: Client Management.
*/
    virtual ILTTextureMgr *GetTextureMgr() = 0;

/*!
Access to the ILTRenderer interface.

\return The ILTRenderer interface.

Used for: Client Management.
*/
    virtual ILTRenderer *GetRenderer() = 0;

/*!
Access to the ILTCustomRender interface.

\return The ILTCustomRender interface.

Used for: Client Management.
*/
    virtual ILTCustomRender *GetCustomRender() = 0;

/*!
Access to the ILTTextureString interface.

\return The ILTTextureString interface.

Used for: Client Management.
*/
    virtual ILTTextureString *GetTextureString() = 0;

/*!
Access to the ILTGraphMgr interface.

\return The ILTGraphMgr interface.

Used for: Client Management.
*/
	virtual ILTGraphMgr* GetGraphMgr() = 0;

	
/*!
Intersections.
*/

/*!
Given an AABB in space specified through the center of the AABB and the half extents in each
direction, this will find all the objects that intersect that region of space, and for each one
will call provided callback function with the object and the provided user data.

\return LT_OK if all parameters were valid, and no errors occured, LT_INVALIDPARAMS if no callback
		was specified.

Used for: Client Management.
*/
    virtual LTRESULT FindObjectsInBox(	const LTVector& vCenter, const LTVector& vHalfDims, 
										TQueryRegionObjectsCB pfnQueryCB, void* pUserData) = 0; 

/*!
This function will take a bounding sphere of space and will then run through the provided world 
model and find all of the geometry that is contained within that sphere. It will then perform any clipping
to clip planes provided if necessary, and return the results of the clipping operation through a
callback that the user provides.

\return LT_OK unless the parameters are invalid, in which case it will return LT_INVALIDPARAMS

Used for: Client Management
*/
	virtual LTRESULT FindPolysInSphere( HOBJECT hWorldModel, const LTVector& vSphereCenter,
										float fSphereRadius, uint32 nNumClipPlanes,
										const LTPlane* pClipPlanes,
										TQueryRegionPolysCB pfnPolyCB, void* pCBUser) = 0;


/*!
Access to the ILTDrawPrim interface.

\return The ILTDrawPrim interface.

Used for: Client Management.
*/
    virtual ILTDrawPrim *GetDrawPrim() = 0;

/*!
\return Pointer to the \b ILTCursor interface.
 
Accesses the ILTCursor interface.

Used for: Interface.
*/
    virtual ILTCursor *Cursor() = 0;

/*!
\defgroup ConnectionStuff
Connection stuff
*/

/*!
\ingroup ConnectionStuff
\param  mode    (return) The current game mode, specified as one of the \b STARTGAME_ defines.

\return LT_OK.

Find out what the current game mode is (set when we started the last
game with StartGame() or through the console).

\see StartGame()

Used for: Client Management.
*/
    LTRESULT (*GetGameMode)(int *mode);

/*!
\ingroup ConnectionStuff
\param  pRequest    \b StartGameRequest structure.

\return \b LT_NOTINITIALIZED when there is no driver to match the request, 
LT_ERROR when unable to sucessfully host or join a lobby session, 
LT_INVALIDPARAMS when the \b m_pNetSession pointer in the StartGameRequest is NULL, 
LT_SERVERERROR when there is an error loading the world (Server/host only); otherwise, returns LT_OK.

Try to start a game in the mode you specify.
The existing game will keep running until the new game successfully starts.
For instance, if there is an error starting the new world, it
will still leave the current one running and put up an error message.

Used for: Client Management.
*/
    LTRESULT (*StartGame)(StartGameRequest *pRequest);

/*!
\ingroup ConnectionStuff

\param pID (return) The local client ID.
\return \b LT_NOTCONNECTED when we're not on a server or haven't gotten a
client ID yet; otherwise, returns \b LT_OK.

Get the local client ID.

Used for: Client Management.  
*/
    LTRESULT (*GetLocalClientID)(uint32 *pID);

/*!
\ingroup ConnectionStuff

Disconnect from the server we're on (if any).

\note This will destroy any client-side objects you've created.

Used for: Client Management.
*/
    void (*Disconnect)();

/*!
\return \b true when connected to server; otherwise, returns false.

Determine whether this client is currently connected to a server.
(The function checks whether the ClientMgr's m_pCurShell is valid.)

Used for: Client Management.
*/
    bool (*IsConnected)();

/*!
\ingroup ConnectionStuff

Shut down the app (not right away, but after the current update).

Used for: Client Management.
*/
    void (*Shutdown)();

/*!  
\ingroup ConnectionStuff

\param pMsg The printf-styled format string to display.  This
parameter can be followed by the usual data parameters.
 
Shut down the app (not right away, but after the current update) and display a message.

Used for: Client Management.  
*/
    void (*ShutdownWithMessage)(const wchar_t *pMsg, ...);

//! \name Renderer management
//@{

/*!
\param  pMode   (return) The current render mode.
\return LT_OK if Successful, LT_ERROR on Failure.

Get the current render mode.

\see SetRenderMode()

Used for: Rendering.
*/
    LTRESULT (*GetRenderMode)(RMode *pMode);

/*!
\param pMode The requested render mode.

\return \b LT_KEPTSAMEMODE when the function cannot set the mode
requested but was able to restore the previous mode, or
\b LT_UNABLETORESTOREVIDEO if it cannot set the mode and cannot restore
the video mode (in which case it will give a message and shutdown
afterwards); otherwise, returns \b LT_OK.

Tells the renderer to switch to the requested render mode.

\note It may not have set the \em exact mode you requested.  You can
check with GetRenderMode() to see what the new mode is.

\see GetRenderMode()

Used for: Rendering.  
*/
    LTRESULT (*SetRenderMode)(RMode *pMode);

/*!
\param flags The flags to while performing the shutdown (a combination
of the \b RSHUTDOWN_ flags in ltcodes.h).  

\return LT_OK if Successful, LT_ERROR on Failure.

Shut down the renderer.  The renderer won't come back until you call 
SetRenderMode().

\see SetRenderMode()

Used for: Rendering.  
*/
    LTRESULT (*ShutdownRender)(uint32 flags);

/*!
\return LT_OK if Successful, LT_NOTIN3D if not in a 3D block, LT_ERROR on Failure.

Called to render the console to the current render target. 

Used for: Rendering.  
*/

	virtual LTRESULT RenderConsoleToRenderTarget() = 0;

/*!
\param	pAddr	Buffer to receive the address of the server
\param	pPort	uint16 to receive the port of the server
\return \bLT_INVALIDPARAMS if one of the parameters is invalid, \b LT_NOTCONNECTED if not connected to a server; otherwise, returns \b LT_OK

Used for: Networking.
*/
	virtual LTRESULT GetServerIPAddress(uint8 pAddr[4], uint16 *pPort) = 0;

/*!
\param  pStruct     Description of object.
\return a valid local client object handle on successful creation and addition of the
LTObject to the local client world, \b NULL on failure

Create a client-side object.

Used for: Object.
*/
    HLOCALOBJ (*CreateObject)(ObjectCreateStruct *pStruct);

/*!
\param  hObj    Object to process (parent object of attachments).
\return 
\b LT_INVALIDPARAMS     if the object handle is invalid or NULL
\b LT_OK                otherwise

Update the position and rotation of the attachments on the object.
Attachments are always automatically updated when the object is
rendered.

Used for: Object.  
*/
    virtual LTRESULT ProcessAttachments(HOBJECT hObj)=0;

/*!  
Camera functions.  
*/

/*!
\param hObj Camera to query.
\param pX (return) X FOV (range: \f$\frac{\pi}{100}\f$ to \f$ (99 \times \frac{\pi}{100}) \f$).
\param pY (return) Y FOV (range: \f$\frac{\pi}{100}\f$ to \f$ (99 \times \frac{\pi}{100}) \f$).

Get the FOV of a camera.  It defaults to (\f$\frac{\pi}{2}\f$, \f$\frac{\pi}{2}\f$).

\see SetCameraFOV()

Used for: Camera Control.
*/
    void (*GetCameraFOV)(HLOCALOBJ hObj, float *pX, float *pY);

/*!
\param  hObj        Camera to modify.
 
\param  pX          (return) X FOV (range: clamped to \f$\frac{\pi}{100}\f$ to \f$ (99 \times \frac{\pi}{100}) \f$).
\param  pY          (return) Y FOV (range: clamped to \f$\frac{\pi}{100}\f$ to \f$ (99 \times \frac{\pi}{100}) \f$).

Set the FOV of a camera.  It defaults to (\f$\frac{\pi}{2}\f$, \f$\frac{\pi}{2}\f$).

\see GetCameraFOV()

Used for: Camera Control.
*/
    void (*SetCameraFOV)(HLOCALOBJ hObj, float fovX, float fovY);

/*!
\param hObj				Camera to query.
\param  rCameraRect		The rectangle on the render target that the rendering will modify
						This is in normalized device coordinates, so each extent should	
						be in the range of [0..1]

Get the viewport rectangle defined for this camera.

\see SetCameraRect()

Used for: Camera Control.
*/
    void (*GetCameraRect)(HLOCALOBJ hObj, LTRect2f& rCameraRect);

/*!
\param  hObj            Camera to modify.
\param  rCameraRect		The rectangle on the render target that the rendering will modify
						This is in normalized device coordinates, so each extent should	
						be in the range of [0..1]

Set the rectangle of the camera on the render target. If
the rectangle extends over the screen boundaries, then it is clipped.

\see GetCameraRect()

Used for: Camera Control.  
*/
    void (*SetCameraRect)(HLOCALOBJ hObj, const LTRect2f& rCameraRect);

/*!
\param  pos             Center of AABB.
\param  dims            Dimensions of AABB.
\param  indices         Array that will be filled in with the indices of the blockers in the AABB.

This retrieves the indices of all particle blockers that are within
the AABB specified by pos and dims.

Used for: Special FX.
*/
	bool (*GetParticleBlockersInAABB)( const LTVector& pos, const LTVector& dims, uint32* pIndices, uint32& nNumIndices );

/*!
\param  index           Index of blocker to retrieve.
\param  normal          Normal portion of blocker plane equation.
\param  dist            Distance portion of blocker plane equation.
\param  numEdges        Number of edges in blocker poly.
\param  edgeNormals     Normals in XZ plane of each of the blocker edges.

\return \b false - index is out of range.
\return \b true - Successful.

This retrieves the information for a particle blocker polygon.  This form
gets the information needed for fast vertical particle intersection, which
is the polygon edge normals in the XZ plane.

Used for: Special FX.
*/
	bool (*GetParticleBlockerEdgesXZ)(uint32 index, LTPlane& blockerPlane, uint32& numEdges, LTPlane*& edgePlanes);

/*
\param	nID				ID of the sector.
\param	pSectorEnabled  Return value destination for the sectors enabled state.
\param	pPortalsEnabled Return value destination for the sectors portal enabled state.

\return \b LT_NOTFOUND - The provided sector was not found.
\return \b LT_NOTINWORLD - No world information is available.
\return \b LT_OK - Successful.

Get the current visibility state of a sector, which includes whether or not a sector can be
looked into and whether or not a sectors portals can be looked through.

Used for: Visibility control
*/
	virtual LTRESULT GetSectorVisibility(uint32 nID, bool *pSectorEnabled, bool *pPortalsEnabled) = 0;

/*!
\param	nID				ID of the occluder.
\param	bSectorEnabled  Desired state for the sector.
\param	bPortalsEnabled Desired state for the sectors portal visibility.


\return \b LT_NOTFOUND - The provided sector was not found.
\return \b LT_NOTINWORLD - No world information is available.
\return \b LT_ERROR - Unable to complete operation.
\return \b LT_OK - Successful.

Set the current visibility state of a sector, which includes whether or not a sector can be
looked into and whether or not a sectors portals can be looked through.

Used for: Visibility control
*/
	virtual LTRESULT SetSectorVisibility(uint32 nID, bool bPortalsEnabled) = 0;

/*!
\param	hRegion			The region to enable/disable all of the sectors within
\param	bEnable			Desired state for the sectors within that region

\return \b LT_INVALIDPARAMS - The region provided was invalid
\return \b LT_OK - Successful.

Set the visibility state of all sectors in a region, allowing turning on or off the sectors

Used for: Visibility control
*/
	virtual LTRESULT EnableRegionSectors(HSTREAMINGREGION hRegion, bool bEnable) = 0;

/*!  
Network startup/join/host functions.  
*/

/*!
\param  pDriver     Name of driver.  Use \b NULL for the default net driver. (currently unsupported)
\param  dwFlags     Flags (currently unsupported).
\return \b LT_OK (always).

Clear any running shells from the client manager; terminate any running network drivers;
start drivers "dplay2" and "internet".

Call this function before calling any other network functions.  

The parameters are ignored.

Used for: Networking.  
*/
    LTRESULT (*InitNetworking)(const char *pDriver, uint32 dwFlags);

/*!
\param socket (return) Reference to a SOCKET that will contain the UDP socket if one is found.
\return \b LT_NOTFOUND if there is not UDP socket; otherwise, returns \b LT_OK.
*/
	typedef unsigned int SOCKET;
	virtual LTRESULT GetUDPSocket(SOCKET& socket) = 0;


/*!
\param  pListHead   (return) List of \b NetServices.
\return \b LT_OK (always)

Get a list of net services (TCP/IP, modem, etc).  

You should call FreeServiceList() to deallocate the returned list when you are done with it.

\see FreeServiceList()

Used for: Networking.
*/
    LTRESULT (*GetServiceList)(NetService* &pListHead);

/*!
\param sDriver String containing name of driver.
\param ppLobbyLaunchData (return) Pointer to a pointer to whatever is going to contain the lobby data.

\return \b LT_INVALIDPARAMS if ppLobbyLaunchData is invalid, \b LT_ERROR if it tries to add a driver and fails or the call to the driver to fill in the info fails; otherwise returns \b LT_OK.

Get the lobby launch info if available.

Passing \b NULL for \b sDriver will cause the "dplay2" driver to be used.

\b ppLobbyLaunchData should point to the type of struct which the specified driver knows how to fill in with lobby data.
In the case of DPlay2, \b ppLobbyLaunchData should point to a \b DPLCONNECTION** (DPlay-defined) struct.

Used for: Networking.
*/
    LTRESULT (*GetLobbyLaunchInfo)(const char* sDriver, void** ppLobbyLaunchData);

/*!
\param pVersionInfo (return) \b LTVERSIONINFOEXT struct to be filled in by the function
\return \b LT_INVALIDPARAMS if \b pVersionInfo is \b NULL or if it's not the expected size (i.e., different versions of the struct); otherwise, returns \b LT_OK.

Get the extended version info.

Used for: Misc.
*/
    LTRESULT (*GetVersionInfoExt)(LTVERSIONINFOEXT* pVersionInfo);

/*!
\param bEnable Enable/Disable access to the console.
\return LT_OK on success.

Enable/Disable access to the console.

Used for: Misc.
*/
    LTRESULT (*SetConsoleEnable)(bool bEnable);

/*!  
Access to server console mirror variables.  
*/

/*!
\param  pName       Name of console variable.
\param  val         (return) Console variable value.
\return \b LT_NOTFOUND if the variable isn't found.

Get server console variable float value.

Used for: Misc.
*/
    virtual LTRESULT GetSConValueFloat(const char *pName, float &val)=0;

/*!
\param  pName       Name of console variable.
\param  valBuf      (return) Buffer to hold console variable string.
\param  bufLen      Length of buffer.
\return \b LT_NOTFOUND if the variable isn't found.

Get server console variable string value.

Used for: Misc.
*/
    virtual LTRESULT GetSConValueString(const char *pName, 
        char *valBuf, uint32 bufLen)=0;



/*!
Helpers.
*/

/*!
\return Number of seconds since the game was started.

Get the time since the game was started.  This accesses the GAME timer
on the server. Since this value comes from the server, it will be
intermittent, so only use it for things that must be synced with the
server. This timer will not update when there isn't a connection to
the server.
 
Used for: Misc.  
*/
    double (*GetGameTime)();

/*!
\return Server's real time from the last update.

Get the time since the game was started.  This accesses the real time
timer the server. Since this value comes from the server, it will be
intermittent, so only use it for things that must be synced with the
server. This timer will not update when there isn't a connection to
the server.

Used for: Misc.  
*/
	virtual LTDOUBLE GetServerRealTime() = 0;

/*!
\param  pMsg    The printf-styled format string to display.  This parameter can be followed 
                by the usual format parameters.

Used to output a TRACE message to the Debug Output window.  Newlines
must be explicitly used.

Used for: Misc.
*/
    void (*DebugOut)(const char *pMsg, ...);

/*!
\return Handle to client object (NULL if currently doesn't exist).

Get your client object.

Used for: Misc.
*/
    HLOCALOBJ (*GetClientObject)();

/*!
Messaging.
*/

/*!
New message functions.  These functions don't free the message, so
call LMessage::Release() after sending.

\param  pMsg         \b Message to send to the server
\param  flags       \b MESSAGE_ Flags defined in \b ltcodes.h
\return \b LT_INVALIDPARAMS if the message is invalid, or the server shell is invalid; otherwise, returns \b LT_OK.

Send a message to the server shell.  The server shell receives the message through the OnMessage() function.

\see ILTCommon::CreateMessage()
\see OnMessage()

Used for: Messaging.
*/
    virtual LTRESULT SendToServer(ILTMessage_Read *pMsg, uint32 flags)=0;

/*!
\param  pListHead   Head of list of \b NetServices to deallocate.
\return LT_OK (always)

Deallocates the list headed by the parameter \b pListHead.

Call this function when you are finished using the list returned by
GetServiceList().

\see GetServiceList()

Used for: Networking.
*/
    LTRESULT (*FreeServiceList)(NetService *pListHead);

/*!
\param  hNetService     Description of net service.
\return \b LT_NOTINITIALIZED if it can't find the service, or \b LT_ERROR if the service is found but can't be selected; otherwise, returns \b LT_OK.

Find and select the given service as the one to use.

Used for: Networking.
*/
    LTRESULT (*SelectService)(HNETSERVICE hNetService);

/*!
\param pListHead (return) List of \b NetSessions
\param pInfo A string containing IP addresses to check for sessions.
\return \b LT_NOTINITIALIZED if the NetMgr has no main driver (failed to call InitNetworking?) or there is no query socket, or \b LT_ERROR if it can't bind to port when starting the query process; otherwise, returns \b LT_OK.

Fills \b pListHead with enumerated sessions.  

\b pInfo is a list of particular IP addresses to check, separated by semicolons.
A record containing a single asterisk (*) means to check the local network
for any broadcast sessions.  A colon followed by a number designates a port to use.
If the first character of an address is a numeral (between '0' and '9') it is treated
as a numeric IP address; otherwise it's treated as a host name.

Internally, the TCP driver will use the StartQuery/UpdateQuery/EndQuery functions
to look for sessions.  It will keep looking for connections for 3.5 seconds
(constant \b QUERY_TIME), sending out queries every 1.0 seconds (constant \b QUERY_SEND_INTERVAL).

You should call FreeSessionList() to deallocate the returned list when you are done with it.

\see FreeSessionList()

Used for: Networking.
*/
    LTRESULT (*GetSessionList)(NetSession* &pListHead, const char *pInfo);

/*!
\param  pListHead       List of \b NetSessions to free.
\return LT_OK (always)

Deallocates the list headed by the parameter \b pListHead.

Call this function when you are finished using the list returned by
GetSessionList() or GetQueryResults().

\see GetSessionList()
\see GetQueryResults()

Used for: Networking.
*/
    LTRESULT (*FreeSessionList)(NetSession *pListHead);

    LTRESULT (*AddInternetDriver)();
    LTRESULT (*RemoveInternetDriver)();

/*!
\defgroup queryfns Query functions.
Alternate method of getting session lists.  These only work for services with
the \b NETSERVICE_TCPIP flag.  These functions return immediately so you can 
update a UI in the background without having to "freeze" the UI while it 
queries hosts.

(GetSessionList() uses these functions to do its work; it just doesn't return
control between updates.)
*/

/*!
\ingroup queryfns
\param pInfo A string containing IP addresses to check for sessions.
\return \b LT_NOTINITIALIZED if there is no main driver, or \b LT_ERROR if it can't bind to port; otherwise, returns \b LT_OK.

\b pInfo is formatted just as described for GetSessionList().

\see GetSessionList()

Used for: Networking.
*/
    virtual LTRESULT StartQuery(const char *pInfo)=0;


/*!
\ingroup queryfns
\return \b LT_NOTINITIALIZED if there is no main driver or there is no query socket; otherwise, returns \b LT_OK.

Search for sessions that match the query set up through a prior call to StartQuery().  

Call this as often as possible.

Results of the query are stored in the main driver connection.  Use GetQueryResults() to retrieve those results.

\see StartQuery()
\see GetQueryResults()

Used for: Networking.
*/
    virtual LTRESULT UpdateQuery()=0;

/*!
\ingroup queryfns

\param pListHead (return) List of \b NetSessions found through the query.
\return \b LT_NOTINITIALIZED if there is no main driver; otherwise, returns \b LT_OK.

Get the list of current results from the query.  

Each time you call this function, a new session list is allocated which must be
freed through a call to FreeSessionList() when you are done with it.

\see FreeSessionList()

Used for: Networking.  
*/
    virtual LTRESULT GetQueryResults(NetSession* &pListHead)=0;

/*!
\ingroup queryfns
\return \b LT_NOTINITIALIZED if there is no main driver; otherwise, returns \b LT_OK.

End the current query set up through a prior call to StartQuery().

\see StartQuery()

Used for: Networking.
*/
    virtual LTRESULT EndQuery()=0;

/*!
\param  sDriver     String containing name of driver.
\return \b LT_ERROR if it tried to add a driver and that failed, or to indicate that the session was not lobby launched; otherwise, return \b LT_OK (indicated a lobby launched session).

Determine if we were lobby-launched.

Passing \b NULL for \b sDriver will cause the "dplay2" driver to be used.

If a driver of the given name cannot be found, IsLobbyLaunched() will try to add it.

Used for: Networking.
*/
    LTRESULT (*IsLobbyLaunched)(const char* sDriver);

/*!
\param bLocal Results of the query
\return LT_OK on success

Returns whether or not the client is local to the server

Used for: Misc.
*/
	virtual LTRESULT IsLocalToServer(bool *bLocal) = 0;

/*!
\param socket (return) Reference to a SOCKET that will contain the UDP host socket if one is found (only valid for local clients).
\return \b LT_NOTFOUND if there is no host UDP socket; otherwise, returns \b LT_OK.

Returns the host UDP socket if the client is the local client in a hosted game.

Used for: Misc.
*/
	virtual LTRESULT GetHostUDPSocket(SOCKET& socket) = 0;

	
/*!  
Engine hooks.  
*/

/*!
\param pName Name of engine hook.

\param pData User data.

\return \b LT_OK when it understands the string; otherwise, returns an error.

This is here so we can avoid adding API functions if necessary and for
some system-dependent or misc. stuff.  Pass in a string describing
what you want and it fills in pData.

\par Hooks:
\li \b HWND Returns main window handle.

Used for: Misc.  
*/
    virtual LTRESULT GetEngineHook(char *pName, void **pData) = 0;


/*!
\param pStats	Destination for results
\param bQuick	Quick-test.  When set to true, will return only feature set details, skipping benchmarks

\return LT_OK on success

Returns performance statistics about the computer.  (CPU speed, GPU feature set, etc.)
See PerformanceStats.h for more information.

Used for: Misc.
*/
	virtual LTRESULT QueryPerformanceStats(SPerformanceStats *pStats, bool bQuick) = 0;


/*!
\param hObj   Object to retrieve the game data for

\param pData  Pointer to the game data that will be returned

\return \b LT_OK when the object and data pointer are non-null; otherwise, returns an LT_INVALIDPARAMS.

This function returns the game data value associated with the specified object

Used for: Objects.  
*/
	virtual LTRESULT GetObjectGameData(HOBJECT hObj, void **pData) = 0;

/*!
\param hObj   Object to set the game data on

\param pData  The game data that will be used

\return \b LT_OK when the object is non-null; otherwise, returns an LT_INVALIDPARAMS.

This function sets the game data value associated with the specified object

Used for: Objects.  
*/
	virtual LTRESULT SetObjectGameData(HOBJECT hObj, void *pData) = 0;

/*!
\return Whether running the MP exe.

Used for: Misc.
*/
virtual bool IsMultiplayerExe( ) = 0;
};

#endif  //! __ILTCLIENT_H__
