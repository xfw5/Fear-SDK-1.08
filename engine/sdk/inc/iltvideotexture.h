//------------------------------------------------------------------------------------
// ILTVideoTexture
//
// Defines the interface used for controlling playback of videos on textures. This
// allows for the creation of videos, changing the way they are played back, and
// querying the status of the videos. 
//
//------------------------------------------------------------------------------------

#ifndef __ILTVIDEOTEXTURE_H__
#define __ILTVIDEOTEXTURE_H__

#ifndef __LTBASEDEFS_H__
#	include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

//for HENGINETIMER
#ifndef __ILTTIMER_H__
#	include "ilttimer.h"
#endif

//define a handle to a video texture
class CVideoTexture;
typedef CVideoTexture* HVIDEOTEXTURE;

//flags that are to be used with the video textures to specify behavior
enum EVideoTextureOption
{
	//this indicates that the movie can be streamed from disk if conditions are right
	eVTO_AllowStreaming			= (1<<0),

	//this indicates that mip maps should be generated for the video texture
	eVTO_MipMaps				= (1<<1),

	//setting this flag disables skipping video frames to catch up to the timing or audio
	eVTO_DisableFrameSkip		= (1<<2),

	//indicates that this video should only play once and then stop when it gets to the end. If
	//this is not specified, it will loop continuously.
	eVTO_PlayOnce				= (1<<3),

	//indicates that this video can play sound. 
	eVTO_PlaySound				= (1<<4),
	
	//indicates that when this video is not visible, it should automatically pause
	eVTO_PauseWhenNotVisible	= (1<<5),
};


/*!
The ILTVideoTexture provides access to all video textures and allows manipulation of
them.

Define a holder to get this interface like this:
\code
define_holder(ILTVideoTexture, your_var);
\endcode
*/

class ILTVideoTexture :
	public IBase
{
public:

	interface_version(ILTVideoTexture, 0);
	virtual ~ILTVideoTexture()			{}

/*!
\param pszFilename		The filename of the video texture that is to be opened.
\param nOptions			The options to associate with the video texture. This should be a combination of the eVTO_ flags above.
\return					The new video texture, or NULL on failure

Creates a new video texture from the specified file and flags. This will return the video texture to the user
so that it can be used with other systems. Note that this must be released with ReleaseVideoTexture.

Used For: Video Textures
*/
	virtual HVIDEOTEXTURE	CreateVideoTexture(const char* pszFilename, uint32 nOptions) = 0;

/*!
\param pszFilename		The filename of the video texture that is to be found.
\return					The video texture, or NULL on failure

This will search for any currently open video textures of the specified file name and return a handle to it as
long as it is not marked as unique. This is useful for obtaining access to videos that are playing in the level.
This handle must be released with ReleaseVideoTexture.

Used For: Video Textures
*/
	virtual HVIDEOTEXTURE	FindVideoTexture(const char* pszFilename) = 0;

/*!
\param hVideoTexture	The video texture to release the reference to
\return					LT_OK or LT_INVALIDPARAMS if the handle is invalid.

Releases a reference to a video texture. The reference passed into this function should not be used after
this call is made.

Used For: Video Textures
*/
	virtual LTRESULT		ReleaseVideoTexture(HVIDEOTEXTURE hVideoTexture) = 0;

/*!
\param hVideoTexture	The video texture to add a reference to
\return					LT_OK or LT_INVALIDPARAMS if the handle is invalid.

Adds an additional reference to the specified video texture

Used For: Video Textures
*/
	virtual LTRESULT		AddVideoTextureRef(HVIDEOTEXTURE hVideoTexture) = 0;

/*!
\param hVideoTexture	The video texture to get the dimensions of
\param vnVideo			The dimensions of the actual video
\param vnTexture		The dimensions of the texture the video is displayed on
\return					LT_OK or LT_INVALIDPARAMS if the handle is invalid.

Obtains the dimensions of the video and the texture that it is displayed to for the associated video
texture.

Used For: Video Textures
*/
	virtual LTRESULT		GetVideoTextureDims(HVIDEOTEXTURE hVideoTexture, LTVector2n& vnVideo, LTVector2n& vnTexture) = 0;

/*!
\param hVideoTexture	The video texture to pause
\param bPause			Flag indicating whether to pause or unpause the video
\return					LT_OK or LT_INVALIDPARAMS if the handle is invalid.

Either pauses or unpauses the specified video texture. Pause is handled through counting, so two calls
to Pause(true) need to be have two calls to Pause(false) before the video will play again.

Used For: Video Textures
*/
	virtual LTRESULT		PauseVideoTexture(HVIDEOTEXTURE hVideoTexture, bool bPause) = 0;

/*!
\param hVideoTexture	The video texture to restart
\return					LT_OK or LT_INVALIDPARAMS if the handle is invalid.

This will restart the video to the beginning. This will not change any of the state such as whether
or not it is paused.

Used For: Video Textures
*/
	virtual LTRESULT		RestartVideoTexture(HVIDEOTEXTURE hVideoTexture) = 0;

/*!
\param hVideoTexture	The video texture to determine if it is finished
\param bFinished		Flag indicating whether or not the video texture is finished
\return					LT_OK or LT_INVALIDPARAMS if the handle is invalid.

This will determine if the provided video texture is finished. This will always return false
unless the video was created with the eVTO_PlayOnce option set, in which case it will return
true when the last frame is hit.

Used For: Video Textures
*/
	virtual LTRESULT		IsVideoTextureFinished(HVIDEOTEXTURE hVideoTexture, bool& bFinished) = 0;

/*!
\param hVideoTexture	The video texture to associate the provided timer with
\param hTimer			The engine timer that will be used to control the playback speed of the video
\return					LT_OK if the video texture is valid, LT_INVALIDPARAMS otherwise

This associates the provided timer with the specified video texture so that it can be used to
control the playback rate of the video. This can be NULL if the video texture is supposed to use
the simulation timer (which is the default for videos). 

Used For: Video Textures
*/
	virtual LTRESULT		SetVideoTextureTimer(HVIDEOTEXTURE hVideoTexture, HENGINETIMER hEngineTimer) = 0;

/*!
\param bPause			Flag indicating whether to pause or unpause the videos
\return					LT_OK.

This will run through all existing videos and pause them or unpause them. It is important to note that
only created videos will be paused or unpaused, and therefore if a video is created after this call,
it will not be properly paused.

Used For: Video Textures
*/
	virtual LTRESULT		PauseAllVideos(bool bPause) = 0;
};

#endif
