//-------------------------------------------------------------------
//
//   MODULE    : ILTCUSTOMRENDERCALLBACK.H
//
//   PURPOSE   : Defines engine interface ILTCustomRenderCallback which
//					is provided to custom render objects when they
//					are called to render
//
//-------------------------------------------------------------------

#ifndef __ILTCUSTOMRENDERCALLBACK_H__
#define __ILTCUSTOMRENDERCALLBACK_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

//forward declarations
class CCustomRenderVertexDecl;
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;

//the maximum number of streams that the game code can use. Therefore any stream from 
//0..This value - 1 is a valid stream. Anything above that is invalid
#define	MAX_CUSTOM_RENDER_VERTEX_STREAMS	4

//the maximum number of dynamic vertex streams that can be locked
#define MAX_DYNAMIC_RENDER_VERTEX_STREAMS	2

//the size of a dynamic render vertex stream in bytes
#define DYNAMIC_RENDER_VERTEX_STREAM_SIZE	(512 * 1024)

//the size of a dynamic index buffer stream in indices
#define DYNAMIC_RENDER_INDEX_STREAM_SIZE	(64 * 1024)

//the number of indices inside of the quad index list
#define QUAD_RENDER_INDEX_STREAM_SIZE		(6 * 4096)

//handles to engine objects so the game code can refer to them

//represents a handle to a vertex declaration that will dictate how the streams are mapped
//to the device
typedef	CCustomRenderVertexDecl*	HVERTEXDECL;
typedef IDirect3DVertexBuffer9*		HVERTEXBUFFER;
typedef IDirect3DIndexBuffer9*		HINDEXBUFFER;

//an enumeration representing the different types of data that can be rendered
enum ECustomRenderPrimType
{
	//a list of single vertex point primitives
	eCustomRenderPrimType_PointList,

	//a list of lines, with every two elements forming one line
	eCustomRenderPrimType_LineList,

	//a list of triangles, with every three elements forming a triangle
	eCustomRenderPrimType_TriangleList,

	//a triangle strip, where the first two vertices create no triangles, and then
	//each following vertex creates a new triangle using the last two vertices
	eCustomRenderPrimType_TriangleStrip,

	//a triangle fan, where the first two vertices create no triangles, and then each
	//following vertex creates a new triangle using the previous vertex and the first vertex
	eCustomRenderPrimType_TriangleFan,
};

//Structure that is filled in by the caller in order to request the locking of one or more dynamic
//vertex buffers. The fields that must be filled in are the number of vertices, and the number
//of streams. Then for each stream the stream ID must be filled in. In addition, if all the vertices
//must start from a specific index, that flag should be set and the index provided. When locking, the
//engine will fill in data as to pointers where memory can be written, the vertex stride, and unless
//one was forced, an index where the vertex buffers begin which is needed for rendering. There
//are utility functions for simplifying setup of this class but they do not perform robust error
//checking.
struct SDynamicVertexBufferLockRequest
{
	//constructor
	SDynamicVertexBufferLockRequest() : 
		m_nNumVertices(0), 
		m_nStartIndex(0),
		m_pData(NULL),
		m_nStride(0),
		m_hInternalBuffer(NULL)
	{}

	//the number of vertices that were actually locked
	uint32			m_nNumVertices;

	//value of either the index that the dynamic locks must occur on or the result of the lock
	//where the buffers lie
	uint32			m_nStartIndex;

	//the locked data of this stream
	uint8*			m_pData;

	//the vertex stride of this stream
	uint32			m_nStride;

	//internal data used by the engine. Do not use this
	HVERTEXBUFFER	m_hInternalBuffer;
};

/*!
The ILTCustomRender provides access to all custom render functions that are relevant
for when it is rendering. This interface cannot be instantiated and is instead
provided to the callback function when the renderer calls into it, and it is through
this interface that the object is to setup the device and render.
*/
class ILTCustomRenderCallback
{
public:

