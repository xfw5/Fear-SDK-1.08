#ifndef __ICLIENTSHELL_H__
#define __ICLIENTSHELL_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

class ILTMessage_Read;
class ILTClient;
extern ILTClient *g_pLTClient;

// Passed into OnConsolePrint for customizing console printing...
class CConsolePrintData
{
	public:
		LTRGB	m_Color;
		int		m_nFilterLevel;
		const char	*m_pMessage;
};

/*!
IClientShell interface. This is full of callback functions that
the engine will call to notify you of things.
*/
class IClientShell : public IBase {
public :
    interface_version(IClientShell, 5);

/*!
\param  pMode       Render mode (initially filled in by engine with 
                    values defined in autoexec.cfg).
\param  pAppGuid    (output) Application Globally Unique Identifier. 
                    Be sure to initialize this before returning. 
                    This controls what netgames you can query and 
                    which ones you can connect to.

\return \b LT_OK - Everything went fine.
\return \b LT_ERROR - A fatal error occurred.

Called after the engine has been fully initialized and is ready to go.
The engine doesn't initialize the renderer itself, but passes in the
default RMode it would use to initialize the renderer (from the
"RenderDLL", "ScreenWidth", and "ScreenHeight" console variables).
You must call SetRenderMode to initialize the renderer in here.
Initialize pAppGuid before returning.  If this function returns
something other than LT_OK, the engine will shut down immediately.
Typically, you would also initialize any client-side lists, managers, 
etc, and create a camera object.  Additionally, this function can 
process command-line arguments, and set the initial game mode (go to 
a menu, run a world, etc).

\note You MUST override this function in your client shell, or else 
LT_ERROR will be returned and the game will not run.

Used for: ClientShell Callbacks.
*/
    virtual LTRESULT OnEngineInitialized(LTGUID *pAppGuid, RMode *pMode) = 0;

/*!
Called just before the engine unloads the clientshell and shuts down.

Used for: ClientShell Callbacks.
*/
    virtual void OnEngineTerm() = 0;

/*!
\param  dwEventID   ID of event (one of the LTEVENT_ defines)
\param  dwParam     ???

Called when a particular engine event happens (e.g. the game lost focus 
due to the player pressing ALT-TAB, or the renderer was initialized, etc).

Used for: ClientShell Callbacks.
*/
    virtual void OnEvent(uint32 dwEventID, uint32 dwParam) = 0;

/*!
\param  hMain       The object involved in this collision.
\param  pInfo       Collision info (provided by engine).
\param  forceMag    Magnitude of collision (provided by engine).

\return \b ???

Called when an object (with FLAG_TOUCH_NOTIFY) moved on the client 
collides with another object or the world.

Used for: ClientShell Callbacks.
*/
	virtual LTRESULT OnTouchNotify(HOBJECT hMain, CollisionInfo *pInfo, float forceMag) = 0;

/*!
\param  pPlaySoundInfo  Description of sound (provided by engine).

Called just after a sound is played by the client or server.

Used for: ClientShell Callbacks.
*/
    virtual void OnPlaySound(PlaySoundInfo *pPlaySoundInfo) = 0;
	virtual void OnGetOcclusionFromPoly(HPOLY *pWorldPoly, float *occlusionDampening, float *occlusionLFRatio) = 0;
	virtual void OnGetSoundFilterFromPoint(LTVector *pVector, char* pszSoundFilter, int32 nMaxChars) = 0;
	virtual void OnGetSoundFilterParamCountFromName(char* pszSoundFilter, int32* piNumParams) = 0;
	virtual void OnGetSoundFilterParamFromName(char* pszSoundFilter, int32 nIndex, char* pszParamName, int32 nParamLength, float* pfParamValue) = 0;

/*!
\param  pData       The packet data.
\param  dataLen     Length of the packet data.
\param  senderAddr  IP address of sender.
\param  senderPort  Port of sender.

\return The engine ignores the callback return value.

Called when the client gets a network packet that it cannot understand.
Typically, this is used in conjunction with apps like GameSpy, etc.

Used for: ServerShell Callbacks.
*/
    virtual LTRESULT ProcessPacket(ILTMessage_Read *pMsg, uint8 senderAddr[4], uint16 senderPort) = 0;

/*!
\param	pData		Pointer to a CConsolePrintData structure containing
					information about the string to be printed.

\return	\b ???

Called when the engine wishes to send output to the console.  Any changes
to the pData structure are reflected in the output, so setting 
pData->m_pMessage to NULL will prevent the engine from printing a message
to the console, etc.

Used for: ClientShell Callbacks.
*/
	virtual	void OnConsolePrint (CConsolePrintData *pData) = 0;

/*!
\param  key     The keycode for the key that was released.

Called if a key is released.  Try to use OnCommandOff() whenever 
possible because key up/down notification is not portable.  Input 
is processed during each update loop, before PreUpdate() is called.

Used for: ClientShell Callbacks.
*/
    virtual void OnKeyUp(int key) = 0;

/*!
\param  key     The keycode for the key that was pressed.
\param  rep     number of repetitions that have occurred since the last 
                update.

Called each update loop if a key is pressed (or held). Try to use 
ILTInput whenever possible because key up/down notification is 
not portable.  Input is processed during each update loop, before 
PreUpdate() is called.

Used for: ClientShell Callbacks.
*/
    virtual void OnKeyDown(int key, int rep) = 0;

/*!
\param  hObj		The model that is currently hitting the frame key.
\param  hTrackerID	The ID of the tracker that triggered this model key string
\param  pArgs		The frame string, broken up into an array of tokens.

Called when a model object with client flag CF_NOTIFYMODELKEYS hits a
frame key.

Used for: ClientShell Callbacks.
*/
    virtual void OnModelKey(HLOCALOBJ hObj, ANIMTRACKERID hTrackerID, ArgList *pArgs) = 0;

/*!
Called when the client enters a world (after the world has been loaded and 
all objects have been loaded, etc).

Used for: ClientShell Callbacks.
*/
    virtual void OnEnterWorld() = 0;

/*!
Called when the client exits a world (after disconnecting/unloading 
the server and all objects have been deleted). You lose \em all server 
objects when you exit the world, so don't reference them after exiting 
the world!

Used for: ClientShell Callbacks.
*/
    virtual void OnExitWorld() = 0;

/*!
\param  messageID   ID of message. This is a custom ID, not an engine ID.
\param  pMessage    The message.

Called whenever the client receives a message from the server.  These 
messages are those that are defined and sent via game code.

Used for: ClientShell Callbacks.
*/
    virtual void OnMessage(ILTMessage_Read *pMessage) = 0;

/*!
\param  pWorldName  Name of the world about to be loaded.

Called right before a world is loaded.  Typically this function 
displays a "loading" screen.

Used for: ClientShell Callbacks.
*/
    virtual void PreLoadWorld(const char *pWorldName) = 0;

/*!
\param  hObj        The object that is about to move.
\param  bTeleport   Teleport status. True = object is teleporting. 
                    False = object is moving normally, and thus physics are 
                    applied.
\param  pNewPos     The destination position.  This CAN be intercepted and 
                    modified in this function.

\return \b ???

Called just before the engine moves an object.  pNewPos can be 
modified at this time.

Used for: ClientShell Callbacks.
*/
    virtual LTRESULT OnObjectMove(HLOCALOBJ hObj, bool bTeleport, LTVector *pNewPos) = 0;

/*!
Called during every update cycle. This is the function that usually 
updates all client-side systems, objects, etc.

Used for: ClientShell Callbacks.
*/
    virtual void Update() = 0;

/*!
\param  hObj        The object associated with the special effect. If NULL, 
                    then this is an "instant" special effect.
\param  pMessage    Special effect message.

Called when the server tells the client about a special effect on an object.
If hObj is NULL, then this is an "instant" special effect (ie. it's not 
associated with an object, but instead, with a location).  These messages 
are those that are defined by game code, and are usually (but not necessarily) 
sent via game code.

Used for: ClientShell Callbacks.
*/
    virtual void SpecialEffectNotify(HLOCALOBJ hObj, ILTMessage_Read *pMessage) = 0;

/*!
\param  hObj    The object that is about to be removed.

Called when an object with client flag CF_NOTIFYONREMOVE is removed.

Used for: ClientShell Callbacks.
*/
    virtual void OnObjectRemove(HLOCALOBJ hObj) = 0;

/*!
\param  hObject		The object that is being added.

Called when an object added to the client world.

Used for: ClientShell Callbacks.
*/
	virtual void OnObjectAdd( HLOCALOBJ hObject ) = 0;

/*!
Called at the beginning of every update cycle -- before Update().

\note Technically, Update() is called immediately after PreUpdate(). 
Therefore, the only benefit to having code in PreUpdate() is purely 
organizational.

Used for: ClientShell Callbacks.
*/
    virtual void PreUpdate() = 0;

/*!
Called at the end of every update cycle -- after Update(), *and* after 
all client-side objects have been processed by the engine.

Used for: ClientShell Callbacks.
*/
    virtual void PostUpdate() = 0;

/*!
\param  hObj        The object that is about to rotate.
\param  bTeleport   Teleport status. True = object is teleporting. 
                    False = object is moving normally, and thus physics are 
                    applied.
\param  pNewRot     The destination rotation.  This CAN be intercepted and 
                    modified in this function.

\return \b ???

Called just before the engine rotates an object.  pNewRot can be 
modified at this time.

Used for: ClientShell Callbacks.
*/
    virtual LTRESULT OnObjectRotate(HLOCALOBJ hObj, bool bTeleport, LTRotation *pNewRot) = 0;

/*!
Called during long-running operations.  This allows the shell to cancel an
operation in response to user input or other events.  Returning LT_YES
from this method will cancel the operation.

Used for: ClientShell Callbacks.
*/
	virtual LTRESULT CheckForCancel() = 0;

}; 


