#ifndef __LTBASEDEFS_H__
#define __LTBASEDEFS_H__

#ifndef __STDLIB_H__
#include <stdlib.h>
#define __STDLIB_H__
#endif

#ifndef __STRING_H__
#include <string.h>
#define __STRING_H__
#endif

#ifndef __STDIO_H__
#include <stdio.h>
#define __STDIO_H__
#endif

#ifndef __ENGINE_H__
#include "engine.h"
#endif

#ifndef __LTCODES_H__
#include "ltcodes.h"
#endif

#ifndef __ILTMEMORY_H__
#include "iltmemory.h"
#endif

#ifndef __LTMEMUTILS_H__
#include "ltmemutils.h"
#endif

#ifndef __LTSTRUTILS_H__
#include "ltstrutils.h"
#endif

#ifndef __LTSTLUTILS_H__
#include "ltstlutils.h"
#endif

#ifndef __LTGUID_H__
#include "ltguid.h"
#endif

#ifndef __LTENUM_H__
#include "ltenum.h"
#endif

#ifndef __LTBOOL_H__
#include "ltbool.h"
#endif

#define BEGIN_EXTERNC() extern "C" \
    {
#define END_EXTERNC() };

class ILTBaseClass;
#define LPBASECLASS ILTBaseClass*
class IAggregate;
#define LPAGGREGATE IAggregate*


//This will determine the size of an array assuming it has at least a single element
#define LTARRAYSIZE(a)		(sizeof(a) / (sizeof((a)[0])))

//This macro should be placed into the private space of a class that should not be copied. This macro
//will ensure that the object will produce either a compile time error when a copy constructor or
//assignment operator is called from external sources when placed in the private portion of a class,
//and will produce either a runtime or link time error when placed in the public scope or accessed
//by friend or the object itself (assuming that the functions aren't actually implemented elsewhere)
#define PREVENT_OBJECT_COPYING(Class) \
	Class(const Class&); \
	Class& operator=(const Class&)

//Template function that will handle checking the passed in parameter for validity, and if
//valid will call release on the pointer and set it to null. This is basically a templatized
//version of the common safe release macro
template <class T>
void LTSafeRelease(T*& pInterfacePtr)
{
	if(pInterfacePtr)
	{
		pInterfacePtr->Release();
		pInterfacePtr = NULL;
	}
}

//This macro should be used in functions that have unused parameters.  Using this macro
//tells the reader that the code is aware the parameter is unused and that is the intent.
#define LTUNREFERENCED_PARAMETER(p)		(p)



//Given four charachters, this will pack it into a uint32 with the first characters being in 
//the lowest bits. This is very useful for file guards and other disk operations. This is
//identical to the windows MAKEFOURCC macro, but lets us avoid needing to include a large number
//of headers to get only a single macro
inline uint32 LTMakeFourCC(char ch0, char ch1, char ch2, char ch3)
{
	return	(((uint32)ch0) << 0)		| 
			(((uint32)ch1) << 8)		|  
			(((uint32)ch2) << 16)		| 
			(((uint32)ch3) << 24);
}

/*!
Used in GetFileList/FreeFileList.
*/
enum ELTFileEntryType
{
	eLTFileEntryType_File,
	eLTFileEntryType_Directory,
};

struct FileEntry 
{
    ELTFileEntryType	m_Type;            //! Directory or file?
    const char*			m_pBaseFilename;   //! pic1.dds
    const char*			m_pFullFilename;   //! interface/bitmaps/pic1.dds
    FileEntry*			m_pNext;		   //! Linked list traversal
};

/*!
Maximum number of child models per model.
*/
#define MAX_CHILD_MODELS 8

/*!
Maximum number of nodes for a model.
*/
#define MAX_MODEL_NODES 128

/*!
Maximum number of materials a model can have
*/
#define MAX_MATERIALS_PER_MODEL	16

//! The most trackers that can be added to a single model.
const uint32 MAX_TRACKERS_PER_MODEL = 16;

#define DEFINE_HANDLE_TYPE(name) \
    typedef struct {int __nothing;} name##_t, *name;


#define MAX_CS_FILENAME_LEN 127

/*!
HMESSAGE defines
*/

/*!
Maximum number of bytes in a communications message.
*/
#define MAX_PACKET_LEN      1100

/*!
Model state flags.
*/

//! The (nonlooping) model has finished playing the animation.
#define MS_PLAYDONE     1

/*!
Gets r, g, and b as 0-255 integers.
*/
inline uint32 GETA(uint32 val)								{ return (val >> 24); }
inline uint32 GETR(uint32 val)								{ return ((val >> 16) & 0xFF); }
inline uint32 GETG(uint32 val)								{ return ((val >> 8) & 0xFF); }
inline uint32 GETB(uint32 val)								{ return (val & 0xFF); }

/*!
r, g, and b, are 0-255 integers.
*/
inline uint32 SETRGB(uint8 r, uint8 g, uint8 b)				{ return (((uint32)r << 16) | ((uint32)g << 8) | ((uint32)b)); }
inline uint32 SETRGBA(uint8 r, uint8 g, uint8 b, uint8 a)	{ return (((uint32)a << 24) | ((uint32)r << 16) | ((uint32)g << 8) | ((uint32)b)); }

/*!
Handle definitions.  It defines them in this weird way so that
you can't accidentally cast between them.
*/
DEFINE_HANDLE_TYPE(HCLASS)
DEFINE_HANDLE_TYPE(HLTSOUND)
DEFINE_HANDLE_TYPE(HCLIENT);
DEFINE_HANDLE_TYPE(HATTACHMENT);
DEFINE_HANDLE_TYPE(HNETSERVICE);

/*!
This is used all over to classify which side of a polygon something's on.
*/
typedef enum {
    BackSide=0,
    FrontSide=1,
    Intersect=2
} PolySide;

/*!
The different types that an engine light can be
*/
enum EEngineLightType 
{
	//invalid light. This has not been initialized and is not rendered
	eEngineLight_Invalid,

	//point light. Equal color emitted in all directions around a point
	eEngineLight_Point,

	//same as a point light, but doesn't allow shadows or specular lighting. This allows rendering
	//of these lights to be many times more efficient than point lights
	eEngineLight_PointFill,

	//directional light. Volume of directional light described an oriented bounding box that projects
	//a texture and shadows orthogonally from the source
	eEngineLight_Directional,

	//spot projector. Directional light targeted at a certain direction
	eEngineLight_SpotProjector,

	//cube projector. Projects a cubic environment map in a sphere around the light.
	eEngineLight_CubeProjector,

	//black light. Spot projector for representing the special black light
	eEngineLight_BlackLight,

	//the number of engine light types. This must come last
	eEngineLight_NumLightTypes
};

/*!
An enumeration that contains different values for level of detail settings. This can be used
for such things as controlling at what level of detail an object casts a shadow, etc.
*/
enum EEngineLOD
{
	//indicates that this is applicable to low or higher (essentially always)
	eEngineLOD_Low,

	//indicates that this will only show up in medium and high detail settings
	eEngineLOD_Medium,

	//indicates that this will only show up in high detail setting
	eEngineLOD_High,

	//indicates that this should be disabled and never show up
	eEngineLOD_Never,

	//the number of LOD types, this must come last
	eEngineLOD_NumLODTypes
};

/*!
The maximum length a class name can be for game objects
*/
#define MAX_OCS_CLASSNAME_LEN   64

/*!
User-assigned identifier used to reference anim trackers in API calls.
Used with SMP networking where the engine creates and manages tracker
instances.
*/
typedef uint8 ANIMTRACKERID;
//! Reserved ID for the main tracker which all models have.
const ANIMTRACKERID MAIN_TRACKER = 0xFF;

/*!
Render modes are what are used to describe a video mode/video card.
*/
struct RMode
{
//!	Does this mode support hardware TnL?
    bool m_bHWTnL;

//! The name of the adapter (rendering HW or service) to use
    char m_DeviceName[128];

//! The dimensions and bit depth of this mode
    uint32 m_Width, m_Height, m_BitDepth;

//! The next mode in the list
    RMode *m_pNext;
};

/*!
Structure used by a driver to describe a net service (such as ipx or tcp/ip).
*/
#define MAX_NETSERVICENAME_LEN  128
class NetService
{
public:
    HNETSERVICE m_handle;
    uint32 m_dwFlags; //! Combination of NETSERVICE_ flags.
    LTGUID m_guidService;
    char m_sName[MAX_NETSERVICENAME_LEN];
    NetService *m_pNext;
};




/*!
Structure used by a driver to describe a specific net session.
*/
#define MAX_NETSESSIONNAME_LEN  4096
#define MAX_HOSTIP_LEN          32
#define NO_HOST_PORT            0xFFFF

class NetSession {
public:
    NetSession() {Clear();}
    virtual ~NetSession() {}

    void Clear() {
        m_dwFlags = 0;
        m_dwMaxConnections = 0;
        m_dwCurConnections = 0;
        m_Ping = 0;
        m_sName[0] = 0;
        m_bHasPassword = false;
        m_HostIP[0] = 0;
        m_HostPort = NO_HOST_PORT;
        m_pNext = NULL;
		m_nGameType = 0;
    }

    uint32 m_dwFlags;
    LTGUID m_guidApp;
    LTGUID m_guidInst;
    uint32 m_dwMaxConnections;
    uint32 m_dwCurConnections;
    uint32 m_Ping; //! Ping time in milliseconds.

/*!
Host info.  0 length string and NO_HOST_PORT if not on TCP/IP.
*/
    wchar_t m_HostIP[MAX_HOSTIP_LEN];
    uint32 m_HostPort;

    wchar_t m_sName[MAX_NETSESSIONNAME_LEN];
	bool m_bHasPassword;
	uint8 m_nGameType;
    NetSession *m_pNext;
};

/*!
Structure used by to instruct a driver to create/host a new session.
*/
struct NetHost {
    uint32 m_Port; //! Port if TCP/IP.  If zero, it uses the default.
	wchar_t m_sBindToAddr[MAX_HOSTIP_LEN]; // specifies the IP address to bind to.  If empty, binds to all
    uint32 m_dwFlags;
    uint32 m_dwMaxConnections;
    wchar_t m_sName[MAX_NETSESSIONNAME_LEN];
    bool m_bHasPassword;
	uint8 m_nGameType;
};

/*!
Ping status results
*/

enum {
	PING_STATUS_UNKNOWN = 0, // The ping ID queried was unknown
	PING_STATUS_SUCCESS = 1, // The ping was successfully processed
	PING_STATUS_WAITING = 2, // The ping has not been responded to
	PING_STATUS_TIMEOUT = 3, // The ping timed out
};

/*!
Used to determine information in GetStandingOn or GetLastCollision.
*/
struct CollisionInfo
{
/*!
The blocking plane of whatever the object collided with.  If both
object's are non-solid, then this will have a normal vector of mag 0.
*/
    LTPlane m_Plane;

/*!
The object collided with.
*/
    HOBJECT m_hObject;

/*!
If the m_hObject is the world, then m_hPoly contains a handle to the
polygon the object collided with, otherwise it is equal to NULL.
*/
    HPOLY m_hPoly;

/*!
Stopping velocity.  Engine will automatically apply this velocity to stop
object from continuing to collide with blocker.
*/
    LTVector m_vStopVel;
};


// Use the new sound system in 3.1
#ifndef USE_ABSTRACT_SOUND_INTERFACES
#define USE_ABSTRACT_SOUND_INTERFACES 1
#endif


/*!
Sound 3D Provider

Used with GetSoundSW3DProviderList, GetSoundHW3DProviderList and
ReleaseSound3DProviderList.
*/



/*!
Sound 3D provider IDs.
*/

enum
{
    
    SOUND3DPROVIDERID_NONE                   =  0,
    SOUND3DPROVIDERID_DS3D_HARDWARE          =  1,
    SOUND3DPROVIDERID_DS3D_SOFTWARE          =  2,
	SOUND3DPROVIDERID_DS3D_DEFAULT			 =  3,
    SOUND3DPROVIDERID_UNKNOWN                =  4
};


/*!
Caps bits.
*/

enum
{
    SOUND3DPROVIDER_CAPS_REVERB        = (1<<0)
};

/*!
Contains information about a 3D sound provider.

\see GetSound3DProviderLists(), ReleaseSound3DProviderList()
*/
struct Sound3DProvider {
/*!
One of the SOUND3DPROVIDERID_xxx values. Many of the commonly used 3D providers have been identified with one of these values.  If the provider doesn't have a unique ID, it will be set to SOUND3DPROVIDERID_UNKNOWN. If this is the case, it can be identified using the m_szProvider parameter. Compare the m_szProvider string with the known provider names listed in the Miles Sound System documentation.
*/
    uint32 m_dwProviderID;

/*!
A string descriptor identifying the 3D Sound Provider. To use a 3D sound provider, you must copy this string into the InitSoundInfo structure when initializing the sound engine.
*/
    char m_szProvider[_MAX_PATH + 1];

/*!
A combination of any of the SOUND3DPROVIDER_CAPS_xxxx flags. You can use this to identify some of the features of the 3D provider.
*/
    uint32 m_dwCaps;

/*!
A pointer to the next 3D sound provider in the linked list. A NULL value indicates the end of the list. To step through all providers, keep accessing the Sound3Dprovider::m_pNextProvider member variable until Sound3DProvider::m_pNextProvider is equal to NULL.
*/
    Sound3DProvider *m_pNextProvider;
};

/*!
Reverb properties.

Use this structure with SetReverb and GetReverb.
*/

enum
{
//! m_fVolume field valid
    REVERBPARAM_VOLUME            =  (1<<0),

//! m_dwAcoustics field valid
    REVERBPARAM_ACOUSTICS         =  (1<<1),

//! m_fReflectTime field valid
    REVERBPARAM_REFLECTTIME       =  (1<<2),

//! m_fDecayTime field valid
    REVERBPARAM_DECAYTIME         =  (1<<3),

//! m_fDamping field valid
    REVERBPARAM_DAMPING           =  (1<<4),

    REVERBPARAM_ALL               =  0x1F
};




/*!
These are the valid values for m_dwAcoustics field
*/
enum {
    REVERB_ACOUSTICS_GENERIC,
    REVERB_ACOUSTICS_PADDEDCELL,
    REVERB_ACOUSTICS_ROOM,
    REVERB_ACOUSTICS_BATHROOM,
    REVERB_ACOUSTICS_LIVINGROOM,
    REVERB_ACOUSTICS_STONEROOM,
    REVERB_ACOUSTICS_AUDITORIUM,
    REVERB_ACOUSTICS_CONCERTHALL,
    REVERB_ACOUSTICS_CAVE,
    REVERB_ACOUSTICS_ARENA,
    REVERB_ACOUSTICS_HANGAR,
    REVERB_ACOUSTICS_CARPETEDHALLWAY,
    REVERB_ACOUSTICS_HALLWAY,
    REVERB_ACOUSTICS_STONECORRIDOR,
    REVERB_ACOUSTICS_ALLEY,
    REVERB_ACOUSTICS_FOREST,
    REVERB_ACOUSTICS_CITY,
    REVERB_ACOUSTICS_MOUNTAINS,
    REVERB_ACOUSTICS_QUARRY,
    REVERB_ACOUSTICS_PLAIN,
    REVERB_ACOUSTICS_PARKINGLOT,
    REVERB_ACOUSTICS_SEWERPIPE,
    REVERB_ACOUSTICS_UNDERWATER,
    REVERB_ACOUSTICS_DRUGGED,
    REVERB_ACOUSTICS_DIZZY,
    REVERB_ACOUSTICS_PSYCHOTIC,

    REVERB_ACOUSTICS_COUNT           //! total number of room types
};

struct ReverbProperties {

//! Set the params bits for which fields are valid
    uint32 m_dwParams;

//! 0.0 - 1.0
    float m_fVolume;

//! One of the REVERB_ACOUSTICS_xxx values
    uint32 m_dwAcoustics;

//! 0.0 - 5.0 Time for first reflection
    float m_fReflectTime;

//! 0.1 - 20.0 Determines how quickly reflection diminish
    float m_fDecayTime;

/*!
0.0 - 2.0, == 1.0f is even damping, < 1.0f low frequencies dampen faster;
> 1.0f high frequencies dampen faster
*/
    float m_fDamping;
};

/*!
Sound effects.

The types of sounds available are ambient, local and 3D.  The flags
controlling these types are mutually exclusive.  An ambient sound will
have distance effects of rolloff, but no orientation effects.  A local
sound will have no orientation or distance effects, and will seem as
if it is played inside the player's head.  The 3D sound will have
distance, orientation and doppler effects.

Sounds are played from the beginning once the message reaches the
client.  If it is important that the playback be synchronized with the
server's beginning time, then set PLAYSOUND_TIMESYNC.  Normally, this
is not important.  The server will use it internally if a client comes
in after a sound has already been played.

The server keeps a sound object if any of the following flags are set:
PLAYSOUND_LOOP, PLAYSOUND_ATTACHED, PLAYSOUND_GETHANDLE, PLAYSOUND_TIMESYNC,
PLAYSOUND_TIME. Server-kept sounds will update clients that come and go.
Non-server-kept sounds are sent to the existing clients once, so the
overhead is much less.

Server-kept sounds with PLAYSOUND_GETHANDLE must be removed by the game.
Other server-kept sounds are removed if they time out, or the object they
are attached to is removed.  When a sound is removed from the server,
it tells the clients to remove their copies.

The server tells clients about its sounds if the client object is
within twice the outer radius of the sound.  If it is a local sound,
then the server always tells all of the clients.

Sounds that have a client object in m_hObject and PLAYSOUND_CLIENTLOCAL
or PLAYSOUND_ATTACHED are played with PLAYSOUND_LOCAL for that particular client.

The PLAYSOUND_CLIENT is for client-initiated sounds only.  When playing
client side sounds, the PLAYSOUND_ATTACHED and PLAYSOUND_CLIENTLOCAL
flags and m_wObjectID are ignored.

*/


enum
{
/*!
Play sound locally (inside head). Mutually exclusive with
\b PLAYSOUND_AMBIENT and \b PLAYSOUND_3D. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_LOCAL       =  0x0000,

/*!
Play sound as ambient sound. Mutually exclusive with \b PLAYSOUND_LOCAL
and \b PLAYSOUND_3D. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_AMBIENT     =  0x0001,

/*!
Play sound as 3D sound. Mutually exclusive with \b PLAYSOUND_AMBIENT
and \b PLAYSOUND_LOCAL. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_3D          =  0x0002,

/*!
Loop the sound. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_LOOP        =  0x0004,

/*!
Sound's position and orientation comes from object in the \b PlaySoundInfo structure's
\b m_hObject member variable. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_ATTACHED    =  0x0008,

/*!
Handle requested. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_GETHANDLE   =  0x0010,

/*!
Server must time sound. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_TIME        =  0x0020,

/*!
Control volume (using the \b PlaySoundInfo structure's \b m_nVolume member variable).
Used with the \b PlaySoundInfo structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_CTRL_VOL    =  0x0040,

/*!
Control pitch (using the \b PlaySoundinfo structure's \b m_nPitch member variable). Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_CTRL_PITCH  =  0x0080,

/*!
Allow reverb. Used with the \b PlaySoundInfo structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_REVERB      =  0x0100,

/*!
Client side sound. Used with the \b PlaySoundInfo structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_CLIENT      =  0x0200,

/*!
Sound is played with \b PLAYSOUND_LOCAL for the object identified by the \b PlaySoundInfo
structure's \b m_hObject member variable. Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_CLIENTLOCAL =  0x0800,

/*!
This sound will only be played once (reduces memory overhead). Used with the \b PlaySoundInfo
structure's \b m_dwFlags member variable.
*/
    PLAYSOUND_ONCE        =  0x1000,

/*
 This sound will have occlusion applied to it when determined its 3d volumes.
 This flag should be used with 3d and maybe ambient, but should not be used
 with local sounds, since they wouldn't be occluded anyways.. 
 */
	PLAYSOUND_USEOCCLUSION = 0x2000,

/*
 This indicates that the sound has a path descriptor that allows
 it to move independent of any object. Useful for ambients and
 sounds not tied to objects (first use was bullet flybys since
 we don't actually have bullets moving through the world).
 */
	PLAYSOUND_USEPATH = 0x4000,

/*
This sound will have occlusion applied to it when determined its 3d volumes.
However, the occlusion will not be applied to any surface inside its inner radius.
This flag should be used with 3d and maybe ambient, but should not be used
with local sounds, since they wouldn't be occluded anyways.. 
*/
	PLAYSOUND_USEOCCLUSION_NO_INNER_RADIUS = 0x8000,

	/*
	This sound has a 2nd sound for use outside a certain radius. 
	*/
	PLAYSOUND_USE_RADIUSBASED_SOUND = 0x010000,

	/*
	This sound should not be stopped when heading to the FE 
	*/
	PLAYSOUND_FE_PERSISTENT_SOUND = 0x020000,

	/*
	This sound allows dynamic fade control, and exists for optimization
	purposes. Without the flag, you can still fade to 0 and stop, but that's
	it..
	*/
	PLAYSOUND_ALLOW_FADE_CONTROL = 0x040000,
};


/*
 *	This Enum describes the default sound mixer channels
 */
enum
{
	PLAYSOUND_MIX_DEFAULT = 1,
	PLAYSOUND_MIX_MUSIC,
	PLAYSOUND_MIX_WEAPONS_PLAYER,
	PLAYSOUND_MIX_WEAPONS_NONPLAYER,
	PLAYSOUND_MIX_FOOTSTEPS_PLAYER,
	PLAYSOUND_MIX_FOOTSTEPS_NONPLAYER,
	PLAYSOUND_MIX_AMBIENT,
	PLAYSOUND_MIX_PHYSICS,
	PLAYSOUND_MIX_WEAPON_IMPACTS,
	PLAYSOUND_MIX_SPEECH,
	PLAYSOUND_MIX_OBJECTS,
	PLAYSOUND_MIX_UI,

};


/*!
Defines a variety of options for sounds. The options you choose can drastically change
the performance of the sound rendering. This structure must be populated for each sound
to be played.
To initialize the \b PlaySoundInfo struct to default values, use the \b PLAYSOUNDINFO_INIT
macro.
*/
struct PlaySoundInfo {
/*!
A combination of any of the \b PLAYSOUND_xxxx flags. \b PLAYSOUND_LOCAL,
\b PLAYSOUND_AMBIENT and \b PLAYSOUND_3D are mutually exclusive.
*/
    uint32 m_dwFlags;

/*!
A string containing the path to the sound to play.
*/
    char m_szSoundName[_MAX_PATH + 1];

/*!
The server-side object to which the sound will be attached. This parameter must be
filled out if \b PLAYSOUND_ATTACHED or \b PLAYSOUND_CLIENTLOCAL is set in the \b
\m_dwFlags.
*/
    HOBJECT m_hObject;

/*!
This parameter is filled out by the \b ILTSoundMgr::PlaySound() function if the
\b PLAYSOUND_GETHANDLE is set. The game can use this handle with other \b ILTSoundMgr
functions that require a handle to the sound.
*/
    HLTSOUND m_hSound;



/*!
The priority of the sound. Zero is the lowest priority. The engine uses this priority
to decide which sounds get rendered when all the voices have been taken.
*/
    unsigned char m_nPriority;

/*!
For Ambient or 3D sounds. Clients outside the outer radius will not hear this sound.
Clients within the inner radius will hear the sound at full volume. Clients between
the inner and outer radius will hear an interpolated volume.
*/
    float m_fOuterRadius;



/*!
For Ambient or 3D sounds. Clients outside the outer radius will not hear this sound.
Clients within the inner radius will hear the sound at full volume. Clients between
the inner and outer radius will hear an interpolated volume.
*/
    float m_fInnerRadius;

/*!
The maximum volume of this sound. This value can range between 0 and 100. This value
is ignored unless \b PLAYSOUND_CTRL_VOLUME is set.
*/
    uint8 m_nVolume;

/*!
This is a multiplier to the playback frequency of the sound, which will shift its pitch.
If you had a sound recorded at 22,000 Hz and set its pitch shift to 1.1f, then the
output sound would be 24,2000 Hz and make the sound have a higher pitch. This value
is ignored unless \b PLAYSOUND_CTRL_PITCH is set.
*/
    float m_fPitchShift;

/*!
The 3D position of the sound in world coordinates. This is only needed for Ambient or 3D
sounds and if \b PLAYSOUND_ATTACHED is not used.
*/
    LTVector m_vPosition;

/*!
Game defined user data. The 32bit value will be passed to \b IClientShell::OnPlaySound().\
*/
    uint32 m_UserData;

	/*!
	Sound type.  This is used with functions to allow different classes of sounds to be 
	played with different volume ranges
	*/
	uint8 m_nSoundVolumeClass;

	/*!
	This is the mix channel to use for the sound. Allows different
	sound categories to have their volumes adjusted on the fly..
	*/
	int16 m_nMixChannel;

	/*!
	 This is the path velocity to use if the PLAYSOUND_USEPATH is set.
	 The velocity indicates that the sound will move at this velocity
	 for the duration of its play.
	 */
	LTVector m_vPathVelocity;

	/*!
	This is the doppler factor. Only needed if it's a 3D sound.
	Doppler factor controls how much the velocity affects doppler
	*/
	float m_fDopplerFactor;

	/*!
	A string containing the path to the alternate (outer radius) sound to play.
	Only usable if PLAYSOUND_USE_RADIUSBASED_SOUND is set.
	*/
	char m_szAlternateSoundName[_MAX_PATH + 1];

	/*!
	For 3D sounds. Radius outside of which the alternate sound file will
	be played instead.   Only usable if PLAYSOUND_USE_RADIUSBASED_SOUND is set.
	*/
	float m_fSoundSwitchRadius;
};

#define PLAYSOUNDINFO_INIT(x) memset(&x, 0, sizeof(x));

/*!
	SoundMixerInfo - info about the sound mixer. gets filled in on the client..
*/

#define	FIRST_MIXER_CHANNEL	1
#define NUM_MIXER_CHANNELS	18
#define	MAX_MIXER_CHANNELS	NUM_MIXER_CHANNELS+FIRST_MIXER_CHANNEL

struct SoundMixerSliders
{
	float fVolumeSettings[MAX_MIXER_CHANNELS];
	float fLowPassSettings[MAX_MIXER_CHANNELS];
	float fEffectSettings[MAX_MIXER_CHANNELS];
	float fPitchSettings[MAX_MIXER_CHANNELS];
	float fOcclusionFactorSettings[MAX_MIXER_CHANNELS];
};

struct SoundMixerInfo 
{
	SoundMixerSliders Sliders;

	uint32 nFadeInTime;
	uint32 nPriority;
	bool   bTempMixer;
	int32  nLayer;
	uint32 nMixerTime;
	uint32 nFadeOutTime;
};

#define OCCLUSION_STRING_MAXLENGTH	100
#define OCCLUSION_USE_MATERIAL		255
#define OCCLUSION_MAX_OCCLUSION_TYPES 256

typedef struct 
{
	char pcName[OCCLUSION_STRING_MAXLENGTH];
	int32 nID;
	uint32 color;
	char pcDescription[OCCLUSION_STRING_MAXLENGTH];
} OcclusionSurfaceEditInfo;

typedef void (DEditGetOcclusionInfoFunc)(OcclusionSurfaceEditInfo* aSurfaceList, uint32* pnNumSurfaces,
																  const uint32 nMaxSurfaces, const uint32 cMaxStringLength);


/*!
InitSoundInfo flags

Used to initialize the sound engine.
*/

enum
{
/*!
Convert all 16-bit sounds to 8-bit sounds when loaded
from disk. Used for the \b m_dwFlags member variable of the InitSoundInfo structure.
*/
    INITSOUNDINFOFLAG_CONVERT16TO8  = (1<<0),

/*!
This tells \b ILTClientSoundMgr::InitSound() to keep track of
any sounds that exist before the InitSound call and reload them after the sound engine
is initialized. This is the best way to reinitialize the sound engine.
Used for the \b m_dwFlags member variable of the InitSoundInfo structure.
*/
    INITSOUNDINFOFLAG_RELOADSOUNDS  = (1<<1)
};

/*!
Engine can fill these flags in the m_dwResults parameter
*/
enum
{
/*!
Indicates that the 3D provider identified by the
\b m_sz3DProvider parameter provides reverb. Used with the \b m_dwResults member
variable of the InitSoundInfo structure.
*/
    INITSOUNDINFORESULTS_REVERB    = (1<<0),
};


/*!
Use this to start a game.
*/

enum
{

//! Start a world and host it
    STARTGAME_HOST,

//! Connect as a client of a remote server.
    STARTGAME_CLIENT,

//! Start a normal singleplayer game.
    STARTGAME_NORMAL,

//! (Used for GetGameMode, means we're not running a world or on a server yet).
    GAMEMODE_NONE,
};


#define MAX_SGR_STRINGLEN   100

class StartGameRequest
{
public:
	
	enum
	{
		kInvalidSocket = -1,
	};

public:
    StartGameRequest() 
	{
        m_Type = STARTGAME_NORMAL;
        m_WorldName[0]	= '\0';
        m_TCPAddress[0] = '\0';
        m_pGameInfo		= NULL;
        m_GameInfoLen = 0;
        m_pNetSession	= NULL;
        m_pClientData	= NULL;
        m_ClientDataLen = 0;
		m_nSocket = ( uint32 )kInvalidSocket;
    }

    int m_Type;
    char m_WorldName[MAX_SGR_STRINGLEN];

	// If specified, then it will join on the socket instead of creating a new one.
	uint32 m_nSocket;

//! TCP/IP address, if any.
    char m_TCPAddress[MAX_SGR_STRINGLEN];

//! This must be used with STARTGAME_CLIENT.
    NetSession *m_pNetSession;

//! This must be used with STARTGAME_HOST.
    NetHost m_HostInfo;

/*!
This data is copied over and can be accessed by
the server with ServerDE::GetGameInfo() (if you're
running a local or hosted game).
*/
    void *m_pGameInfo;
    uint32 m_GameInfoLen;

/*!
This data gets sent up and passed into OnClientEnterWorld on the server.
*/
    void *m_pClientData;
    uint32 m_ClientDataLen;
};

/*!
Parsing stuff.
*/
#define PARSE_MAXTOKENS     64
#define PARSE_MAXTOKENSIZE  96



/*!
The new console parsing thing.
*/
class ConParse {
public:
	ConParse() {m_pCommandPos = NULL;}
	ConParse(const char *pBuffer) {m_pCommandPos = pBuffer;}

	/*!
	Sets it up to parse the specified buffer.
	*/
	void Init(const char *pBuffer) {m_pCommandPos = pBuffer;}

	/*!
	The parsed arguments.
	*/
	char *m_Args[PARSE_MAXTOKENS];
	int m_nArgs;

	/*!
	Used internally by the engine.
	*/
public:

	/*!
	Parse the next set of tokens.

	\return true if it parsed anything.
	*/
	bool Parse();

	/*!
	Parses until it finds tokens with pLookFor as the first one.
	skipCount says how many to skip.
	You can use this just like Parse like this:


	while(ParseFind("AmbientLight", false)) { ... }
	*/
	bool ParseFind(char *pLookFor, bool bCaseSensitive, uint32 minTokens=1);

private:
	const char *m_pCommandPos;
	char m_ArgBuffer[PARSE_MAXTOKENS*PARSE_MAXTOKENSIZE];
};

/*!
The Unicode parsing thing.
*/
class ConParseW {
public:
	ConParseW() {m_pCommandPos = NULL;}
	ConParseW(const wchar_t *pBuffer) {m_pCommandPos = pBuffer;}

	/*!
	Sets it up to parse the specified buffer.
	*/
	void Init(const wchar_t *pBuffer) {m_pCommandPos = pBuffer;}

	/*!
	The parsed arguments.
	*/
	wchar_t *m_Args[PARSE_MAXTOKENS];
	int m_nArgs;

	/*!
	Used internally by the engine.
	*/
public:

	/*!
	Parse the next set of tokens.

	\return true if it parsed anything.
	*/
	bool Parse();

	/*!
	Parses until it finds tokens with pLookFor as the first one.
	skipCount says how many to skip.
	You can use this just like Parse like this:


	while(ParseFind("AmbientLight", false)) { ... }
	*/
	bool ParseFind(wchar_t *pLookFor, bool bCaseSensitive, uint32 minTokens=1);

private:
	const wchar_t *m_pCommandPos;
	wchar_t m_ArgBuffer[PARSE_MAXTOKENS*PARSE_MAXTOKENSIZE];
};


template<class T>
class BaseObjArray {
public:

    BaseObjArray() {
        m_pArray = NULL;
        m_MaxListSize = 0;
        m_nElements = 0;
    }

    inline bool AddObject(const T& obj) {
        if (m_nElements < m_MaxListSize) {
            m_pArray[m_nElements] = obj;
            m_nElements++;
            return true;
        }

        return false;
    }

    inline T GetObject(uint32 index) {
        ASSERT(index < m_nElements);
        return m_pArray[index];
    }

    inline void Reset() {
        m_nElements = 0;
    }

    inline uint32 NumObjects() {
        return m_nElements;
    }

protected:
    T       *m_pArray;
    uint32 m_MaxListSize;

/*!
Number of valid objects in m_pList.
*/
    uint32 m_nElements;
};

template<class T, int size>
class ObjArray : public BaseObjArray<T> {
public:
    ObjArray() {
        m_pArray = m_Array;
        m_MaxListSize = size;
    }

private:
    T m_Array[size];
};

/*!
Object flags.
*/
enum
{
    FLAG_VISIBLE =					(1<<0),

/*!
LT normally compresses the position and rotation info
to reduce packet size.  Some things must be exact
(like some WorldModels) so this will
enlarge the packets for better accuracy.
*/
    FLAG_FULLPOSITIONRES =			(1<<1),

/*!
Just use the object's color and global light scale.
(Don't affect by area or by dynamic lights).
*/
    FLAG_NOLIGHT =					(1<<2),
    
/*!
Uses minimal network traffic to represent rotation
(1 byte instead of 3, but only rotates around the Y axis).

Mutually exclusive with FLAG_FULLPOSITIONRES (FLAG_FULLPOSITIONRES
forces full uncompressed rotation distribution, and FLAG_YROTATION
is ignored)
*/
    FLAG_YROTATION =				(1<<3),

//! Object is hit by raycasts.
    FLAG_RAYHIT =					(1<<4),

//! Object can't go thru other solid objects.
    FLAG_SOLID =					(1<<5),

//! Use simple box physics on this object (used for WorldModels and containers).
    FLAG_BOXPHYSICS =				(1<<6),

//! This object is solid on the server and nonsolid on the client.
    FLAG_CLIENTNONSOLID =			(1<<7),

//! This object's visibility should be delayed on the client (used by the prediction).
	FLAG_DELAYCLIENTVISIBLE =		(1<<8),

//! This object will not be placed into the world tree, meaning that it can't be rayhit, collected
//! by find objects in sphere, or collide with other objects. 
	FLAG_NOTINWORLDTREE =			(1<<9),

//! Object is touchable when other objects move into it, but doesn't touch objects when it moves.
	FLAG_TOUCHABLE =	            (1<<10),

//! Will get an MID_AFFECTPHYSICS message for each object that is inside its volume.
	FLAG_CONTAINER =		        (1<<11),

//! Remove this object automatically if it gets outside the world.
	FLAG_REMOVEIFOUTSIDE =			(1<<12),
};

/*!
Which flags the client knows about.
*/
#define CLIENT_FLAGMASK (FLAG_VISIBLE| \
                         FLAG_FULLPOSITIONRES|FLAG_NOLIGHT|\
						 FLAG_YROTATION|FLAG_RAYHIT|FLAG_CONTAINER|\
                         FLAG_SOLID|FLAG_BOXPHYSICS|FLAG_CLIENTNONSOLID|\
						 FLAG_TOUCHABLE|FLAG_DELAYCLIENTVISIBLE|\
						 FLAG_NOTINWORLDTREE|FLAG_REMOVEIFOUTSIDE)