	//allow for proper destruction when derived from
	virtual ~ILTCustomRenderCallback()		{}

/*!
\param hDecl	The vertex declaration that will be used for the next render call

Sets the vertex declaration that will be associated with the device. Note that when
this is changed it will clear out any vertex streams that were previously setup on
the device. This is becaues the stream sizes may change. Therefore make sure to install
this prior to setting up streams.

Used For: Custom Render Object Rendering
*/
	virtual	HRESULT	SetVertexDeclaration(HVERTEXDECL hDecl) = 0;

/*!
\param hIndexBuffer		The index buffer to use when rendering indexed primitives

Sets the index stream to use the one provided. Then when rendering, all indices will
be extracted from this bufer

Used For: Custom Render Object Rendering
*/
	virtual HRESULT BindIndexStream(HINDEXBUFFER hIndexBuffer) = 0;

/*!
Sets the index stream to use a precalculated quad based index stream. The indices in this
list begin with 0, and use four vertices for every triangle. In order to get the number
of triangles it can hold, take the max and divide it by 6. The number of indices it references
is equal to four times the number of triangles.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT BindQuadIndexStream() = 0;


/*!
\param hVertexBuffer	The vertex buffer to use when rendering primitives
\param nStream			The stream that this vertex buffer will be mapped to

Sets the vertex buffer as the source for the specified stream. This requires that a vertex
declaration has already been installed previously.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT BindVertexStream(HVERTEXBUFFER hVertexBuffer, uint32 nStream) = 0;

/*!
\param nNumIndices		The number of indices to lock from the dynamic index buffer
\param nStartIndex		Returned starting index, used for the render indexed call
\param pIndexBuffer		Returned pointer to the memory block that can be written to

Locks down a portion of memory in the dynamic index buffer for writing. Note that this
returned pointer can only be written to, otherwise unexpected behavior can result.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT LockDynamicIndexBuffer(uint32 nNumIndices, uint32& nStartIndex, uint16*& pIndexBuffer) = 0;

/*!
Unlocks the previously locked index buffer, and will install it as the active index buffer.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT UnlockAndBindDynamicIndexBuffer() = 0;

/*!
\param Request			A filled in vertex buffer lock request structure that will be filled in with
						the lock results

Locks down the specified number of dynamic vertex buffers as specified using any additional rules
provided.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT LockDynamicVertexBuffer(uint32 nNumVerts, SDynamicVertexBufferLockRequest& Request) = 0;

/*!
\param Request			A request structure that had been passed into the Lock function

Unlocks all the dynamic vertex buffers that had been previously locked, and installs them on each
appropriate stream for rendering.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT UnlockAndBindDynamicVertexBuffer(SDynamicVertexBufferLockRequest& Request) = 0;

/*!
\param pClipPlanes		An array of planes to use for clipping
\param nNumPlanes		The number of planes to use for clipping

Sets the clipping planes to use for rendering.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT SetClipPlanes(const LTPlane *pPlanes, uint32 nNumPlanes) = 0;
	
/*!
\param pClipPlanes		An array of planes in use for clipping
\param nNumPlanes		The number of planes in the array.
\returns The total number of active planes in nNumPlanes

Gets the active clipping planes used for rendering.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT GetClipPlanes(LTPlane *pPlanes, uint32 &nNumPlanes) = 0;

/*!
\param eType			The type of primitive that the vertices should be interpreted as
\param nStartVert		The starting vertex to start rendering with. This should include
						any vertex indexes necessary, such as that returned from locking dynamic
						vertex buffers.
\param nNumVerts		The number of vertices to use when rendering the primitives.

Using only the vertex streams this will render the list of primitives.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT Render(ECustomRenderPrimType eType, uint32 nStartVert, uint32 nNumVerts) = 0;

/*!
\param eType			The type of primitive that the vertices should be interpreted as
\param nStartIndex		The first vertex to begin with when rendering
\param nNumVerts		The number of vertices to render
\param nIndexOffset		A value to shift the indices by, should include values like the index returned
						from locking dynamic vertex buffers.
\param nMinVertexRef	The minimum vertex index to render, relative to the index offset
\param nMaxVertexRef	The maximum vertex index to render, relative to the index offset

Renders using both the currently installed index buffer and vertex buffers.

Used For: Custom Render Object Rendering
*/
	virtual HRESULT RenderIndexed(	ECustomRenderPrimType eType, 
									uint32 nStartIndex, uint32 nNumVerts, int32 nIndexOffset,
									uint32 nMinVertexRef, uint32 nMaxVertexRef) = 0;

};

#endif