/*!
IClientShellStub class. This class gives empty bodies to all IClientShell
functions.  Derive your IClientShell class from IClientShellStub and
you only need to declare functions that have non-trivial bodies.
*/
class IClientShellStub : public IClientShell {
public:
    virtual void OnKeyDown(int key, int rep) { LTUNREFERENCED_PARAMETER(key); LTUNREFERENCED_PARAMETER(rep); }
    virtual void OnKeyUp(int key) { LTUNREFERENCED_PARAMETER(key); }
    virtual void OnMessage(ILTMessage_Read *pMessage) { LTUNREFERENCED_PARAMETER(pMessage); }
    virtual void OnModelKey(HLOCALOBJ hObj, ArgList *pArgs) { LTUNREFERENCED_PARAMETER(hObj); LTUNREFERENCED_PARAMETER(pArgs);}
	virtual void PreLoadWorld(const char *pWorldName) { LTUNREFERENCED_PARAMETER(pWorldName); }
    virtual void OnEnterWorld() {}
    virtual void OnExitWorld() {}
    virtual void SpecialEffectNotify(HLOCALOBJ hObj, ILTMessage_Read *pMessage) { LTUNREFERENCED_PARAMETER(hObj); LTUNREFERENCED_PARAMETER(pMessage); }
    virtual void OnObjectRemove(HLOCALOBJ hObj) { LTUNREFERENCED_PARAMETER(hObj);}
    virtual void PreUpdate() {}
    virtual void Update() {}
    virtual void PostUpdate() {}
    virtual LTRESULT OnObjectMove(HLOCALOBJ hObj, bool bTeleport, LTVector *pNewPos)
	{
		LTUNREFERENCED_PARAMETER(hObj);
		LTUNREFERENCED_PARAMETER(bTeleport);
		LTUNREFERENCED_PARAMETER(pNewPos);
		return LT_OK;
	}
    virtual LTRESULT OnObjectRotate(HLOCALOBJ hObj, bool bTeleport, LTRotation *pNewRot)
	{
		LTUNREFERENCED_PARAMETER(hObj);
		LTUNREFERENCED_PARAMETER(bTeleport);
		LTUNREFERENCED_PARAMETER(pNewRot);
		return LT_OK;
	}
    virtual LTRESULT OnEngineInitialized(RMode *pMode, LTGUID *pAppGuid) 
	{
		LTUNREFERENCED_PARAMETER(pMode); LTUNREFERENCED_PARAMETER(pAppGuid);
		return LT_ERROR;
	}
    virtual void OnEngineTerm() {}
	virtual void OnEvent(uint32 dwEventID, uint32 dwParam) { LTUNREFERENCED_PARAMETER(dwEventID); LTUNREFERENCED_PARAMETER(dwParam); }
    virtual LTRESULT OnTouchNotify(HOBJECT hMain, CollisionInfo *pInfo, float forceMag)
	{
		LTUNREFERENCED_PARAMETER(hMain);
		LTUNREFERENCED_PARAMETER(pInfo);
		LTUNREFERENCED_PARAMETER(forceMag);
		return LT_OK;
	}
    virtual void SRand() {srand(123);}
    virtual void OnPlaySound(PlaySoundInfo *pPlaySoundInfo) { LTUNREFERENCED_PARAMETER( pPlaySoundInfo ); }
	virtual void OnGetOcclusionFromPoly(HPOLY *pWorldPoly, float *occlusionDampening, float *occlusionLFRatio)
	{ LTUNREFERENCED_PARAMETER( pWorldPoly );
	LTUNREFERENCED_PARAMETER( occlusionDampening );
	LTUNREFERENCED_PARAMETER( occlusionLFRatio );}
	virtual void OnGetSoundFilterFromPoint(LTVector *pVector, char* pszSoundFilter, int32 nMaxChars)
		{ LTUNREFERENCED_PARAMETER( pVector ); LTUNREFERENCED_PARAMETER( pszSoundFilter ); LTUNREFERENCED_PARAMETER( nMaxChars );}
	virtual void OnGetSoundFilterParamCountFromName(char* pszSoundFilter, int32* piNumParams)
		{  LTUNREFERENCED_PARAMETER (pszSoundFilter); LTUNREFERENCED_PARAMETER (piNumParams);}
	virtual void OnGetSoundFilterParamFromName(char* pszSoundFilter, int32 nIndex, char* pszParamName, int32 nParamLength, float* pfParamValue)
		{  LTUNREFERENCED_PARAMETER (pszSoundFilter); LTUNREFERENCED_PARAMETER (nIndex), LTUNREFERENCED_PARAMETER(pszParamName);LTUNREFERENCED_PARAMETER(nParamLength);LTUNREFERENCED_PARAMETER(pfParamValue);}
	virtual	void OnConsolePrint (CConsolePrintData *pData) { LTUNREFERENCED_PARAMETER( pData ); }
    virtual LTRESULT ProcessPacket(ILTMessage_Read *pMsg, uint8 senderAddr[4], uint16 senderPort) 
	{ 
		LTUNREFERENCED_PARAMETER(pMsg);
		LTUNREFERENCED_PARAMETER(senderAddr);
		LTUNREFERENCED_PARAMETER(senderPort);
		return LT_OK; 
	}
	virtual LTRESULT CheckForCancel() { return LT_NO; }
};