/*! 
This indicates all of the available flags for modification.  (Useful for SetObjectFlags.)
*/
#define FLAGMASK_ALL (0xFFFFFFFF)

/*!
Server only flags.
*/
//*
//! Gets touch notification.
#define FLAG_TOUCH_NOTIFY       (1<<16)

//! Gravity is applied.
#define FLAG_GRAVITY            (1<<17)

//! Object can pass through world
#define FLAG_GOTHRUWORLD        (1<<18)

//! Don't follow the object this object is standing on.
#define FLAG_DONTFOLLOWSTANDING (1<<19)

//Use this whenever possible as it saves cycles.

//! Object won't slide agaist polygons
#define FLAG_NOSLIDING          (1<<20)

//Uses much (10x) faster physics for collision detection, but the
//object is a point (dims 0,0,0).  Standing info is not set when
//this flag is set.
#define FLAG_POINTCOLLIDE       (1<<21)
//*/

//! The object won't get get MID_MODELSTRINGKEY messages unless it sets this flag.
#define FLAG_MODELKEYS          (1<<22)

//! Force client updates even if the object is OT_NORMAL or invisible.
#define FLAG_FORCECLIENTUPDATE  (1<<23)

/*!
If you clear these flags (flags &= ~FLAG_OPTIMIZEMASK) the engine never even iterates over 
the object for movement, raycasting or movement.  Use this whenever you can.
*/
#define FLAG_OPTIMIZEWORLDTREEMASK (FLAG_SOLID|FLAG_TOUCH_NOTIFY|FLAG_TOUCHABLE|FLAG_RAYHIT|FLAG_CONTAINER)

