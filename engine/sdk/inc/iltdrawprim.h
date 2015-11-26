//-------------------------------------------------------------------
//
//   MODULE    : ILTDRAWPRIM.H
//
//   PURPOSE   : Defines engine interface ILTDrawPrimitive
//
//   CREATED   : On 8/11/00 At 10:40:21 AM
//
//-------------------------------------------------------------------

#ifndef __ILTDRAWPRIM_H__
#define __ILTDRAWPRIM_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

#ifndef __ILTTEXTUREMGR_H__
#include "ilttexturemgr.h"
#endif

#ifndef __ILTVIDEOTEXTURE_H__
#include "iltvideotexture.h"
#endif

#ifndef __ILTRENDERER_H__
#include "iltrenderer.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

// screen near z -- use this value if you want your polygons to draw
// in front of everything

#ifdef _WIN32
#define SCREEN_NEAR_Z       0.0
#endif


//-----------------------------
// Vertex Primitive Definitions
//-----------------------------

// RGBA Color def
/*!
  Contains the RGBA data for a vertex. Gouraud shaded
  vertices use this structure. 
*/
struct LT_VERTRGBA
{
	LT_VERTRGBA()										{ memset(this,0,sizeof(LT_VERTRGBA)); }
	LT_VERTRGBA(uint8 r, uint8 g, uint8 b, uint8 a)		{ Init(r, g, b, a); }
	void Init(uint8 r, uint8 g, uint8 b, uint8 a)		{ m_nColor = SETRGBA(r, g, b, a); }
	uint32 m_nColor;
};

// Textured gouraud shaded vertex
/*!
  Contains coordinate, color, and texture information for a textured,
  Gouraud shaded vertex.
*/
struct LT_VERTGT
{

	LT_VERTGT() { pos.Init(); uv.Init(); }
    LTVector		pos;
	LT_VERTRGBA		rgba;
    LTVector2		uv;
};

// Gouraud shaded vertex
/*!
Contains coordinate and color information for a Gouraud-shaded vertex.
*/
struct LT_VERTG
{       
	LT_VERTG() { pos.Init(); }
    LTVector		pos;
	LT_VERTRGBA		rgba;
};

// Textured gouraud shaded vertex with tangent space
/*!
Contains coordinate, color, and texture information for a textured,
Gouraud shaded vertex.
*/
struct LT_VERTGTTS
{

	LT_VERTGTTS() { pos.Init(); uv.Init(); normal.Init(0.0f, 0.0f, -1.0f); tangent.Init(1.0f, 0.0f, 0.0f); binormal.Init(0.0f, 1.0f, 0.0f);  }
	LTVector		pos;
	LT_VERTRGBA		rgba;
	LTVector2		uv;
	LTVector		normal;
	LTVector		tangent;
	LTVector		binormal;
};

//------------------------------
// Polygon Primitive Definitions
//------------------------------

// Tangent space textured gouraud shaded triangle
/*!
Contains vertex structure information for a textured, Gouraud shaded triangle.
*/
struct LT_POLYGTTS3
{
	enum { k_nNumVerts		= 3};
	LT_VERTGTTS	verts[k_nNumVerts];
};

// Textured gouraud shaded triangle
/*!
Contains vertex structure information for a textured, Gouraud shaded triangle.
*/
struct LT_POLYGT3
{
	enum { k_nNumVerts		= 3};
    LT_VERTGT	verts[k_nNumVerts];
};

// Gouruad shaded triangle
/*!
Contains vertex structure information for a Gouraud shaded triangle.
*/
struct LT_POLYG3
{
	enum { k_nNumVerts		= 3};
    LT_VERTG	verts[k_nNumVerts];
};

// Tangent space textured gouraud shaded quad
/*!
Contains vertex structure information for a textured, Gouraud shaded triangle.
*/
struct LT_POLYGTTS4
{
	enum { k_nNumVerts		= 4};
	LT_VERTGTTS	verts[k_nNumVerts];
};

// Textured gouraud shaded quad
/*!
Contains vertex structure information for a textured, Gouraud shaded quadrilateral.
*/
struct LT_POLYGT4
{
	enum { k_nNumVerts		= 4};
    LT_VERTGT	verts[k_nNumVerts];
};

