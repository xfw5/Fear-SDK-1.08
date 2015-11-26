//-------------------------------------------------------------------
//
//   MODULE    : ILTRENDERER.H
//
//   PURPOSE   : Defines engine interface ILTRenderer
//
//   CREATED   : On 8/11/00 At 10:40:21 AM
//
//-------------------------------------------------------------------

#ifndef __ILTRENDERER_H__
#define __ILTRENDERER_H__

#ifndef __LTBASEDEFS_H__
#	include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

#ifndef __ILTVIDEOTEXTURE_H__
#	include "iltvideotexture.h"
#endif

//Forward declarations
class CRendererFrameStats;


/*!
Render target options. These should be used when constructing a render target to specify its behavior as
a collection of bit flags. Note that many of them are exclusive.
*/
enum ERenderTargetOption
{
	//! Specifies that the render target should be a cube map instead of a single 2d texture.
	eRTO_CubeMap				= (1<<0),

	//! Specifies that the render target should have automaticlly generated mip maps. If the device
	//! doesn't support this or this is not specified, there will be no mip maps.
	eRTO_AutoGenMipMaps			= (1<<1),

	//! Specifies that the render target should use a depth buffer when rendering
	eRTO_DepthBuffer			= (1<<2),

	//! Specifies that the render target should support rendering of 'Current frame effect' materials
	eRTO_CurrentFrameEffect		= (1<<3),

	//! Specifies that the render target should support rendering of 'Previous frame effect' materials
	eRTO_PreviousFrameEffect	= (1<<4),

	//! Specifies that the render target should support rendering of fog volumes
	eRTO_FogVolume				= (1<<5),

	//! Specifies that this render target refers to the screen. This is for internal usage only.
	eRTO_ScreenInternal			= (1<<6),
	
	//! Specifies that this render target has a floating-point precision, single-channel color buffer
	eRTO_FloatBuffer			= (1<<7),

	//! Specifies that the depth buffer should be accessible as a texture when rendering
	eRTO_DepthBufferTexture		= (1<<8),

	//! Specifies that the shadow buffer buffer should be accessible as a texture when rendering
	eRTO_ShadowBuffer			= (1<<9),

	//!Force the enumeration to be a 32 bit integer
	eRTO_Force32Bit				= (0x7FFFFFFF)
};

/*!
An enumeration of the different faces of a cubic environment map. Each one is defined as the normal of
the plane as seen from the outside of the cube.
*/
enum ECubeTextureFaces
{
	eCTF_PositiveX = 0,
	eCTF_NegativeX = 1,
	eCTF_PositiveY = 2,
	eCTF_NegativeY = 3,
	eCTF_PositiveZ = 4,
	eCTF_NegativeZ = 5,

	//the number of cube map faces, this must come last
	eCTF_CubeMapFaces
};


/*!
Screen clear flags. These are flags that can be passed to any clear operations such as clearing
the current render target or the current viewport.
*/
enum
{
	//! Clear the color components of the render target
	CLEARRTARGET_COLOR =	(1<<0),

	//! Clear the depth buffer associated with the current render target
	CLEARRTARGET_ZBUFFER =	(1<<1),

	//! Clear the stencil buffer associated with the current render target
	CLEARRTARGET_STENCIL =	(1<<2),

	//! Clear all of the above
	CLEARRTARGET_ALL =		(CLEARRTARGET_COLOR | CLEARRTARGET_ZBUFFER | CLEARRTARGET_STENCIL),
};

//! Prototype for a callback function that will be called prior to deleting a model decal
typedef void (*ModelDecalDeleteFn)(HMODELDECAL hDecal, void *pUser);

//! Handle to a render target object which can be used to control the output of rendering
class CRenderTarget;
typedef CRenderTarget*	HRENDERTARGET;

//! Handle to a render target visibility query, which wraps a render target object and allows
//! determination of when the render target vis query was used to render the render target
class CRenderTargetVisQuery;
typedef CRenderTargetVisQuery*	HRENDERTARGETVIS;

/*!
The ILTRenderer provides access to all renderer related tasks.

Define a holder to get this interface like this:
\code
define_holder(ILTRenderer, your_var);
\endcode
*/