/*!
FLAG2_ defines.
*/
enum
{
// WARNING: only the first 16 bits of FLAG2 are distributed to the
// client.  If you add a new flag in the high 16 bits, the client will
// never hear about it!

//! Use a y-axis aligned cylinder to collide with the BSP.
    FLAG2_PLAYERCOLLIDE =			(1<<0),

/*!
Don't render this object thru the normal stuff,
only render it when processing sky objects.
*/
    FLAG2_SKYOBJECT =				(1<<1),

/*!
!forces an object to be considered translucent
during rendering. This should be used when
an object has parts that are translucent, but
overall the alpha is completely opaque.
*/
    FLAG2_FORCETRANSLUCENT =		(1<<2),

/*!
Disables the client-side prediction on this object
*/
	FLAG2_DISABLEPREDICTION =		(1<<3),

/*!
Enables transferring of dims to the client
*/
	FLAG2_SERVERDIMS		=		(1<<4),

/*!
Indicates that objects with this flag are non-solid to other objects with this flag
*/
	FLAG2_SPECIALNONSOLID	=		(1<<5),

/*!
Indicates that objects with this flag should use rigid body simulation
*/
	FLAG2_RIGIDBODY			=		(1<<6),

/*!
Indicates that this object should be simulated physically on the client and not adhere
to any server simulation data.
*/
	FLAG2_CLIENTRIGIDBODY	=		(1<<7),

/*!
This object behaves as a normal sky object, but should be rendered after the scene
has been drawn on top to allow it to overlay on top of the screen. Typically this is used
for effects such as blinding flares or lens flares in the sky
*/
	FLAG2_SKYOVERLAYOBJECT =		(1<<8),

//! Steps up stairs.  This flag is ignored unless FLAG2_PLAYERCOLLIDE is set.
	FLAG2_PLAYERSTAIRSTEP   =		(1<<9),

};


