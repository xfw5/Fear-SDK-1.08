/*!
Base class for ILTClient and ILTServer.  
This is just here for b/w compatibility.
*/

#ifndef __ILTCSBASE_H__
#define __ILTCSBASE_H__


#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

/*! Forward declarations of base interface classes */

class ILTPhysics;
class ILTPhysicsSim;
class ILTResourceMgr;
class ILTTimer;
class ILTModel;
class ILTSoundMgr;
class ILTCommon;
class ILTMessage_Read;
class ILTMessage_Write;
class LTObjRef;
class ILTFileMgr;
class ILTLoadingProgress;

//--------------------------------------------------------------------------------------------------
// ILTCSBase Types
//--------------------------------------------------------------------------------------------------

//handle used to represent a console variable state
struct LTCommandVar;
typedef LTCommandVar* HCONSOLEVAR;

//function prototype for console programs that will be called when the user enters the registered name
//and will provide the parsed arguments to this function
typedef void (*ConsoleProgramFn)(int argc, char **argv);

/*! Types used by this interface */

//--------------------------------------------------------------------------------------------------
// ILTCSBase
//--------------------------------------------------------------------------------------------------

class ILTCSBase : public IBase {
public:
    interface_version(ILTCSBase, 1);

/*!
Messaging functions.  All of these functions are obsolete.  Use
the LMessage functions. */

public:
                
/*!
\param sAddress (return) buffer to receive local IP address string in dot notation format (NNN.NNN.NNN.NNN) followed by a null byte.
\param dwBufferSize  size of sAddress buffer (this should be at least 17 bytes to ensure room for the dot notation string and a null byte)  if there is not enough room the string will be truncated to (dwBufferSize - 1) bytes.
\param hostPort (return) filled in with host port
\return \b LT_INVALIDPARAMS if \b sAddress is null, or \b LT_NOTINITIALIZED if there is no main driver; otherwise returns \b LT_OK.

Get the TCP/IP address of the main driver, if it is available.  If you
are hosting a game, \b hostPort is filled in with the port you are
hosting on.  If you are not hosting, it is set to 0xFFFF.

\note  dwBufferSize must be greater than zero bytes and less than or equal to the size of the space allocated for sAddress.   This is not checked by the current code in the engine, and it will fault if you pass in zero bytes.

Used for: Networking.
*/
    LTRESULT (*GetTcpIpAddress)(char* sAddress, uint32 dwBufferSize,
        uint16 &hostPort);

/*!
\param  pMsg	user data to send
\param  pAddr   IP address to target; can be numeric or host name.
\param  nPort   Port to use
\return \b LT_NOTINITIALIZED if there is no TCP/IP driver or the driver's socket is uninitialized, or \b LT_ERROR if the address cannot be built; otherwise, returns \b LT_OK.

Send a message using the TCP/IP protocol.

\b sAddr is a string containing an IP addresses to check.
If the first character of the address is a numeral (between '0' and '9') it is treated
as a numeric IP address; otherwise it's treated as a host name.

Used for: Networking. 
*/
    virtual LTRESULT SendTo(ILTMessage_Read *pMsg, const char *pAddr, uint16 nPort) = 0;

/*!
\param  pAddr   IP address to target; can be numeric or host name.
\param  nPort   Port to use
\param	pPingID Resulting Ping ID
\return \b LT_NOTINITIALIZED if there is no TCP/IP driver or the driver's socket is uninitialized, or \b LT_ERROR if the address cannot be built; otherwise, returns \b LT_OK.

Start a ping request to a TCP/IP address

Used for: Networking
*/
    virtual LTRESULT StartPing(const char *pAddr, uint16 nPort, uint32 *pPingID) = 0;

/*!
\param	nPingID		Ping ID to query
\param	pStatus		Ping status result
\param  pLatency	Latency result if *pStatus == PING_STATUS_SUCCESS

\return \b LT_NOTFOUND if that ping ID is unknown, LT_OK otherwise.
*/
    virtual LTRESULT GetPingStatus(uint32 nPingID, uint32 *pStatus, uint32 *pLatency) = 0;

/*!
\param	nPingID	Ping ID to remove

\return \b LT_NOTFOUND if that ping ID is unknown, LT_OK otherwise.
*/
    virtual LTRESULT RemovePing(uint32 nPingID) = 0;

/*!
\return This function returns the main world model if in a world with a main world model, otherwise NULL
*/
	virtual HOBJECT GetMainWorldModel() = 0;

/*!
Gets the rotation of the specified object

Used for: Object.
*/
    virtual LTRESULT GetObjectRotation(HLOCALOBJ hObj, LTRotation *pRotation) = 0;


/*!
Sets the rotation of the specified object

Used for: Object.
*/
	virtual LTRESULT SetObjectRotation(HOBJECT hObj, const LTRotation& rRotation) = 0;


/*!
Gets the position of the specified object

Used for: Object.
*/
    virtual LTRESULT GetObjectPos(HLOCALOBJ hObj, LTVector *pPos) = 0;

/*!
Sets the position of the specified object

Used for: Object.
*/
	virtual LTRESULT SetObjectPos(HLOCALOBJ hObj, const LTVector& vPos) = 0;

/*!
Forces (teleports) the object to its current position

Used for: Object.
*/
virtual LTRESULT ForceCurrentObjectPos(HLOCALOBJ hObj) = 0;

/*!
Gets the transform of the specified object

Used for: Object.
*/
    virtual LTRESULT GetObjectTransform(HLOCALOBJ hObj, LTRigidTransform *pTransform) = 0;

/*!
Sets the transform of the specified object

Used for: Object.
*/
	virtual LTRESULT SetObjectTransform(HLOCALOBJ hObj, const LTRigidTransform& tTransform) = 0;

/*!
Gets the transform of the specified object

Used for: Object.
*/
	virtual LTRESULT GetObjectTransform(HLOCALOBJ hObj, LTTransform *pTransform) = 0;

/*!
\return Pointer to the \b ILTResourceMgr interface.

Accesses the ILTResourceMgr interface which is used for management of assets

Used for:   Interface.
*/
	virtual ILTResourceMgr*	ResourceMgr() = 0;

/*!
\return Pointer to the \b ILTTimer interface.

Accesses the ILTTimer interface which is used for controlling the game and engine timers

Used for:   Interface.
*/
	virtual ILTTimer *Timer() = 0;

/*!
\return Pointer to the \b ILTModel interface.

Accesses the ILTModel interface.

Used for:   Interface.
*/
    virtual ILTModel *GetModelLT() = 0;

/*!
\return Pointer to the \b ILTSoundMgr interface.

Accesses the ILTSoundMgr interface.

Used for:   Interface.
*/
    virtual ILTSoundMgr *SoundMgr() = 0;

/*!
\return Pointer to the \b ILTFileMgr interface.

Accesses the ILTFileMgr interface.

Used for:   Interface.
*/
    virtual ILTFileMgr *FileMgr() = 0;

/*!
\return Pointer to the \b ILTCommon interface.

Accesses the ILTCommon interface.

Used for:   Interface.
*/
    virtual ILTCommon *Common() = 0;

/*!
\return Pointer to the \b ILTPhysics interface.

Accesses the ILTPhysics interface.

Used for:   Interface.
*/
    virtual ILTPhysics *Physics() = 0;

/*!
\return Pointer to the \b ILTPhysicsSim interface.

Accesses the ILTPhysicsSim interface which is used for the rigid body simulation

Used for:   Interface.
*/
    virtual ILTPhysicsSim *PhysicsSim() = 0;

/*!
\return Pointer to the \b ILTLoadingProgress interface.

Accesses the ILTLoadingProgress interface.

Used for:   Interface.
*/
    virtual ILTLoadingProgress *LoadingProgress() = 0;

/*!
Sets the transform of the specified object

Used for: Object.
*/
	virtual LTRESULT SetObjectTransform(HLOCALOBJ hObj, const LTTransform& tTransform) = 0;

/*!
\param hObj Object to query.

\param pScale (return) Scale.

\return \b LT_ERROR if jObj or pScale is invalid, else \b LT_OK

Get the scale of the object (only works on models and sprites).

Used for: Object.
*/
    virtual LTRESULT GetObjectScale(HOBJECT hObj, float* pScale) = 0;

/*!
\param hObj Object to modify.

\param pNewScale New scale to set.

Scale the object. Note that the engine only supports uniform scaling.

Used for: Object.
*/
    virtual LTRESULT SetObjectScale(HOBJECT hObj, float fScale) = 0;

/*!
\param  hObject     Object to query.
\param  r           (return) Red value (range: 0.0-1.0).
\param  g           (return) Green value (range: 0.0-1.0).
\param  b           (return) Blue value (range: 0.0-1.0).
\param  a           (return) Alpha value (range: 0.0-1.0).

Get the color and alpha of the object.

\see SetObjectColor()

Used for: Object.
*/
    virtual LTRESULT GetObjectColor(HLOCALOBJ hObject, float *r, float *g, float *b, float *a) = 0;

/*!
\param  hObject     Object to modify.
\param  r           Red value (range: 0.0-1.0).
\param  g           Green value (range: 0.0-1.0).
\param  b           Blue value (range: 0.0-1.0).
\param  a           Alpha value (range: 0.0-1.0).

Set the color and alpha of the object.

\note This function overrides a model's render style's material diffuse color.

\see GetObjectColor()

Used for: Object.
*/
    virtual LTRESULT SetObjectColor(HLOCALOBJ hObject, float r, float g, float b, float a) = 0; 

/*!
\param  hObject     Object to modify.
\param  eShadowLOD  The LOD at which the shadow will be visible

Gets the LOD associated with either a model or world model so that it will only be
cast at that LOD or higher. This will return LT_INVALIDPARAMS if the object type
is anything other than a model or world model

Used for: Object.
*/
    virtual LTRESULT GetObjectShadowLOD(HLOCALOBJ hObject, EEngineLOD& eShadowLOD) = 0; 

/*!
\param  hObject     Object to modify.
\param  eShadowLOD  The LOD at which the shadow will be visible

Sets the LOD associated with either a model or world model so that it will only be
cast at that LOD or higher. This will return LT_INVALIDPARAMS if the object type
is anything other than a model or world model

Used for: Object.
*/
    virtual LTRESULT SetObjectShadowLOD(HLOCALOBJ hObject, EEngineLOD eShadowLOD) = 0; 

/*!
\param hObj Model instance
\param pAnimName name of animation
\return -1 if the animation doesn't exist or if the object isn't a
model.

Get an animation index from a model.
The animation index is a handle to an animation associated with a model.

Used for: Models and Animation.  */
    virtual HMODELANIM  GetAnimIndex(HOBJECT hObj, const char *pAnimName) = 0;

/*!
\param hObj ???
\return The animation the model is currently on, or (\b HMODELANIM)-1 if
none.

Used for: Models and Animation.  */
    virtual HMODELANIM  GetModelAnimation(HOBJECT hObj) = 0;



/*!
\param hObj ???
\param bLoop ???

Set the looping state of the model.  The default state is \b TRUE.

Used for: Models and Animation.
*/
    virtual void    SetModelLooping(HOBJECT hObj, bool bLoop) = 0;


/*!
\param hObj ???
\return ???

Get the looping state of the model.  The default state is \b TRUE.

Used for: Models and Animation.
*/
    virtual bool  GetModelLooping(HOBJECT hObj) = 0;

/*!
\param hObj ???
\return ???

Tell what the playback state of the model is (a combination of the
\b MS_ bits defined in basedefs_de.h).

Used for: Models and Animation.
*/
    virtual uint32  GetModelPlaybackState(HOBJECT hObj) = 0;

/*!
\param hLight the light object to perform the operation on
\param eLightType the type of the specified light, for possible values see the enumeration
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Changes the type of the associated light. This is used to control how a light uses its
parameters for rendering.

Used for: Light Control.
*/
	virtual LTRESULT	SetLightType(HOBJECT hLight, EEngineLightType eLightType) = 0;
/*!
\param hLight the light object to perform the operation on
\param eLightType the type of the specified light, for possible values see the enumeration
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the type of the associated light. This is used to control how a light uses its
parameters for rendering.

Used for: Light Control.
*/
	virtual LTRESULT	GetLightType(HOBJECT hLight, EEngineLightType& eLightType) = 0;
/*!
\param hLight the light object to perform the operation on
\param fRadius the radius of the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the radius of the specified light

Used for: Light Control.
*/
	virtual LTRESULT	SetLightRadius(HOBJECT hLight, float fRadius) = 0;
/*!
\param hLight the light object to perform the operation on
\param fRadius the radius of the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the radius of the specified light

Used for: Light Control.
*/
	virtual LTRESULT	GetLightRadius(HOBJECT hLight, float& fRadius) = 0;
/*!
\param  pName       Name of console program.
\param  fn          Associated function.

\return \b LT_OK or \b LT_ALREADYEXISTS.

Register a console program.  pName is just stored so it should either
be static or allocated.  When the clientshell DLL is unloaded, it gets
rid of any registered programs.

Used for: Misc.
*/
    virtual LTRESULT RegisterConsoleProgram(const char *pName, ConsoleProgramFn fn) = 0;

/*!
\param  pName   Name of console program.

\return \b LT_OK or \b LT_NOTFOUND.

Unregister a console program.

Used for: Misc.
*/
    virtual LTRESULT UnregisterConsoleProgram(const char *pName) = 0;

/*!
\param hObj Handle to container object.
\param pCode Address of variable that gets set.
\return \b false when the object isn't a container.

Get the container's container code (can only be set in the 
\b ObjectCreateStruct during creation).  

Used for: Misc.
*/
    virtual bool  GetContainerCode(HOBJECT hObj, uint16 *pCode) = 0;

/*!
\param hObj The object to query.

\param pContainerList (return) The container list array/buffer.

\param pFlagList (return) The flag list array/buffer.

\param maxListSize The number of elements in list arrays.

\return The number of elements in \b pContainerList filled in.

Get the list of containers the object is inside.  \b pFlagList must
be the same size as \b pContainerList.

Used for: Object.
*/
    virtual uint32 GetObjectContainers(HOBJECT hObj,
        HOBJECT *pContainerList, uint32 maxListSize) = 0;

/*!
\param hContainer The container to query.

\param pObjectList (return) The object list array/buffer.

\param pFlagList (return) The flag list array/buffer.

\param maxListSize The number of elements in list arrays.

\return The number of elements in \b pContainerList filled in.

Get the list of objects inside the container.
Returns the number of elements in \b pObjectList filled in.

Used for: Object.  
*/
    virtual uint32 GetContainedObjects(HOBJECT hContainer,
        HOBJECT *pObjectList, uint32 maxListSize) = 0;

/*!
\param  pAddr   IP address to target; can be numeric or host name.
\param  nPort   Port to use
\return \b LT_NOTINITIALIZED if there is no TCP/IP driver or the driver's socket is uninitialized, or \b LT_ERROR if the address cannot be built; otherwise, returns \b LT_OK.

Open socket to address.

Used for: Networking
*/
    virtual LTRESULT OpenSocket( uint32 *phSocket ) = 0;


/*!
\param hLight the light object to perform the operation on
\param fIntensity the intensity scale of the light, should be from [0..1]
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the intensity scale of the light which can be used to control how bright the light
is. This will effect all appropriate colors of the light.

Used for: Light Control.
*/
	virtual LTRESULT	SetLightIntensityScale(HOBJECT hLight, float fIntensity) = 0;
/*!
\param hLight the light object to perform the operation on
\param fIntensity the intensity scale of the light, should be from [0..1]
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the intensity scale of the light which can be used to control how bright the light
is. This will effect all appropriate colors of the light.

Used for: Light Control.
*/
	virtual LTRESULT	GetLightIntensityScale(HOBJECT hLight, float& fIntensity) = 0;
/*!
\param hLight the light object to perform the operation on
\param pszTexture the texture associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the texture associated with the specified light

Used for: Light Control.
*/
	virtual LTRESULT	SetLightTexture(HOBJECT hLight, const char* pszTexture) = 0;
/*!
\param hLight the light object to perform the operation on
\param pszTexture the texture associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the texture associated with the specified light

Used for: Light Control.
*/
	virtual LTRESULT	GetLightTexture(HOBJECT hLight, char* pszTexture, uint32 nBufferLen) = 0;
/*!
\param hLight the light object to perform the operation on
\param pszTexture the texture associated with the light's attenuation
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the attenuation texture associated with the specified light that controls how the light
falls off away from a light (Directional lights only)

Used for: Light Control.
*/
	virtual LTRESULT	SetLightAttenuationTexture(HOBJECT hLight, const char* pszTexture) = 0;
/*!
\param hLight the light object to perform the operation on
\param pszTexture the texture associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the attenuation texture associated with the specified light that controls how the light
falls off away from a light (Directional lights only)

Used for: Light Control.
*/
	virtual LTRESULT	GetLightAttenuationTexture(HOBJECT hLight, char* pszTexture, uint32 nBufferLen) = 0;
/*!
\param hLight the light object to perform the operation on
\param vTransColor the translucent color associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the translucent color associated with the light. The value will be clamped from 0..1
and is quantized to 255 positions, so it should not be interpolated.

Used for: Light Control.
*/
	virtual LTRESULT	SetLightTranslucentColor(HOBJECT hLight, const LTVector& vTransColor) = 0;

/*!
\param  hObject     Object to detach what it is standing on

This will cause the provided object to detach whatever it is standing on so that it
will force it to reevaluate physics the next update loop.

Used for: Object.
*/
    virtual LTRESULT DetachObjectStanding(HLOCALOBJ hObject) = 0; 


/*!
\param  pQuery     Information about the intersection to perform
\param  pInfo      Results of the intersection if it hit something

Performs a ray intersection and returns the results. Will return true if anything
was hit, false otherwise

Used for: Object.
*/
	virtual bool IntersectSegment(const IntersectQuery& Query, IntersectInfo *pInfo) = 0;

/*!
\param  pQuery     Information about the intersection to perform
\param  pInfo      Results of the intersection if it hit something
\param	pObject	   The object to perform the ray intersection against

Performs a ray intersection against the specified object only, and will perform filtering and returning
of collision results in the same manner as IntersectSegment

Used for: Object.
*/
	virtual bool IntersectSegmentAgainst(const IntersectQuery& Query, IntersectInfo *pInfo, HOBJECT hObject) = 0;

/*!
\param hLight the light object to perform the operation on
\param vTransColor the translucent color associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the translucent color associated with the light. The value will be clamped from 0..1
and is generated from 255 quantized positions, so it should not be interpolated.

Used for: Light Control.
*/
	virtual LTRESULT	GetLightTranslucentColor(HOBJECT hLight, LTVector& vTransColor) = 0;
/*!
\param hLight the light object to perform the operation on
\param vSpecularColor the specular color associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the specular color associated with the light. The value will be clamped from 0..1
and is quantized to 255 positions, so it should not be interpolated.

Used for: Light Control.
*/
	virtual LTRESULT	SetLightSpecularColor(HOBJECT hLight, const LTVector& vSpecularColor) = 0;
/*!
\param hLight the light object to perform the operation on
\param vSpecularColor the specular color associated with the light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the specular color associated with the light. The value will be clamped from 0..1
and is generated from 255 quantized positions, so it should not be interpolated.

Used for: Light Control.
*/
	virtual LTRESULT	GetLightSpecularColor(HOBJECT hLight, LTVector& vSpecularColor) = 0;
/*!
\param hLight the light object to perform the operation on
\param vDims the new dimensions for the directional light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the directional dimensions of the directional light. These dimensions are measured in half
dimensions in the X/Y, and full dimensions in the Z. This creates a box that is projected
down the forward of the object from the position.

Used for: Light Control.
*/
	virtual LTRESULT	SetLightDirectionalDims(HOBJECT hLight, const LTVector& vDims) = 0;
/*!
\param hLight the light object to perform the operation on
\param vDims the new dimensions for the directional light
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the directional dimensions of the directional light. These dimensions are measured in half
dimensions in the X/Y, and full dimensions in the Z. This creates a box that is projected
down the forward of the object from the position.

Used for: Light Control.
*/
	virtual LTRESULT	GetLightDirectionalDims(HOBJECT hLight, LTVector& vDims) = 0;
/*!
\param hLight the light object to perform the operation on
\param fFovX the X field of view of the light (in radians)
\param fFovY the Y field of view of the light (in radians)
\param fNearClip the distance along the forward vector of the object for the near clip plane
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Changes the parameters used for rendering a spot projector light. This includes the field of view
of the projected area and also the clipping planes for the area it casts onto

Used for: Light Control.
*/
	virtual LTRESULT	SetLightSpotInfo(HOBJECT hLight, float fFovX, float fFovY, float fNearClip) = 0;
/*!
\param hLight the light object to perform the operation on
\param fFovX the X field of view of the light (in radians)
\param fFovY the Y field of view of the light (in radians)
\param fNearClip the distance along the forward vector of the object for the near clip plane
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the parameters used for rendering a spot projector light. This includes the field of view
of the projected area and also the clipping planes for the area it casts onto

Used for: Light Control.
*/
	virtual LTRESULT	GetLightSpotInfo(HOBJECT hLight, float& fFovX, float& fFovY, float& fNearClip) = 0;
/*!
\param eLightEnable The LOD at which this light will light the world and objects
\param eWorldShadows The LOD at which this light will cast shadows from the world
\param eObjectShadows The LOD at which this light will cast shadows from objects
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Sets the level of detail settings associated with the light

Used for: Light Control.
*/
	virtual LTRESULT	SetLightDetailSettings(HOBJECT hLight, EEngineLOD eLightEnable, EEngineLOD eWorldShadows, EEngineLOD eObjectShadows) = 0;
/*!
\param eLightEnable The LOD at which this light will light the world and objects
\param eWorldShadows The LOD at which this light will cast shadows from the world
\param eObjectShadows The LOD at which this light will cast shadows from objects
\return LT_OK if the specified object is valid and a light, LT_INVALIDPARAMS otherwise

Gets the level of detail settings associated with the light

Used for: Light Control.
*/
	virtual LTRESULT	GetLightDetailSettings(HOBJECT hLight, EEngineLOD& eLightEnable, EEngineLOD& eWorldShadows, EEngineLOD& eObjectShadows) = 0;
/*!
\param pPoint Location to test.
\param pList Array of \b HOBJECT
\param maxListSize Length of the pList array.
\return The number of containers filled in.

Find out what containers contain the given point.

Used for: Misc.
*/
    virtual uint32  GetPointContainers(const LTVector *pPoint, HOBJECT *pList, 
        uint32 maxListSize) = 0;

/*!
\param pMsg \b message string.

Called to display the string out to the console.

Used for: Console.
*/
virtual void    CPrintNoArgs(const char *pMsg) = 0;

/*!
\param pMsg \b printf style format string and variable parameter list.

Called to display the provided formatted text out to the console

Used for: Console.
*/
    virtual void    CPrint(const char *pMsg, ...) = 0;

/*!
\param pszVarName The name of the console variable that wishes to be obtained

Called to obtain a handle to a console variable given the variable's textual name. This will
return NULL upon failure.

Used for: Console
*/
	virtual HCONSOLEVAR	GetConsoleVariable(const char* pszVarName) = 0;

/*!
\param pszCommand The command to execute for the console

Called to execute the provided string just as if it had been entered into the console
directly by the user

Used for: Console
*/
	virtual LTRESULT	RunConsoleCommand(const char* pszCommand) = 0;

/*!
\param hVar Handle of console variable.
\return The floating point value of a console variable, 
or 0 if the \b hVar is \b NULL. 

Used for: Console.
*/
    virtual float GetConsoleVariableFloat(HCONSOLEVAR hVar) = 0;

/*!
\param pszName The name of the console variable to set.
\param fValue The value to assign to the provided console variable
\return The LT_OK if hVar is valid, otherwise LT_INVALIDPARAMS

Called to set the value of the provided console variable to the specified floating point value

Used for: Console.
*/
    virtual LTRESULT SetConsoleVariableFloat(const char* pszVarName, float fValue) = 0;

/*!
\param hVar Handle of console variable.
\return The string value of a console variable, or \b NULL if the \b hVar is \b NULL.

Used for: Console.
*/
    virtual const char* GetConsoleVariableString(HCONSOLEVAR hVar) = 0;

/*!
\param hVar Handle of console variable.
\param pszValue The value to assign to the provided console variable
\return The LT_OK if hVar and pszValue are valid, otherwise LT_INVALIDPARAMS

Called to set the value of the provided console variable to the specified string value

Used for: Console.
*/
    virtual LTRESULT SetConsoleVariableString(const char* pszVarName, const char* pszValue) = 0;


/*!
\return The current time, in seconds, since the shell started.

Used for: Misc.
*/
    virtual LTDOUBLE GetTime() = 0;
        
/*!
\return The current time, in milliseconds, since the shell started.

Used for: Misc.
*/
	virtual uint32 GetTimeMS() = 0;


/*!
Gets the object's current unguaranteed data value

Used for: Object/networking.
*/
	virtual LTRESULT GetObjectUnguaranteedData(HLOCALOBJ hObj, uint32 *pData) = 0;

/*!
\return The time since the last shell update.

Used for: Misc.
*/
    virtual LTFLOAT GetFrameTime() = 0;

/*!

  The worlds are offset at processing time to be centered around the origin. This offset
  can be found by calling this function. This is useful for mapping position values back
  to the original level values for debugging and error reporting

\param vVector The vector offset from the original level. To get the orginal value use the equation orignal = position + offset
\return Always returns LT_OK-

  Used for: Misc.
*/