class ILTRenderer :
	public IBase
{
public:

	interface_version(ILTRenderer, 0);


/*!
 
\return Pointer to RMode Linked list structure containing the valid render modes.

Get a \b NULL - terminated list of supported render modes.

\see GetRenderMode()
\see SetRenderMode()

Used for: Rendering.
*/
    virtual RMode* GetRenderModes() = 0;

/*!
\param pModes An array of \b pMode variables to relinquish.

Relinquish list of supported render modes.

Used for: Rendering.  
*/
    virtual void RelinquishRenderModes(RMode *pModes) = 0;


/*!
\param nWidth (return) Width of the current render target in pixels
\param nHeight (return) Width of the current render target in pixels
\return LT_OK if successful or LT_ERROR if the screen is not yet initialized

Determines the dimensions of the currently active render target in pixels

Used for: Rendering.
*/
    virtual LTRESULT GetCurrentRenderTargetDims(uint32& nWidth, uint32& nHeight) = 0;

/*!
\param  nClearFlags Flags to use when performing the clear screen (a combination of 
                    \b CLEARSCREEN_ flags).
\param  ClearColor  Color to clear the screen to. 
\return \b LT_OK or \b LT_NOTINITIALIZED.

Provides access to clearing all of the current render target

Used for: Rendering.
*/
    virtual LTRESULT ClearRenderTarget(uint32 nClearFlags, uint32 ClearColor) = 0;


/*!
\return true if the device is currently in 3d, or false otherwise.

Determines if the application is within a 3d rendering block.

\note You must be in a Start3D()/End3D() block in order to render
through any cameras.

\see Start3D(), End3D()

Used for: Rendering.
*/
	virtual bool IsIn3D() = 0;

/*!
\return \b LT_OK or \b LT_NOTINITIALIZED or \b LT_ALREADYIN3D.

Start a Start3D()/End3D() block.

\note You must be in a Start3D()/End3D() block in order to render
through any cameras.

\see End3D()

Used for: Rendering.
*/
    virtual LTRESULT Start3D() = 0;

/*!  
\return \b LT_OK, \b LT_NOTINITIALIZED, or \b LT_NOTIN3D.

End a Start3D/End3D block.

\note You must be in a Start3D()/End3D() block in order to render
through any cameras.

\see Start3D()

Used for: Rendering.
*/
    virtual LTRESULT End3D() = 0;


/*!
\return \b LT_OK or \b LT_NOTINITIALIZED.

Indicates that rendering of the current frame is completed, and displays the contents of
the currently displayed back buffer.

Used for: Rendering.
*/
    virtual LTRESULT FlipScreen() = 0;

/*!
\param  pFrameStats     The frame stats object that will receive all the frame stats for scenes
						rendered. This can be NULL to disassociate any current frame stats.
\return \b LT_OK

Called to change the frame stats object that will receive any frame statistics as scenes
are rendered.

Used for: Rendering.
*/

	virtual LTRESULT SetFrameStats(CRendererFrameStats* pFrameStats) = 0;

/*!
\param  pFrameStats     The pointer to receive the current frame stats object
\return \b LT_OK

Called to obtain the frame stats object that will receive any frame statistics as scenes
are rendered.

Used for: Rendering.
*/

	virtual LTRESULT GetFrameStats(CRendererFrameStats*& pFrameStats) = 0;


/*!
\param TableIndex		The index into the depth bias table to set the parameters for
\param SlopeScaleBias	The amount to scale the current slope relative to the screen (in Z space)
\param DepthBias		The amount to shift the depth values of the pixels (in Z space)

Called to set the depth bias information in the depth bias table so that any objects indexing
into this same index will have their depths biased accordingly. These values are in Z space, so
-0.5 moves it forwards half the Z buffer, etc. Note that the table index 0 cannot be changed. This
will return false if zero is provided or the specified index is out of range of the table.

Used for: Depth biasing
*/
	virtual LTRESULT SetDepthBiasTableIndex(uint32 nIndex, float fSlopeScale, float fDepthBias) = 0;

/*!
\param hObject		The object to set the depth bias table index of
\param nIndex		The index to set for the provided index

Given an object, this will set the depth bias table index for that object so that it will use
the associated depth bias settings when rendering. Zero disables this functionality and this will
have no effect if the specified index is out of range.

Used for: Depth biasing
*/
	virtual LTRESULT SetObjectDepthBiasTableIndex(HOBJECT hObject, uint32 nTableIndex) = 0;

/*!
\param hCamera		specifies the camera that the point will be projected onto
\param vWorldPos	specifies the point that will be projected onto the camera's viewing plane
\param vScreenPos	the output of the operation. X,Y are the normalized screen coordinates, 
					Z is the distance along the Z axis in camera space (the depth of the object in the screen)

\return The return value will indicate if there was any critical issue, such as not providing a valid camera object. If this is not LT_OK, the output will always be 0,0,0.

Called to determine where on the screen a specified point in world space will appear, and at what
depth. The returned value is in normalized device coordinates, where 0,0 is the upper left, and 1,1
is the bottom right of the screen. When the depth of the point is close to zero, the projection may become unstable,
and it is the callers responsibility to handle this case by checking the resulting Z return value to determine
if it is in a range that is usable for it. Remember that a -Z will project onto the screen plane, but mirrored
over the X and Y screen axis.

*/

	virtual LTRESULT WorldPosToScreenPos(	HOBJECT hCamera, const LTVector& vWorldPos, LTVector& vScreenPos) = 0;

/*!
\param hCamera		specifies the camera that will project the point into space
\param vScreenPos	the point on the screen to project into space
\param fDepth		the depth to extrude that point outwards from the camera. This distance is measured along
					the line through the point, NOT the distance along the camera's forward
\param vWorldPos	The resulting position of the point in world space position

\return The return value will indicate if there was any critical issue, such as not providing a valid camera object. If this is not LT_OK, the output will always be 0,0,0.

Called to project through a point on the screen a certain distance and return the world space position of that
point. This can be useful for handling situations such as the user clicking on the screen and needing to do a
ray cast through that point. This could be then used to project a segment out along a certain length to determine
which direction the ray should be projected.

*/
	virtual LTRESULT ScreenPosToWorldPos(HOBJECT hCamera, const LTVector2& vScreenPos, float fDepth, LTVector& vWorldPos) = 0;

/*!
\param  hCamera     The camera to render.
\return \b LT_OK, \b LT_NOTINITIALIZED, or \b LT_NOTIN3D.

Render from the position of the camera into its rectangle on the current render target.

Used for: Rendering.
*/
    virtual LTRESULT RenderCamera(HLOCALOBJ hCamera, const char *pTechniqueOverride) = 0;
	virtual LTRESULT RenderCamera(	const LTRigidTransform& tCamera, const LTVector& vInitialSectorPos, 
									const LTVector2& vFOV, bool bOrthogonal, const LTRect2f& rViewport, 
									const LTPlane* pClipPlane, const char *pTechniqueOverride) = 0;

/*!
\param  hCamera     The camera to render.
\param  pObjects    An array of objects to render.
\param  nObjects    The size of the \b pObjects array.
\return \b LT_OK, \b LT_NOTINITIALIZED, or \b LT_NOTIN3D.

Renders the list of objects through the given camera to the current render target.

Used for: Rendering.
*/
    virtual LTRESULT RenderObjects(HLOCALOBJ hCamera, HLOCALOBJ *pObjects, int nObjects, const char *pTechniqueOverride) = 0;

/*!
Aliases for the rendering functions for rendering without a technique override
*/
	virtual LTRESULT RenderCamera(HLOCALOBJ hCamera) 
		{ 
			return RenderCamera(hCamera, NULL); 
		}
	virtual LTRESULT RenderCamera(	const LTRigidTransform& tCamera, const LTVector& vInitialSectorPos, 
									const LTVector2& vFOV, const LTRect2f& rViewport, 
									const LTPlane* pClipPlane) 
		{ 
			return RenderCamera(tCamera, vInitialSectorPos, vFOV, rViewport, pClipPlane, NULL); 
		}
	virtual LTRESULT RenderCamera(	const LTRigidTransform& tCamera, const LTVector& vInitialSectorPos, 
									const LTVector2& vFOV, const LTRect2f& rViewport, 
									const LTPlane* pClipPlane, const char *pTechniqueOverride)
		{
			return RenderCamera(tCamera, vInitialSectorPos, vFOV, false, rViewport, pClipPlane, NULL); 
		}
	virtual LTRESULT RenderObjects(HLOCALOBJ hCamera, HLOCALOBJ *pObjects, int nObjects) 
		{ 
			return RenderObjects(hCamera, pObjects, nObjects, NULL); 
		}

/*!
\param  hCamera			The camera to use as a position for the generation of the cubic environment map
\param  nSize			The number of pixels on a side of the final output environment map
\param  pszFile			The filename for the cubic environment map that will be created
\return \b LT_OK, \b LT_NOTINITIALIZED, or \b LT_NOTIN3D.

Given the specified position of the camera it will generate a cubic environment map in the scene

Used for: Rendering.
*/
    virtual LTRESULT MakeCubicEnvMap(const LTVector& vCenter, uint32 nSize, const char* pszFile) = 0;

/*!
\param  pszFile			The filename for the screenshot that will be created
\return \b LT_OK, \b LT_ERROR

This function takes the current contents of the screen and saves them out the file provided

Used for: Rendering.
*/
    virtual LTRESULT MakeScreenShot(const char* pszFile) = 0;


/*!
\param  nWidth			The width of the render target to be created. This should be >0 and a power of two
\param	nHeight			The height of the render target. This must be the same as width if it is a cube map
\param	nOptions		A combination of the above eRTO_ flags indicating what this render target should support
\return \b LT_OK, \b LT_ERROR

This function will create a render target object which can then become the target of rendering operations. This
render target must be released with the ReleaseRenderTarget function. 

Used for: Render Targets.
*/
	virtual LTRESULT CreateRenderTarget(uint32 nWidth, uint32 nHeight, uint32 nOptions, HRENDERTARGET& hTarget) = 0;

/*!
\param  hTarget			The render target to add an additional reference to
\return \b LT_OK, \b LT_INVALIDPARAMS

This function adds an additional reference to the render target. This should be used if additional references
are held to the render target

Used for: Render Targets.
*/
	virtual LTRESULT AddRenderTargetRef(HRENDERTARGET hTarget) = 0;

/*!
\param  hTarget			The render target to release a reference to
\return \b LT_OK, \b LT_INVALIDPARAMS

This function releases a reference to the specified render target. After this call, it is not safe to use
that render target handle as this call could release the render target itself.

Used for: Render Targets.
*/
	virtual LTRESULT ReleaseRenderTarget(HRENDERTARGET hTarget) = 0;

/*!
\param  hTarget			The render target to set as the active render target
\param  nSurface		For cube map render targets, this indicates the surface to render to
\return \b LT_OK, \b LT_INVALIDPARAMS

This function sets the provided render target as the destination for all rendering operations. This should
be uninstalled by either installing another render target, or by calling SetRenderTargetScreen, as soon as
rendering to the render target is complete.

Used for: Render Targets.
*/
	virtual LTRESULT SetRenderTarget(HRENDERTARGET hTarget, ECubeTextureFaces eSurface = eCTF_PositiveX) = 0;

/*!
This function will return a handle to the render target that is currently associated with the device. This
will return NULL if no render target is currently associated, which means that the screen is the current
render target. The returned handle must be released through ReleaseRenderTarget to avoid resource leaks.
*/
	virtual HRENDERTARGET GetCurrentRenderTarget() = 0;

/*!
\return \b LT_OK

This function removes any currently installed render targets, and sets the current render target back
to the screen.

Used for: Render Targets.
*/
	virtual LTRESULT SetRenderTargetScreen() = 0;


/*!
\param  hSourceTarget	The render target to copy the image from
\param  rSourceRect		The rectangle to copy the image from, specified in [0..1] range
\param	hDestTarget		The render target to copy the image to
\param	rDestRect		The rectangle to copy the image to, specified in [0..1] range
\return \b LT_OK, \b LT_INVALIDPARAMS

This function copies a rectangular area from one render target to another rendertarget.  Either the
source or destination may be set to NULL to indicate use of the screen render target.  

Note: The source and destination targets are not allowed to be the same.

Used for: Render Targets.
*/
	virtual LTRESULT StretchRect(	HRENDERTARGET hSourceTarget, const LTRect2f& rSourceRect,
									HRENDERTARGET hDestTarget, const LTRect2f& rDestRect) = 0;


/*!
\param hTarget			The render target that the associated visibility will wrap
\return This will return either a valid render target vis, or NULL on failure

This creates a render target visibility query, which can be used in place of a render target.
When this visibility query is used by a material, it will have the visibility flag set, and
it is then the responsibility of the client to clear the flag as appropriate.

Used for: Render Target Vis Queries.
*/
	virtual HRENDERTARGETVIS CreateRenderTargetVisQuery(HRENDERTARGET hTarget) = 0;

/*!
\param hTargetVis		The render target visibility to add a reference to
\return LT_OK or LT_INVALIDPARAMS

This will add a reference to the provided render target vis. This must have a corresponding
release.

Used for: Render Target Vis Queries.
*/

	virtual LTRESULT AddRenderTargetVisQueryRef(HRENDERTARGETVIS hTargetVis) = 0;

/*!
\param hTargetVis		The render target visibility to release a reference of
\return LT_OK or LT_INVALIDPARAMS

This will release the reference count of the associated render target visibility query. The
provided reference should not be used after this call.

Used for: Render Target Vis Queries.
*/

	virtual LTRESULT ReleaseRenderTargetVisQuery(HRENDERTARGETVIS hTargetVis) = 0;

/*!
\param hTargetVis		The render target vis to query about whether or not it was rendered
\param bRendered		Return status of whether or not the render target was displayed or not
\return \b LT_OK, \b LT_INVALIDPARAMS

Determines if the visible flag has been set for the specified render target vis. This flag is set
to true each time that a material is rendered that uses the specified render target vis, and can
then be cleared by the user. This is often useful for determining when a render target needs to be
updated.

Used for: Render Target Vis Queries.
*/
	virtual LTRESULT WasRenderTargetVisVisible(HRENDERTARGETVIS hTargetVis, bool& bRendered) = 0;

/*!
\param hTargetVis		The render target vis query to clear the was visible flag
\return \b LT_OK, \b LT_INVALIDPARAMS

Clears the visible flag from the associated render target vis. See WasRenderTargetVisible for
more detailed information.

Used for: Render Target Vis Queries.
*/
	virtual LTRESULT ClearRenderTargetVisVisible(HRENDERTARGETVIS hTargetVis) = 0;

/*!
\param hTargetVis		The render target vis to query to change the render target it is associated with
\param hRenderTarget	The new render target to associate the visibility query with
\return \b LT_OK, \b LT_INVALIDPARAMS

This changes the render target that this visibility query is associated with, without recreating the visibility
query.

Used for: Render Target Vis Queries.
*/
	virtual LTRESULT SetRenderTargetVisTarget(HRENDERTARGETVIS hTargetVis, HRENDERTARGET hRenderTarget) = 0;

/*!
\param pszMaterialName	The name of the material instance that is to be loaded

Called to create a material from the specified name. This will return an HMATERIAL
or INVALID_MATERIAL if it failed

Used For: Material Instance Management.
*/
	virtual HMATERIAL	CreateMaterialInstance(const char* pszMaterialName) = 0;

/*!
\param hMaterial	The material that is to be released. This reference should not be used again

releases a material instance reference

Used For: Material Instance Management.
*/
	virtual LTRESULT	ReleaseMaterialInstance(HMATERIAL hMaterial) = 0;

/*!
\param hMaterial	The material that is to have an additional reference added to it

adds a reference to an existing material instance

Used For: Material Instance Management.
*/
	virtual LTRESULT	AddRefMaterialInstance(HMATERIAL hMaterial) = 0;

/*!
\param hMaterial	The material to duplicate

Creates a duplicate of a material instance that may be modified without affecting the original

Used For: Material Instance Management.
*/
	virtual HMATERIAL	CloneMaterialInstance(HMATERIAL hMaterial) = 0;

/*!
\param hMaterial	The material that is to have the filename extracted from it
\param pszBuffer	The buffer to fill the name into
\param nBufferLen	The length of the buffer, this will be enforced

gets the filename from a material instance

Used For: Material Instance Management.
*/
	virtual LTRESULT	GetMaterialFileName(HMATERIAL hMaterial, char* pszBuffer, uint32 nBufferLen) = 0;


/*!
This is called to optimize the video memory, and most likely will result in all device resources being
destroyed and recreated and all managed resources being evicted from video memory. This is a VERY
expensive call and must never be called during play.
*/
	virtual LTRESULT	OptimizeVideoMemory() = 0;
	
/*!
\param pszMatInstance	The name of the material instance that holds the desired parameter
						this is relative to the resource root (i.e. materials\somematerial.lmi)
\param pszParam			The name of the parameter to change (case sensitive)
\param Val				The value to set the parameter to

Changes the specified parameter of the specified material instance to the value specified. This
will return LT_INVALIDPARAMS if it cannot find the material instance, or a parameter that
matches the name and type within that material instance.

Used For: Material Instance Parameters.
*/
	virtual LTRESULT	SetInstanceParamFloat(HMATERIAL hMaterial, const char* pszParam, float fVal) = 0;
	virtual LTRESULT	SetInstanceParamBool(HMATERIAL hMaterial, const char* pszParam, bool bVal) = 0;
	virtual LTRESULT	SetInstanceParamVector(HMATERIAL hMaterial, const char* pszParam, const LTVector& vVal) = 0;
	virtual LTRESULT	SetInstanceParamVector4(HMATERIAL hMaterial, const char* pszParam, const LTVector4& vVal) = 0;
	virtual LTRESULT	SetInstanceParamMatrix(HMATERIAL hMaterial, const char* pszParam, const LTMatrix& mVal) = 0;
	virtual LTRESULT	SetInstanceParamTexture(HMATERIAL hMaterial, const char* pszParam, HTEXTURE Texture) = 0;
	virtual LTRESULT	SetInstanceParamTextureAnim(HMATERIAL hMaterial, const char* pszParam, const char* pszTextureAnim) = 0;
	virtual LTRESULT	SetInstanceParamRenderTarget(HMATERIAL hMaterial, const char* pszParam, HRENDERTARGET hRenderTarget) = 0;
	virtual LTRESULT	SetInstanceParamRenderTargetVis(HMATERIAL hMaterial, const char* pszParam, HRENDERTARGETVIS hRenderTargetVis) = 0;
	virtual LTRESULT	SetInstanceParamVideoTexture(HMATERIAL hMaterial, const char* pszParam, HVIDEOTEXTURE hVideoTexture) = 0;
	virtual LTRESULT	SetInstanceParamInt(HMATERIAL hMaterial, const char* pszParam, int32 nVal) = 0;

/*!
\param pszMatInstance	The name of the material instance that holds the desired parameter
						this is relative to the resource root (i.e. materials\somematerial.lmi)
\param pszParam			The name of the parameter to read (case sensitive)
\param Val				The location to store the parameter in

Reads the specified parameter of the specified material instance. This
will return LT_INVALIDPARAMS if it cannot find the material instance, or a parameter that
matches the name and type within that material instance.

Used For: Material Instance Parameters.
*/
	virtual LTRESULT	GetInstanceParamFloat(HMATERIAL hMaterial, const char* pszParam, float& fVal) = 0;
	virtual LTRESULT	GetInstanceParamBool(HMATERIAL hMaterial, const char* pszParam, bool& bVal) = 0;
	virtual LTRESULT	GetInstanceParamVector(HMATERIAL hMaterial, const char* pszParam, LTVector& vVal) = 0;
	virtual LTRESULT	GetInstanceParamVector4(HMATERIAL hMaterial, const char* pszParam, LTVector4& vVal) = 0;
	virtual LTRESULT	GetInstanceParamMatrix(HMATERIAL hMaterial, const char* pszParam, LTMatrix& mVal) = 0;
	virtual LTRESULT	GetInstanceParamTexture(HMATERIAL hMaterial, const char* pszParam, HTEXTURE& Texture) = 0;
	virtual LTRESULT	GetInstanceParamTextureAnim(HMATERIAL hMaterial, const char* pszParam, char* pszBuffer, uint32 nBufferLen) = 0;
	virtual LTRESULT	GetInstanceParamRenderTarget(HMATERIAL hMaterial, const char* pszParam, HRENDERTARGET& hRenderTarget) = 0;
	virtual LTRESULT	GetInstanceParamRenderTargetVis(HMATERIAL hMaterial, const char* pszParam, HRENDERTARGETVIS& hRenderTargetVis) = 0;
	virtual LTRESULT	GetInstanceParamVideoTexture(HMATERIAL hMaterial, const char* pszParam, HVIDEOTEXTURE& hVideoTexture) = 0;
	virtual LTRESULT	GetInstanceParamInt(HMATERIAL hMaterial, const char* pszParam, int32& nVal) = 0;

/*!
\param pszMatInstance	The name of the material instance that holds the desired parameter
						this is relative to the resource root (i.e. materials\somematerial.lmi)
\param pszParam			The name of the parameter to reset (case sensitive)

Restores the specified parameter of the specified material instance back to the value saved on disk. This
will return LT_INVALIDPARAMS if it cannot find the material instance, or a parameter that
matches the name and type within that material instance.

Used For: Material Instance Parameters.
*/
	virtual LTRESULT	RestoreInstanceParamDefault(HMATERIAL hMaterial, const char* pszParam) = 0;
	
	
/*!
\param hObject	The object to which the decal will be applied
\param tProejction	The projection used to apply the decal
\param hMaterial	The material to use when rendering the decal
\param bColorOverride	When set to true, the decal color will be used instead of the model's color
\param nColor	If bColorOverride is set to true, this color will be used when rendering the decal
\param hDecal	The created decal

Creates a model decal.

Used For: Model Decals.
*/
	virtual LTRESULT	CreateModelDecal(HOBJECT hObject, const LTMatrix3x4 &mProjection, HMATERIAL hMaterial, bool bColorOverride, uint32 nColor, HMODELDECAL &hDecal) = 0;
/*!
\param hDecal	The decal to destroy

Destroys a model decal.

Used For: Model Decals.
*/
	virtual LTRESULT	DestroyModelDecal(HMODELDECAL hDecal) = 0;
	
/*!
\param hObject	The object to query.
\param pDecalArray	An array buffer for returning the decals associated with the object
\param nDecalArraySize	On entry, this function specifies the size of the pDecalArray array.  
						On exit, this will be set to the number of decals associated with the model.

Queries the list of decals that are associated with an object.

Used For: Model Decals.
*/
	virtual LTRESULT	GetModelDecals(HOBJECT hObject, HMODELDECAL *pDecalArray, uint32 &nDecalArraySize) = 0;
	
/*!
\param hDecal	The decal to use

Queries or sets the properties of a model decal

Used For: Model Decals.
*/
	virtual LTRESULT	GetModelDecalObject(HMODELDECAL hDecal, HOBJECT &hObject) = 0;
	virtual LTRESULT	SetModelDecalObject(HMODELDECAL hDecal, HOBJECT hObject) = 0;
	virtual LTRESULT	GetModelDecalProjection(HMODELDECAL hDecal, LTMatrix3x4 &mProjection) = 0;
	virtual LTRESULT	SetModelDecalProjection(HMODELDECAL hDecal, const LTMatrix3x4 &mProjection) = 0;
	virtual LTRESULT	GetModelDecalMaterial(HMODELDECAL hDecal, HMATERIAL &hMaterial) = 0;
	virtual LTRESULT	SetModelDecalMaterial(HMODELDECAL hDecal, HMATERIAL hMaterial) = 0;
	virtual LTRESULT	GetModelDecalColorOverride(HMODELDECAL hDecal, bool &bColorOverride) = 0;
	virtual LTRESULT	SetModelDecalColorOverride(HMODELDECAL hDecal, bool bColorOverride) = 0;
	virtual LTRESULT	GetModelDecalColor(HMODELDECAL hDecal, float &fColorR, float &fColorG, float &fColorB, float &fColorA) = 0;
	virtual LTRESULT	SetModelDecalColor(HMODELDECAL hDecal, float fColorR, float fColorG, float fColorB, float fColorA) = 0;

/*!
\param pCallback	The callback function to use
\param pUser		User data provided to the callback function

Sets the callback function called when a model decal gets deleted

Used For: Model Decals.
*/
	virtual LTRESULT	SetModelDecalDeleteFn(ModelDecalDeleteFn pCallback, void* pUser) = 0;
	virtual LTRESULT	GetModelDecalDeleteFn(ModelDecalDeleteFn &pCallback, void* &pUser) = 0;
};

#endif