/*!
Different object types.  Some can only be created on the client.
*/
enum EObjectType
{
//! Invisible object.  Note, client's aren't told about
    OT_NORMAL =           0,

//! Model object.
    OT_MODEL =            1,

//! WorldModel.
    OT_WORLDMODEL =       2,

//! Dynamic light.
    OT_LIGHT =            3,

//! Camera.
    OT_CAMERA =           4,

//! Container.
    OT_CONTAINER =        5,

//! Custom Render Object
	OT_CUSTOMRENDER =	  6,

/*!
NOTE: the high bit of the object type is reserved
for the engine's networking.
ALSO NOTE: The networking assumes there are < 16 object types!
*/
    NUM_OBJECTTYPES =     7,
};

/*!
Intersection stuff. Eventually this should be moved into its own include file.
*/

/*!
Intersect flags. Multiple of these can be combined for a single intersection call.
*/
enum
{
	//! Try to hit objects.  There is a small speed hit by doing so.
	INTERSECT_OBJECTS				= (1<<0),

	//! Ignore nonsolid objects and nonsolid surfaces. Just a helper filter.
	IGNORE_NONSOLID					= (1<<1),

	//! Fill in the \b HPOLY for this intersection (this slows it down a little).
	INTERSECT_HPOLY					= (1<<2),