// Gouruad shaded quad
/*!
Contains vertex structure information for a Gouraud shaded quadrilateral.
*/
struct LT_POLYG4
{
	enum { k_nNumVerts		= 4};
    LT_VERTG	verts[k_nNumVerts];
};

// Gouruad shaded line
/*!
Contains vertex structure information for a Gouraud shaded line.
*/
struct LT_LINEG
{
	enum { k_nNumVerts		= 2};
    LT_VERTG	verts[k_nNumVerts];
};

//------------------------------------------------
// Color Utilities
//------------------------------------------------


//------------------------------------------------
// Template Utilities
//------------------------------------------------

//these functions will take any one of the above primitive types and will perform color operations
//upon them as specified. This is much more efficient than having to call through to an interface
//each time, and these templates will expand out to be very efficient.
template <class T>
void DrawPrimSetRGBA(T& Primitive, uint32 nColor)
{
	for(uint32 nCurrVert = 0; nCurrVert < T::k_nNumVerts; nCurrVert++)
	{
		Primitive.verts[nCurrVert].rgba.m_nColor = nColor;
	}
}

template <class T>
void DrawPrimSetRGBA(T& Primitive, uint8 r, uint8 g, uint8 b, uint8 a)
{
	DrawPrimSetRGBA(Primitive, SETRGBA(r, g, b, a));
}

template <class T>
void DrawPrimSetAlpha(T& Primitive, uint8 a)
{
	for(uint32 nCurrVert = 0; nCurrVert < T::k_nNumVerts; nCurrVert++)
	{
		uint32 nSrcColor = Primitive.verts[nCurrVert].rgba.m_nColor;
		Primitive.verts[nCurrVert].rgba.m_nColor = SETRGBA(GETR(nSrcColor), GETG(nSrcColor), GETB(nSrcColor), a);
	}
}

//sets up the UV coordinates of a vertex
template <class T>
void DrawPrimSetUV(T& Primitive, uint32 nVert, float fU, float fV)
{
	LTASSERT(nVert < T::k_nNumVerts, "Error: Invalid vertex specified");
	Primitive.verts[nVert].uv.Init(fU, fV);
}

//sets up the XY coordinates of a vertex
template <class T>
void DrawPrimSetXY(T& Primitive, uint32 nVert, float fX, float fY)
{
	LTASSERT(nVert < T::k_nNumVerts, "Error: Invalid vertex specified");
	Primitive.verts[nVert].pos.Init(fX, fY, 1.0f);
}

//sets up the XYZ coordinates of a vertex
template <class T>
void DrawPrimSetXYZ(T& Primitive, uint32 nVert, float fX, float fY, float fZ)
{
	LTASSERT(nVert < T::k_nNumVerts, "Error: Invalid vertex specified");
	Primitive.verts[nVert].pos.Init(fX, fY, fZ);
}

//sets up the XY coordinates of a quad with the rectangle extents specified
template <class T>
void DrawPrimSetXYWH(T& Primitive, float fLeft, float fTop, float fWidth, float fHeight)
{
	DrawPrimSetXY(Primitive, 0, fLeft, fTop);
	DrawPrimSetXY(Primitive, 1, fLeft + fWidth, fTop);
	DrawPrimSetXY(Primitive, 2, fLeft + fWidth, fTop + fHeight);
	DrawPrimSetXY(Primitive, 3, fLeft, fTop + fHeight);
}

//sets up the UV coordinates of a quad with the rectangle extents specified in texture space
template <class T>
void DrawPrimSetUVWH(T& Primitive, float fLeft, float fTop, float fWidth, float fHeight)
{
	DrawPrimSetUV(Primitive, 0, fLeft, fTop);
	DrawPrimSetUV(Primitive, 1, fLeft + fWidth, fTop);
	DrawPrimSetUV(Primitive, 2, fLeft + fWidth, fTop + fHeight);
	DrawPrimSetUV(Primitive, 3, fLeft, fTop + fHeight);
}

