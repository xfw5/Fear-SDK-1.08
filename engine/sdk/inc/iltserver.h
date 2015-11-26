/*!
server_de defines the ILTServer interface, which consists of all the
server-side DirectEngine functionality.
*/

#ifndef __ILTSERVER_H__
#define __ILTSERVER_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTSERVEROBJ_H__
#include "ltserverobj.h"
#endif

#ifndef __ILTCSBASE_H__
#include "iltcsbase.h"
#endif


DEFINE_HANDLE_TYPE(HCLIENTREF);

/*!  
Load world flags.  
*/
enum
{
	//!  Load objects from the world file?
	LOADWORLD_LOADWORLDOBJECTS	= (1<<0),
	//! Start running world immediately?
	LOADWORLD_RUNWORLD			= (1<<1),
	/*!  
	Don't reload geometry if the world filename is the same as the
	currently loaded one.  
	*/
	LOADWORLD_NORELOADGEOMETRY	= (1<<2),
};

/*!  
Save objects flags.  
*/
enum
{
    SAVEOBJECTS_SAVEGAMECONSOLE		= (1<<0),   //! Save the console state?
	SAVEOBJECTS_SAVEGAMESTATE		= (1<<1),   //! Save the game state such as timing and physics?
};


/*!
Client info flags.

\b CIF_LOCAL and \b CIF_PLAYBACK are not settable with SetClientInfoFlags().
*/

enum
{
//! Client is local (non-network) connection.
    CIF_LOCAL                 = (1<<0),
//! Virtual client from a demo playback.
    CIF_PLAYBACK              = (1<<1),
//! Client object is sent with full position resolution.
    CIF_FULLRES               = (1<<2),
//! Client object rotations are sent to client.
    CIF_SENDCOBJROTATION      = (1<<3)
};

/*!
Object states.

\b OBJSTATE_INACTIVE and \b OBJSTATE_INACTIVE_TOUCH override the effect of
PingObjects().  \b OBJSTATE_ACTIVE clears the effect of \b OBJSTATE_INACTIVE
or \b OBJSTATE_INACTIVE_TOUCH, but the object can be auto-deactivated.
*/

enum
{
//! Normal healthy object.
    OBJSTATE_ACTIVE         = 0,

//! Inactive (no updates, physics, or touch notifications).
    OBJSTATE_INACTIVE       = 1,

//! Inactive, but receives touch notifications
    OBJSTATE_INACTIVE_TOUCH = 2,
};



/*!  
Object lists.  
*/

struct ObjectLink
{
    HOBJECT     m_hObject;
    ObjectLink  *m_pNext;
};

struct ObjectList
{
    ObjectLink  *m_pFirstLink;
    int     m_nInList;
};


#define MAX_CLASSPROPINFONAME_LEN   128

/*!  
Used by ILTServer::GetClassProp.  
*/

class ClassPropInfo
{
public:
    char    m_PropName[MAX_CLASSPROPINFONAME_LEN];
    short       m_PropType;     //! PT_ define from serverobj_de.h.
};

class ILTPhysics;
class ILTModel;
class ILTLightAnim;
class ILTSoundMgr;
struct ObjectCreateStruct;

/*!  
ILTServer interface. This is the interface to most server-side engine
functionality.  
*/

class ILTServer : public ILTCSBase {
public:
    interface_version_derived(ILTServer, ILTCSBase, 5);
    
    

/*!  
World control.  
*/

#ifndef DOXYGEN_SHOULD_SKIP_THIS
 
		virtual LTRESULT	GetWorldBox(LTVector &min, LTVector &max)=0;
		ObjectList*	(*GetPointAreas)(const LTVector *pPoint);
		ObjectList*	(*GetBoxIntersecters)(const LTVector *pMin, const LTVector *pMax);
		virtual LTRESULT FindWorldModelObjectIntersections(
			HOBJECT hWorldModel,
			const LTVector &vNewPos, const LTRotation &rNewRot,
			BaseObjArray<HOBJECT> &objArray)=0;
		ObjectList* (*FindObjectsTouchingSphere)(const LTVector *pPosition,float radius);
		virtual LTRESULT GetObjectUnguaranteedData(HLOCALOBJ hObj, uint32 *pData) = 0;