	//! If a segment starts within an object it will not detect this unless this flag is set
	CHECK_FROM_POINT_INSIDE_OBJECTS = (1<<3),
};

/*!
\return true to select this object and false to not select it.
*/
typedef bool (*ObjectFilterFn)(HOBJECT hObj, void *pUserData);
typedef bool (*PolyFilterFn)(HPOLY hPoly, void *pUserData, const LTVector& vIntersectPoint);

/*!
Pass this in to the IntersectSegment routine.
*/
class IntersectQuery {
public:

	IntersectQuery() {
		m_Flags = 0;
		m_FilterFn = 0;
		m_PolyFilterFn = 0;
		m_FilterActualIntersectFn = 0;
		m_pActualIntersectUserData = NULL;
		m_pUserData = NULL;

		m_From.Init();
		m_To.Init();
	}

	/*!
	Start and end points.
	*/
	LTVector m_From;
	LTVector m_To;

	/*!
	A combination of the intersect flags (in ltcodes.h).
	*/
	uint32 m_Flags;

	/*!
	If this is not NULL, then it will call this function when it has a
	POSSIBLE object intersection (it doesn't know if it really intersects
	yet when it calls this).  If you return false from this function,
	then it will ignore the object and continue on.
	*/
	ObjectFilterFn m_FilterFn;