//sets up the tangent space of the vertices within the quad to match those provided
template <class T>
void DrawPrimSetTangentSpace(T& Primitive, const LTVector& vNormal, const LTVector& vTangent, const LTVector& vBinormal)
{
	for(uint32 nCurrVert = 0; nCurrVert < T::k_nNumVerts; nCurrVert++)
	{
		Primitive.verts[nCurrVert].normal	= vNormal;
		Primitive.verts[nCurrVert].tangent	= vTangent;
		Primitive.verts[nCurrVert].binormal = vBinormal;
	}
}

//------------------------------------------------
// ILTDrawPrim enums
//------------------------------------------------

/*!
Enumerates the modes with which a primitive is drawn. 
\see ILTDrawPrim::SetAlphaBlendMode()
*/
enum  ELTDrawPrimRenderMode
{
    //modulates between a vertex color and a texture and writes it out to the screen with no
	//blending
    eLTDrawPrimRenderMode_Modulate_NoBlend,
	//performs a modulate between a texture and vertex color and then adds it to the screen
	//buffer scaled using the alpha of the two modulated
    eLTDrawPrimRenderMode_Modulate_Additive,
    //performs a modulate between a texture and vertex color and then blends it with the screen
	//buffer using the alpha of the two modulated
    eLTDrawPrimRenderMode_Modulate_Translucent,

	//this must come last and represents the number of available render modes
	eLTDrawPrimRenderMode_NumModes,
};

/*!
Enumerates the different transform modes that can be used for rendering geometry
\see ILTDrawPrim::SetTransformMode()
*/
enum ELTDrawPrimTransformMode
{
	//screen space, this is the default
	eLTDrawPrimTransformMode_Screen,
	//world space
	eLTDrawPrimTransformMode_World,

	//this must come last and represents the number of transform modes available
	eLTDrawPrimTransformMode_NumModes,
};

/*!
Enumerates the different Z buffering modes that can be used for rendering geometry
\see ILTDrawPrim::SetZMode()
*/
enum ELTDrawPrimZMode
{
	//don't read or write the Z buffer
	eLTDrawPrimZMode_None,
	//don't read from the Z buffer, but write to it
	eLTDrawPrimZMode_NoRead,
	//don't write to the Z buffer but read from it
	eLTDrawPrimZMode_NoWrite,
	//full read and write support
	eLTDrawPrimZMode_Full,

	//this must come last and represents the number of transform modes available
	eLTDrawPrimZMode_NumModes,
};

/*!
The ILTDrawPrim is a low level polygon based drawing/rendering interface.

Define a holder to get this interface like this:
\code
define_holder(ILTDrawPrim, your_var);
\endcode
*/
class ILTDrawPrim : public IBase
{
public :
    interface_version(ILTDrawPrim, 0);

	/*!
    Called to begin a draw primitive block. This is for optimizing draw primitive
	calls. Calls to have the engine render anything not through the draw primitive
	cannot be made inside this block. In addition, calls to DrawPrimMaterial must not
	be made within this block (use BeginDrawPrimMaterialBlock below)
    
    Used for: Rendering.
    */
	virtual LTRESULT BeginDrawPrimBlock() = 0;

	/*!
    Called to end a draw primitive block. 
    
    Used for: Rendering.
    */
	virtual LTRESULT EndDrawPrimBlock() = 0;

    /*!
    \param hCamera  The handle of the camera.
    
    Set the camera relative to which the primitive is drawn.
    
    Used for: Rendering.
    */
    virtual LTRESULT SetCamera(const HOBJECT hCamera) = 0;
	virtual HOBJECT GetCamera() = 0;

	/*!
	\param eZMode  One of the ELTDrawPrimZMode types indicating how to use the Z buffer

	Sets the type of manipulation that should be done with the Z buffer, valid for
	both DrawPrim and DrawPrimMaterial

	Used for: Rendering.
	*/
	virtual LTRESULT SetZMode(const ELTDrawPrimZMode eZMode) = 0;
	virtual ELTDrawPrimZMode GetZMode() = 0;

    /*!
    \param hTexure  The handle of the texture.
    
    Set the texture used by the primitive. If a texture is not set using this function, all textured polygons will be rendered with random texture data.
    
    Used for: Rendering.
    */
    virtual LTRESULT SetTexture(HTEXTURE hTexture) = 0;