		LTRESULT	(*RotateObject)(HOBJECT hObj, const LTRotation& Rotation);
		LTRESULT	(*GetLastCollision)(CollisionInfo *pInfo);

#endif//doxygen

/*!  
New message functions. The main difference between these and the
functions above is that these don't free the message, so you
can send it multiple times.  
*/

/*!
\param  pMsg     \b Message to send to the object.
\param  hSender     Source object (can be \b NULL).
\param  hSendTo     Destination object.
\param  flags       Message flags.
\return \b LT_INVALIDPARAMS if the message is invalid; otherwise, returns \b LT_OK.

\see ObjectMessageFn()
\see ILTCommon::CreateMessage()
\see ILTMessage_Write::Read()

Send a message to an object. The object will receive the message through its
ObjectMessageFn function.

Used for: Messaging.  
*/
    virtual LTRESULT SendToObject(ILTMessage_Read *pMsg,
        HOBJECT hSender, HOBJECT hSendTo, uint32 flags)=0;

/*!
\param  pMsg     \b Message to send to the server.
\param  hSender     Source object (cannot be \b NULL).
\param  flags       Message flags.
\return \b LT_INVALIDPARAMS if the message is invalid, or the server shell is invalid; otherwise, returns \b LT_OK.

\see OnObjectMessage()
\see ILTCommon::CreateMessage()
\see ILTMessage_Write::Read()

Send the message to the server shell. The server shell will receive the message
through its OnObjectMessage() function.

Used for: Messaging.  
*/
    virtual LTRESULT SendToServer(ILTMessage_Read *pMsg,
        HOBJECT hSender, uint32 flags)=0;

/*!
\param  hObject     Object to modify.
\param  pMsg     \b Message to associate with this object.
\return \b LT_INVALIDPARAMS if the message is invalid; otherwise, returns \b LT_OK.

Sets up a special effect message.  If your object has a
special effect message, the SpecialEffectNotify() of the client shell
will be called.

\note An object can have only \b one associated special effect
message.  If the object already has a special effect message, then
calling this function will replace it.

\see SpecialEffectNotify()
\see ILTCommon::CreateMessage()
\see ILTMessage_Write::Read()

Used for: Messaging.  
*/
    virtual LTRESULT SetObjectSFXMessage(HOBJECT hObject,
        ILTMessage_Read *pMsg)=0;

/*!
\param  hObject     Object to modify.
\param  pMsg     \b Message associated with this object.
\return \b LT_INVALIDPARAMS if the message is invalid; otherwise, returns \b LT_OK.

Gets a special effect message.  

\see SpecialEffectNotify()
\see ILTCommon::CreateMessage()
\see ILTMessage_Write::Read()

Used for: Messaging.  
*/
	virtual LTRESULT GetObjectSFXMessage( HOBJECT hObject, ILTMessage_Write *pMsg ) = 0;

/*!
\param  pMsg     \b Message to send to the client.
\param  hSendTo     Destination client.  Use \b NULL to send to all clients.
\param  flags       Message flags.
\return \b LT_INVALIDPARAMS if the message is invalid; otherwise, returns \b LT_OK.

\see OnMessage()
\see ILTCommon::CreateMessage()
\see ILTMessage_Write::Read()

Send the message to the client.  The client shell will receive the message
through its OnMessage() function.

Used for: Messaging.  
*/
    virtual LTRESULT SendToClient(ILTMessage_Read *pMsg,
        HCLIENT hSendTo, uint32 flags)=0;

/*!
\param  pMsg     \b SFX Message to send
\param  flags       Message flags.
\return \b LT_INVALIDPARAMS if the message is invalid; otherwise, returns \b LT_OK.

\see SpecialEffectNotify()
\see ILTCommon::CreateMessage()
\see ILTMessage_Write::Read()

Send an "instant" special effect message to all the clients. The clients will receive 
the message through the IClientShell::SpecialEffectNotify() function.

Used for: Messaging.  
*/
    virtual LTRESULT SendSFXMessage(ILTMessage_Read *pMsg, uint32 flags)=0;

/*!  
Client Functions.  
*/

/*!
\param hPrev (input/return) The client to start from.  Use \b NULL to
get the first client.

\return Handle to the next client after \b hPrev.  \b NULL when there are
no more.

Get next client.

Used for: Server Management.  
*/
    HCLIENT (*GetNextClient)(HCLIENT hPrev);

/*!
\param hRef (input/return) The client reference to start from.  Use
\b NULL to get the first client reference.

\return Handle to the next client reference after \b hRef, or \b NULL when
there are no more.

Get next saved client reference. (These are created when a world is
saved.)

Used for: Server Management.  
*/
    HCLIENTREF (*GetNextClientRef)(HCLIENTREF hRef);

/*!
\param hObj Object to modify.

\param nextUpdate Next update time.

This is a counter that controls when each object has its Update()
function called. It is set to 0.0001 when an object is created, so
Update() is called right away by default.

Used for: Object.
*/
    void (*SetNextUpdate)(HOBJECT hObj, LTFLOAT nextUpdate);

/*!
\param hObj The object to query.

\param flags (return) Flags (a combination of \b NETFLAG_ defines).

\return \b LT_INVALIDPARAMS if hObj is not found, else \b LT_OK

Get the net flags of an object.

Used for: Object.
*/
    virtual LTRESULT GetNetFlags(HOBJECT hObj, uint32 &flags)=0;

/*!
\param hObj The object to modify.

\param flags Flags (a combination of \b NETFLAG_ defines).

\return \b LT_INVALIDPARAMS if hObj is not found, else \b LT_OK

Set the net flags of an object.

Used for: Object.
*/
    virtual LTRESULT SetNetFlags(HOBJECT hObj, uint32 flags)=0;

/*!
\param hClient Client to query.

\return Client reference info flags.

\b HCLIENTREF objects are used for comparison to \b HCLIENT objects.

Used for: Server Management.  
*/
    uint32 (*GetClientRefInfoFlags)(HCLIENTREF hClient);

/*!
\param hClient Client to query.

\param pName (return) Buffer to hold client reference name.

\param maxLen Buffer length.

\return false if the client ref is invalid. Otherwise, true.

An \b HCLIENTREF is used for comparison to an \b HCLIENT.

Used for: Server Management.  
*/
    bool (*GetClientRefName)(HCLIENTREF hClient, char *pName,
        int maxLen);

/*!
\param hClient Client to query.

\return Handle to client reference object.

An \b HCLIENTREF is used for comparison to an \b HCLIENT.

Used for: Server Management.  
*/
    HOBJECT (*GetClientRefObject)(HCLIENTREF hClient);

/*!
\param clientID Client to query.

\return Client handle.

Get the handle of the client identified by clientID.  GetClientHandle() will return NULL if clientID is not valid.

Used for: Server Management.  
*/
    HCLIENT (*GetClientHandle)(uint32 clientID);

/*!
\param hClient Client to query.

\return Client ID.

Get the unique ID of a client.

Used for: Server Management.  
*/
    uint32 (*GetClientID)(HCLIENT hClient);

/*!
\param hClient Client to query.

\return HOBJECT associated with client.

Get the object associated with client.  Can be NULL.

Used for: Server Management.  
*/
	virtual HOBJECT GetClientObject(HCLIENT hClient) = 0;

/*!
\param hClient Client to modify.
\param hObject Object to use.

\return LT_OK on success.  LT_ERROR on error.

Sets the object to associate with a client.  Can be null.

Used for: Server Management.  
*/
	virtual LTRESULT SetClientObject(HCLIENT hClient, HOBJECT hObject) = 0;

/*!
\param hClient Client to query.

\param ping (return) Ping time.

\return LT_NOTINITIALIZED if client is not connected. Otherwise, LT_OK.

Get the ping time of a client.

Used for: Server Management.  
*/
    virtual LTRESULT GetClientPing(HCLIENT hClient, float &ping)=0;

/*!
\param hClient Client to query.

\param nBandwidth (return) Current bandwidth setting.

\return LT_NOTINITIALIZED if client is not connected. Otherwise, LT_OK.

Get the bandwidth setting for a client.

Used for: Server Management.  
*/
	virtual bool GetClientBandwidth(HCLIENT hClient, uint32 &nBandwidth)=0;

/*!
\param hClient Client to query.

\param nBandwidth New bandwidth setting for this client

\return LT_NOTINITIALIZED if client is not connected. Otherwise, LT_OK.

Set the bandwidth setting for a client.

Used for: Server Management.  
*/
	virtual bool SetClientBandwidth(HCLIENT hClient, const uint32 nBandwidth)=0;

/*!
\param hClient Client to query.

\param pName (return) Buffer to hold client name.

\param maxLen Buffer length.

\return \b true if successful; otherwise, returns \b false.

Get the name of a client.

Used for: Server Management.  
*/
    bool (*GetClientName)(HCLIENT hClient, char *pName, int maxLen);

/*!
\param hClient Client to query.

\param pName Buffer holding desired name

\param maxLen Buffer length.

\return \b true if successful; otherwise, returns \b false.

Set the name of a client.

Used for: Server Management.  
*/
		bool	(*SetClientName)(HCLIENT hClient, const char *pName, int MaxLen);

/*!
\param hClient Client to query.

\param pData (out) Buffer to hold client data.  Pass NULL for length returned in maxLen.

\param maxLen (in,out) Buffer length.  Sets bytes copied.

\return \b true if successful; otherwise, returns \b false.

Get the name of a client.

Used for: Server Management.  
*/
    virtual bool GetClientData(HCLIENT hClient, uint8* pData, int& maxLen) = 0;

/*!
\param hClient Client to query.

\param pData Buffer holding desired data

\param len Buffer length.

\return \b true if successful; otherwise, returns \b false.

Set the data of a client.

Used for: Server Management.  
*/
	virtual bool SetClientData(HCLIENT hClient, uint8 const* pData, int len ) = 0;

/*!
\param hClient Client to query

\param pAddr Array for holding the client's address

\param pPort uint16 for holding the client's port

\return \b LT_ERROR if an invalid parameter is provided; otherwise, returns \b LT_OK

Retrieve the TCP/IP communications address and port for the given client

Note: The local client will return 0.0.0.0:0

Used for: Client Management
*/
		virtual LTRESULT GetClientAddr(HCLIENT hClient, uint8 pAddr[4], uint16 *pPort) = 0;

/*!
\param hClient Client to modify.

\param dwClientFlags Client info flags (a combination of the \b CIF_
flags above).

Set client info flags.

Used for: Server Management.  
*/
    void (*SetClientInfoFlags)(HCLIENT hClient, uint32 dwClientFlags);

/*!
\param hClient Client to query.

\return Client info flags (a combination of the \b CIF_ flags above).

Get client info flags.

Used for: Server Management.  
*/
    uint32 (*GetClientInfoFlags)(HCLIENT hClient);

/*!
\param hClient Client to modify.

\param pData User data.

Set user data for \b HCLIENT objects.

Used for: Server Management.  
*/
    void (*SetClientUserData)(HCLIENT hClient, void *pData);

/*!
\param hClient Client to query.

\return Pointer to user data.

Get user data for \b HCLIENT objects.

Used for: Server Management.  
*/
    void* (*GetClientUserData)(HCLIENT hClient);

/*!
\param hClient Client to complete connection.

Sets client as fully connected and able to enter worlds.
Once OnAddClient is called, the game must call
FinishClientConnect before the client will be allowed
to get OnClientEnterWorld calls.  Only needs to be
called once per client after OnAddClient.

Used for: Server Management.  
*/
    virtual LTRESULT FinishClientConnect(HCLIENT hClient) = 0;

/*!
\param hClient Client to kick.

Kick a client off the server. OnRemoveClient() will be called.

\see OnRemoveClient()

Used for: Server Management.  
*/
    LTRESULT (*KickClient)(HCLIENT hClient);

/*!
\param hClient Client to modify.

\param pPos Position (in world coordinates).

\return LT_INVALIDPARAMS if hClient is not valid. Otherwise, LT_OK.

Set where the client is seeing and hearing from.  This function
controls what the server sends the client.  You should set this every
frame.

Used for: Server Management.  
*/
    LTRESULT (*SetClientViewPos)(HCLIENT hClient, const LTVector *pPos);

/*!
Helpers.
*/

/*!
\param pObject Object to query.

\return Handle to \b pObject.

Get objects from handles.

Used for: Object.  
*/
    HOBJECT (*ObjectToHandle)(LPBASECLASS pObject);

/*!
\param pObject Handle of object to query.

\return Object.

Get handles from objects.

\note HandleToObject() will return \b NULL if the given object does not
reside on this server (i.e. if it is controlled remotely).

Used for: Server Management.  
*/
    LPBASECLASS (*HandleToObject)(HOBJECT hObject);

/*!  
Inter-object link manipulation.
*/

/*!
\param hOwner Owner object.

\param hLinked Linked object.

\return \b LT_ERROR if trying to make a link to itself.

If you want to hold on to an \b HOBJECT and receive notification when
the object is removed from the world, you can create an inter-object
link between the two.  The owner will get \b MID_LINKBROKEN
notification when the link is being broken (by either the owner or
linked object being removed).  If a link between the two objects
already exists, the function will not create another link and return
\b LT_OK.

Used for: Object.
*/
    LTRESULT (*CreateInterObjectLink)(HOBJECT hOwner, HOBJECT hLinked);

/*!
\param hOwner Owner object.

\param hLinked Linked object.

Break an inter-object link between the owner and the linked object, if
one exists.  You can only break a link from the owner, since the
linked object doesn't know it is linked to the owner.

\note \b MID_LINKBROKEN will \em not be called.

Used for: Object.
*/
    virtual void BreakInterObjectLink(HOBJECT hOwner, HOBJECT hLinked)=0;

/*!
\param hObj Object that links will be broken from.

\param hLinked Linked object.

\param bNotify Should the linked object be notified?

Break all inter-object links between the object and the linked object, if
one exists.  If hLinked is \b NULL all links to hObj are broken.

\note \b MID_LINKBROKEN will \em not be called if bNotify is \b false.

Used for: Object.
*/
	virtual void BreakInterObjectLink( HOBJECT hObj, HOBJECT hLinked, bool bNotify )=0;	

/*!
Object manipulation.
*/

/*!
\param hParent Parent object.

\param hChild Child object.

\param pSocketName Socket name (on parent).

\param pOffset Position offset (in local parent coordinates).

\param pRotationOffset Rotation offset (in local parent coordinates).

\param pAttachment (return) Attachment handle.

\return \b LT_OK, \b LT_NODENOTFOUND, or \b LT_INVALIDPARAMS.

Attaches \b hChild to \b hParent.  If \b pSocketName is set, this
function will attach \b hChild to a specific node on \b
hParent. Engine.exe will automatically detach if you remove \b hParent,
but it will \em not automatically detach if you remove \b hChild.  If
you remove the child without removing the attachment, the results are
undefined.

Used for: Object.
*/
    LTRESULT (*CreateAttachment)(HOBJECT hParent, HOBJECT hChild,
								 const char  *pSocketName, 
								 LTVector    *pOffset, 
								 LTRotation  *pRotationOffset,
								 HATTACHMENT *pAttachment);

/*!
\param hAttachment Attachment to remove.

\return \b LT_INVALIDPARAMS if hAttachment is NULL, \b LT_ERROR if hAttachment
has no parent, LT_NOTFOUND if the parent is not found, else \b LT_OK

Removes an attachment.

\note An attachment becomes invalid when you remove the parent so this
function will crash if you call it with an attachment with a parent
you've removed.

Used for: Object.
*/
    LTRESULT (*RemoveAttachment)(HATTACHMENT hAttachment);

/*!
\param hParent Parent object.

\param hChild Child object.

\param hAttachment (return) Attachment handle, or \b NULL if there is an
error.

\return \b LT_ERROR, \b LT_NOTFOUND, or \b LT_OK if it found it.

Look for an attachment on the parent.

Used for: Object.
*/
    LTRESULT (*FindAttachment)(HOBJECT hParent, HOBJECT hChild,
        HATTACHMENT *hAttachment);

/*!
\param hChild potential child object to test and see if it is attached
\param bAttached the resulting attached state of this object

Given an object, this will determine if it is attached to any other objects

Used for: Object.
*/
	virtual LTRESULT IsObjectAttached(HOBJECT hObject, bool& bAttached) = 0;

/*!
\param pName Name to search for.

\param objArray (return) An array of objects.

\param nTotalFound (return) Number of objects in array.

Get all objects with the given name.

Used for: Object.  
*/
    virtual LTRESULT FindNamedObjects(const char *pName,
                     BaseObjArray<HOBJECT> &objArray,
                     uint32 *nTotalFound=NULL)=0;

/*!
\param  pList       Object list to relinquish.

Any time the engine gives you a list, give the list back with
this function, or else you will have significant memory leaks.

Used for: Misc.  
*/
    void (*RelinquishList)(ObjectList *pList);

/*!  
Object list management.  
*/

/*!
\return A pointer to the allocated list.

Allocate a list.

\note Free the list with RelinquishList().

\see RelinquishList()

Used for: Object.
*/
    ObjectList* (*CreateObjectList)();

/*!
\param pList Object list to modify.

\param hObj Object to add.

\return a pointer to the new ObjectLink

Add objects to a list.

Used for: Object.  
*/
    ObjectLink* (*AddObjectToList)(ObjectList *pList, HOBJECT hObj);

/*!
\param pList Object list to modify.

\param hObj Object to remove.

Remove an object from the list given the handle.

\note \b Caution: Engine.exe will do a search over all objects in the list.

Used for: Object.  
*/
    void (*RemoveObjectFromList)(ObjectList *pList, HOBJECT hObj);

/*!
\param hObj Object to start from.  Use \b NULL to get first object.

\return \b NULL if there are no more objects; otherwise, returns the next
object.

Iterate through all the objects in the world.  This is generally a bad
idea for speed reasons, but sometimes it is necessary.

\see GetNextInactiveObject()

Used for: Object.
*/
    HOBJECT (*GetNextObject)(HOBJECT hObj);

/*!
\param hObj Object to start from.  Use \b NULL to get the first inactive
object.

\return \b NULL if there are no more inactive objects; otherwise, returns
the next inactive object.

Same as GetNextObject(), but this iterates over all the inactive objects.

\see GetNextObject()

Used for: Object.
*/
    HOBJECT (*GetNextInactiveObject)(HOBJECT hObj);

/*!
\param hObject Object to query.

\param pName (return) Buffer to hold name.

\param nameBufSize Buffer size.

\return LT_INVALIDPARAMS if the buffer is not large enough, else LT_OK

Get the name of an object.

Used for: Object.
*/
    virtual LTRESULT GetObjectName(HOBJECT hObject, char *pName,
        uint32 nameBufSize)=0;

/*!
\param hObj The object to modify.

\param nData The unguaranteed data value to use

\return \b LT_INVALIDPARAMS if hObj is not found, else \b LT_OK

Set the unguaranteed data value for an object

Used for: Object.
*/
	virtual LTRESULT SetObjectUnguaranteedData(HLOCALOBJ hObj, uint32 nData)=0;

/*!
\param hObj The object to modify.

\param state The object state (a combination of \b OBJSTATE_ defines).

Set the state of an object.

Used for: Object.
*/
    void (*SetObjectState)(HOBJECT hObj, int state);

/*!
\param hObj The object to query.

\return The object state (a combination of \b OBJSTATE_ defines).

Get the state of an object.

Used for: Object.
*/
    int (*GetObjectState)(HOBJECT hObj);

/*!
\param hObj The object to modify.

\param pri The blocking priority.

Set the blocking priority (defaults to 0).  See the FAQ for a description
of how this works.

Used for: Object.
*/
    void (*SetBlockingPriority)(HOBJECT hObj, uint8 pri);

/*!
\param hObj The object to query.

\return The blocking priority.

Get the blocking priority (defaults to 0).  See the FAQ for a description
of how this works.

Used for: Object.
*/
    uint8 (*GetBlockingPriority)(HOBJECT hObj);

/*!
Network session manipulation 
*/

/*!
\param sName    New session name to use.
\return \b LT_NOTINITIALIZED if the main network driver is uninitialized, or \b LT_ERROR if the session name cannot be set; otherwise, returns \b LT_OK.

Update the session name.

Used for: Networking.
*/
    LTRESULT (*UpdateSessionName)(const char* sName);

/*!
\param sName (return) String to fill with session name.
\param dwBufferSize Size of sName parameter.
\return LT_NOTINITIALIZED if the main network driver is uninitialized, or LT_ERROR if session is invalid or the buffer is too small to contain the session name; otherwise, returns LT_OK.

Get the name of the session.

Used for: Networking.
*/
    LTRESULT (*GetSessionName)(char* sName, uint32 dwBufferSize);

/*!
Model manipulation.
*/

/*!
\param hObj Model to modify.

\param bPlaying \b true for playing, or \b false for not playing.

Set the Playing state of the model.  The default state is \b false.

Used for: Models and Animation.
*/
    void (*SetModelPlaying)(HOBJECT hObj, bool bPlaying);

/*!
Save game 
*/

/*!
\param pszSaveFileName The save filename.

\param pList The list of objects to save.

\param dwParam User parameters (\b dwParam gets passed into
\b MID_SAVEOBJECT as \b fData).

\param flags The save flags (\b SAVEOBJECTS_ defines).

\return \b LT_ERROR if file could not be opened.  \b LT_OK on success.

Save objects to a file.

Used for: Misc.
*/
    LTRESULT (*SaveObjects)(const char *pszSaveFileName, ObjectList *pList,
        uint32 dwParam, uint32 flags);

/*!
\param pszRestoreFileName The restore filename.

\param dwParam User parameters (\b dwParam gets passed into
\b MID_LOADOBJECT as \b fData).

\param flags The save flags (\b RESTOREOBJECTS_ defines).

\return \b LT_OK on success.

Restore objects from a file.

Used for: Misc.
*/
    LTRESULT (*RestoreObjects)(const char *pszRestoreFileName, uint32 dwParam,
        uint32 flags);

/*!
\param nSaveFileVersion - The save file version number.

\return \b LT_OK on success.

Access the engine's save file version.

Used for: Misc.
*/
    virtual LTRESULT GetSaveFileVersion( uint32& nSaveFileVersion ) = 0;

/*!
Load world
*/

/*!
\param pszWorldFileName Name and path in the rez of the world file to load.

\param flags A combination of the \b LOADWORLD_ flags.

\return LT_MISSINGWORLDFILE is the world file cannot be found. Otherwise, LT_OK.

Used for: Server Management.
*/
    LTRESULT (*LoadWorld)(const char *pszWorldFileName, uint32 flags);

/*!
\return LT_NOTINWORLD if world is not loaded. Otherwise, LT_OK.

Run the loaded world.

Used for: Server Management.
*/
    LTRESULT (*RunWorld)();

/*!
\param msg		Message to send.
\return \b LT_NOTFOUND if there is no server app or if it isn't set up to receive the message; otherwise, returns \b LT_OK.

Send a message to the standalone server app.  It will receive it through the ServerAppHandler::ShellMessageFn() function.

Used for: Networking.
*/
		LTRESULT (*SendToServerApp)( ILTMessage_Read& msg );

/*!
\param nMaxConnections (return) Filled in with maximum number of remote connections allowed.
\return LT_NOTINITIALIZED if the main network driver is uninitialized; otherwise, returns LT_OK.

Get the maximum number of remote clients allowed for the session.

Used for: Networking.
*/
    LTRESULT (*GetMaxConnections)(uint32 &nMaxConnections);

/*!
\param socket (return) Reference to a SOCKET that will contain the UDP socket if one is found.
\return \b LT_NOTFOUND if there is not UDP socket; otherwise, returns \b LT_OK.
*/
	typedef unsigned int SOCKET;
	virtual LTRESULT GetUDPSocket(SOCKET& socket) = 0;

/*!  
Main functions.  
*/

/*!
Grants access to the \b m_pGameInfo from the \b StartGameRequest structure.

\param ppData Filled with a pointer to the GameInfo data.
\param pLen Filled with the length of the GameInfo data.
\return LT_OK

\note This is \em not copy of the data. The pointer filled in can be NULL if
there is no game info data.

Used for: Server Management.  
*/
    LTRESULT (*GetGameInfo)(void **ppData, uint32 *pLen);

/*!
\param hClass Class to query.
\param count (return) Number of properties.
\return LT_OK

Get the number of properties defined by the given class.

Used for: Server Management.
*/
    virtual LTRESULT GetNumClassProps(const HCLASS hClass,
        uint32 &count)=0;

/*!
\param pName Name of class to retrieve.

\return \b NULL if the class doesn't exist; otherwise, returns the
handle of the requested class.

Used for: Server Management.  
*/
    HCLASS (*GetClass)(const char *pName);

/*!
\param  hClass      Class to query.
\param  iProp       Property to query.
\param  info (return) Property info.
\return LT_OK

Get the \b ClassDef info from a class. This is useful if you want to
access the property names of the class.

Used for: Server Management.  
*/
    virtual LTRESULT GetClassProp(const HCLASS hClass,
        const uint32 iProp, ClassPropInfo &info)=0;

/*!
\param  hClass Class to query.
\param  pName (return) Buffer for name.
\param  maxNameBytes (return) Size of buffer.
\return LT_OK

Get the name of a class.

Used for: Server Management.
*/
    virtual LTRESULT GetClassName(const HCLASS hClass,
        char *pName, uint32 maxNameBytes)=0;

/*!
\param  hObject     Object to query.
\return The class of the object.

Get the class of an object.

Used for: Object.
*/
    HCLASS (*GetObjectClass)(HOBJECT hObject);

/*!
\param  hClass      Class to query.
\param  hTest       Test base class.

\return \b TRUE if \b hClass is an \b hTest, or is derived from \b hTest.
Returns \b FALSE if \b hClass is not an \b hTest, if \b hClass is not
derived from \b hTest, or if \b hTest is \b NULL.

Tells if \b hClass is the class of \b hTest (or is derived from \b hTest).  If
you pass in \b NULL for \b hTest (for example, if GetClass() returns \b NULL
in IsKindOf(hClass, GetClass("TestClass"))), then IsKindOf() will return \b FALSE.

Used for: Object.  
*/
    bool (*IsKindOf)(HCLASS hClass, HCLASS hTest);

/*!
\param  hClass      Class to use for object creation.
\param  pStruct     Description of the object.
\return Handle to object.

Create an object using the default type of the object.  If you pass in \b NULL for
pStruct, OnPreCreate will not be called, and the object will not be added to the world.
(This is only valid for creating CF_CLASSONLY objects.)

Used for: Object.
*/
    LPBASECLASS (*CreateObject)(HCLASS hClass, ObjectCreateStruct *pStruct);

/*!
\param  hClass      Class to use for object creation.
\param  pStruct     Description of object.
\param  pszProps    Properties string.
\return Handle to object.

Create an object, using a string to specify the properties of the object.

Used for: Object.
*/
    LPBASECLASS (*CreateObjectProps)(HCLASS hClass,
        ObjectCreateStruct *pStruct, const char *pszProps);

/*!
Re-declaration of ILTCSBase::RemoveObject(HOBJECT)
*/
    virtual LTRESULT RemoveObject(HOBJECT hObj) = 0;

/*!
Helpers.
*/

/*!
\param pMsg The printf-styled format string to display.  This
parameter can be followed by the usual format parameters.

Output a TRACE message to the Debug Output window.  Newlines
must be explicitly used.

Used for: Misc.  
*/
    void (*DebugOut)(const char *pMsg, ...);

/*!
\param  tSkyCamera		The transform to use as the base sky camera transform

\return Always returns \b LT_OK.

Sets the transform that should be used when rendering the sky

Used for: Special FX.
*/
    virtual LTRESULT SetSkyCamera(const LTRigidTransform& tSkyCamera) = 0;

/*!
\param  nNumDownloadArchiveFiles (out)	 The number of downloadable archive files in use by the server.

\return Always returns \b LT_OK.

Gets the number of downloadable archive files in use by the server.

Used for: Misc.
*/
virtual LTRESULT GetNumDownloadableArchiveFiles(uint32& nNumDownloadArchiveFiles) = 0;

/*!
\param  nElement    Index into the list of downloadable archive files.
\param  cGUID (out) GUID of the downloadable archive file.
\param  nSize (out) Size of the file.
\return LT_ERROR if the specified index is invalid, or LT_OK.

Get information about the downloadable archive file specified by nIndex.

Used for: Misc.
*/
virtual LTRESULT GetDownloadableArchiveFileInfo(const uint32 nElement, LTGUID& cGUID, uint64& nSize) = 0;

/*!
\return The server real time in seconds.

Used for: Misc.
*/
virtual LTDOUBLE GetRealTime( ) = 0;

/*!
\return The server real time in milliseconds.

Used for: Misc.
*/
virtual uint32 GetRealTimeMS( ) = 0;

/*!
\return Whether running the MP exe.

Used for: Misc.
*/
virtual bool IsMultiplayerExe( ) = 0;

};

#endif  //! __ILTSERVER_H__