	virtual LTRESULT GetSourceWorldOffset(LTVector& vVector) = 0;

/*!
\param hObj ???

Remove the object from the world.  

\note The object will not be removed or deleted until the end of the frame.

Used for: Object.
*/
    virtual LTRESULT RemoveObject(HOBJECT hObj) = 0;

/*!
\param pName Name of the sector

Function to generate the appropriate sector ID given the name of the sector.
\note	Sector names are case-insensitive.

Used for: Visibility control
*/
	virtual LTRESULT GetSectorID(const char *pName, uint32 *pResult) const = 0;

/*!
\param hObj Object to link to
\param pRef LTObjRef object to add to the object's reference list

This function adds a reference link to the object's list, to be called when the
object gets deleted.
\note	This function is called internally by the LTObjRef class.  Do not call this function externally!
*/
	virtual LTRESULT LinkObjRef(HOBJECT hObj, LTObjRef *pRef) = 0;


/*!
\param nNum Index of blind data.

\param nId ID of blind data (will fail if doesn't match).

\param pData A pointer to the blind data is placed here.

\param nSize Size of blind data is placed here.

\return Returns LTTRUE on success.

Retrieve a pointer to a section of blind data
Get a pointer to a section of blind data.  Blind data
should be freed with FreeBlindObjectData after it has been used.

Used for: Misc.
*/
	virtual LTRESULT GetBlindObjectData(uint32 nNum, uint32 nId, uint8*& pData, uint32& nSize) = 0;

/*!
\param tSkyCamera [out] This is the returned transform of the sky camera

\return Returns LT_OK

This will obtain the current sky camera transform that has been set on the server
and is being used as the camera position and relative orientation for rendering
the sky

Used for: Misc
*/
	virtual LTRESULT GetSkyCameraTransform(LTRigidTransform& tSkyCamera) = 0;

/*!
\param pBuffer		 Pointer to data for the stream.
\param nBufferLength Amount of data.
\param bCopyData	 Flag indicating if the data should be copied or referenced.
\param ppStream		 Address of pointer to an \b ILTInStream that gets set.

Opens an ILTInStream on an existing in-memory buffer.

Used for: Misc.
*/
	virtual LTRESULT OpenStreamFromBuffer(uint8* pData, const uint32 nDataSize, bool bCopyData, ILTInStream** ppStream) = 0;

/*!
\param  nSourceBufferSize        The size of the source buffer that will be compressed using CompressBuffer().
\param  nCompressedBufferMaxSize The maximum destination buffer size necessary to store data compressed with CompressBuffer().
\return \b LT_ERROR if a problem occurred; otherwise, returns \b LT_OK.

Gets the required buffer size for a subsequent call to CompressBuffer.

Used for: Misc
*/
	virtual LTRESULT GetCompressedBufferMaxSize(const uint32 nSourceBufferSize, uint32& nCompressedBufferMaxSize) = 0;

/*!
\param  pSourceData			  Data to compress.
\param  nSourceDataLen		  Amount of data pointed to by pData.
\param  pCompressedBuffer	  Target buffer for compressed data.
\param  nCompressedBufferSize Allocated size of target buffer.
\param  nCompressedSize		  Size of the data in its compressed form.
\return \b LT_ERROR if a problem occurred during compression; otherwise, returns \b LT_OK.

Compress a buffer.

Used for: Misc
*/
	virtual LTRESULT CompressBuffer(const uint8* pSourceData, const uint32 nDataLen, const uint8* pCompressedBuffer, const uint32 nCompressedBufferSize, uint32& nCompressedSize) = 0;

/*!
\param  pSourceData			    Data to decompress.
\param  nSourceDataLen		    Amount of data pointed to by pData.
\param  pDecompressedBuffer	    Target buffer for decompressed data.
\param  nDecompressedBufferSize Allocated size of target buffer.
\param  nDecompressedSize		Size of the data in its decompressed form.
\return \b LT_ERROR if a problem occurred during decompression; otherwise, returns \b LT_OK.

Decompress a buffer.

Used for: Misc
*/
	virtual LTRESULT DecompressBuffer(const uint8* pSourceData, const uint32 nDataLen, const uint8* pDecompressedBuffer, const uint32 nDeompressedBufferSize, uint32& nDecompressedSize) = 0;

/*!
\param  pCRCValue	The total CRC value for the current world assets
\return \b LT_ERROR if a problem occurred; otherwise, returns \b LT_OK.

Get the CRC value for all assets associated with the current world.

Used for: Misc
*/
	virtual LTRESULT GetWorldCRC(uint32& nWorldCRC) = 0;

};


#endif //! __ILTCSBASE_H__