	/*!
	\param hTexure  The handle of the dynamic texture.

	Set the texture used by the primitive. If a texture is not set using this function, all textured polygons will be rendered with random texture data.

	Used for: Rendering.
	*/
	virtual LTRESULT SetDynamicTexture(HDYNAMICTEXTURE hTexture) = 0;

	/*!
    \param hVideoTexure  The handle of the video texture.
    
    Set the texture used by the primitive. This will install the current frame of the video as the active texture.
	If a texture is not set using this function, all textured polygons will be rendered with random texture data.
    
    Used for: Rendering.
    */
    virtual LTRESULT SetVideoTexture(HVIDEOTEXTURE hVideoTexture) = 0;

	/*!
    \param hRenderTarget  The handle of the render target.
    
    Set the texture used by the primitive. This will install the render target image as the active texture.
	If a texture is not set using this function, all textured polygons will be rendered with random texture data.
    
    Used for: Rendering.
    */
    virtual LTRESULT SetRenderTarget(HRENDERTARGET hRenderTarget) = 0;

    /*!
    \param eRenderMode An \b ELTDrawPrimRenderMode enum value.
    
    Set the alpha blend mode of the primitive.
    
    Used for: Rendering.
    */
    virtual LTRESULT SetRenderMode(const ELTDrawPrimRenderMode eRenderMode) = 0;
	virtual ELTDrawPrimRenderMode GetRenderMode() = 0;

	/*!
	\param eTransformMode An \b ELTDrawPrimTransformMode enum value.

	Set the transform mode that will be used when rendering

	Used for: Rendering.
	*/
	virtual LTRESULT SetTransformMode(const ELTDrawPrimTransformMode eTransformMode) = 0;
	virtual ELTDrawPrimTransformMode GetTransformMode() = 0;

    
    /*!
    \param pPrim        A pointer to an array of polygon structures to draw.
    \param nCount       The number of polygons (default value is 1) in the array identified by the \b pPrim parameter.

    Draw one or more polgons of one type (line, triangle, or quadrilateral). This overloaded function supports the following structures: \b LT_LINEF, \b LT_LINEFT, \b LT_LINEG, \b LT_LINEGT, \b LT_POLYF3, \b LT_POLYFT3, \b LT_POLYG3, \b LT_POLYGT3, \b LT_POLYF4, \b LT_POLYFT4, \b LT_POLYG4, and\b LT_POLYGT4.

    
    Used for: Rendering.
    */
    virtual LTRESULT DrawPrim (LT_POLYGT3 *pPrim, const uint32 nCount = 1) = 0;
    virtual LTRESULT DrawPrim (LT_POLYG3 *pPrim, const uint32 nCount = 1) = 0;
    virtual LTRESULT DrawPrim (LT_POLYGT4 *pPrim, const uint32 nCount = 1) = 0;
    virtual LTRESULT DrawPrim (LT_POLYG4 *pPrim, const uint32 nCount = 1) = 0;
    virtual LTRESULT DrawPrim (LT_LINEG *pPrim, const uint32 nCount = 1) = 0;

	/*!
	Called to begin a draw primitive material block. This is for optimizing draw primitive
	calls. Calls to have the engine render anything not through the draw primitive
	cannot be made inside this block. In addition, calls to DrawPrim must not
	be made within this block (use BeginDrawPrimBlock above)

	Used for: Rendering.
	*/
	virtual LTRESULT BeginDrawPrimMaterialBlock() = 0;

	/*!
	Called to end a draw primitive material block. 

	Used for: Rendering.
	*/
	virtual LTRESULT EndDrawPrimMaterialBlock() = 0;


	//called to render the material using the provided material and shader LOD
	virtual LTRESULT DrawPrimMaterial(	LT_POLYGTTS4 *pPrim, const uint32 nCount, 
										HMATERIAL hMaterial) = 0;
	virtual LTRESULT DrawPrimMaterial(	LT_POLYGTTS3 *pPrim, const uint32 nCount, 
										HMATERIAL hMaterial) = 0;

};

#endif // __ILTDRAWPRIM_H_