	/*!
	If this is not NULL, then it will call this function when it has an
	ACTUAL object intersection.  If you return false from this function,
	then it will ignore the object and continue on.
	*/
	ObjectFilterFn m_FilterActualIntersectFn;

	/*!
	If this is not NULL, then it will call this function when it has a
	poly intersection. If you return false from this function,
	then it will ignore the poly and continue on.
	*/
	PolyFilterFn m_PolyFilterFn;

	/*!
	Passed into pUserData of m_Filter function.
	*/
	void *m_pUserData;

	/*!
	Passed into pUserDaga of m_FilterActualIntersectFn function
	*/

	void *m_pActualIntersectUserData;
};


struct IntersectInfo {
	IntersectInfo() {
		m_Point.Init();
		m_Plane.m_Normal.Init();
		m_Plane.m_Dist = 0.0f;
		m_hObject = NULL;
		m_hPoly = INVALID_HPOLY;
		m_SurfaceFlags = 0;
	}

	/*!
	Point of intersection.
	*/
	LTVector m_Point;

	/*!
	Plane of intersection.
	*/
	LTPlane m_Plane;

	/*!
	Object it hit.
	*/
	HOBJECT m_hObject;

	/*!
	The polygon it hit (if it's a world poly).
	Value is INVALID_HPOLY if it didn't hit one.
	*/
	HPOLY m_hPoly;

	/*!
	Surface flags of what it hit (these aren't object flags, and are
	only set if it hit the world or a WorldModel).
	*/
	uint32 m_SurfaceFlags;
};

//this determines the number of bits needed to encode the specified number. This inclusive
//version includes the number provided (so FNumBitsInclusive<8> = 4)
template <unsigned int NUMBER>
struct FNumBitsInclusive
{
	enum { k_nValue = (FNumBitsInclusive<NUMBER / 2>::k_nValue + 1) };
};

template<>
struct FNumBitsInclusive<0>
{
	enum { k_nValue = 0 };
};

template<>
struct FNumBitsInclusive<1>
{
	enum { k_nValue = 1 };
};

//this determines the number of bits needed to encode the specified number. This exclusive
//version doesn't include the number provided (so FNumBitsInclusive<8> = 3)
template <unsigned int NUMBER>
struct FNumBitsExclusive
{
	enum { k_nValue = (FNumBitsInclusive<NUMBER - 1>::k_nValue) };
};

template<>
struct FNumBitsExclusive<0>
{
	enum { k_nValue = 0 };
};

#endif  //! __LTBASEDEFS_H__