/*!
Interface class that manages the client shell DLL's windows instance handle.
*/
class IInstanceHandleClient : public IBase {
public:
    interface_version(IInstanceHandleClient, 4);

/*!
Called by the engine if the interface is defined.  If the client shell
is in a windows DLL, the instance handle is passed in.  On other platforms,
or if the client shell is not in a DLL, NULL is passed in.
*/
    virtual void SetInstanceHandle(void *handle) = 0;

/*!
Call this function to retrieve the instance handle that was given by the engine.
*/
    virtual void *GetInstanceHandle() = 0;
};

/*!
Macro that defines the global g_pLTClient holder
*/

#define SETUP_GPLTCLIENT()                  \
    ILTClient *g_pLTClient;                 \
    define_holder(ILTClient, g_pLTClient);  \


/*!
Macro that defines and instantiates an implementation of the IInstanceHandleClient
interface.
*/
#define SETUP_DLLINSTANCECLIENT()                                           \
    class CInstanceHandleClient : public IInstanceHandleClient {            \
      public:                                                               \
        declare_interface(CInstanceHandleClient);                                                \
        CInstanceHandleClient() {hLTDLLInstance = NULL;}                    \
        void SetInstanceHandle(void *handle) {hLTDLLInstance = handle;}     \
        void *GetInstanceHandle() {return hLTDLLInstance;}                  \
        void *hLTDLLInstance;                                               \
    };                                                                      \
    define_interface(CInstanceHandleClient, IInstanceHandleClient);         \


/*!
Use this macro one time somewhere in your game client code.

It sets up a global pointer to the ILTClient interface.
*/
#define SETUP_CLIENTSHELL()         \
    SETUP_GPLTCLIENT();             \
    SETUP_DLLINSTANCECLIENT();      \


#endif  //! __ICLIENTSHELL_H__


