//-------------------------------------------------------------------
//
//   MODULE    : ILTCUSTOMRENDER.H
//
//   PURPOSE   : Defines engine interface ILTCustomRender which is
//					used to handle manipulation of the custom render
//					objects and related operations.
//
//-------------------------------------------------------------------

#ifndef __ILTCUSTOMRENDER_H__
#define __ILTCUSTOMRENDER_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

#ifndef __ILTCUSTOMRENDERCALLBACK_H__
#include "iltcustomrendercallback.h"
#endif

//forward declarations
class ILTCustomRenderCallback;

//the maximum size that a vertex declaration can be. This number should not cause any
//problems, but can be increased easily if necessary.
#define	MAX_CUSTOM_RENDER_DECL_LENGTH	64

//the maximum size of a vertex buffer. A constant is used instead of a device cap to help
//prevent bugs appearing on different cards, and also to prevent the overhead of querying
//to determine the size
#define MAX_VERTEX_BUFFER_SIZE			(1024 * 1024)

//same thing for an index buffer, but is in terms of indices instead of bytes
#define MAX_INDEX_BUFFER_SIZE			(64 * 1024)

//Types that the custom render interface uses to communicate with game code

//Prototype for a function that will be called by the engine during rendering to allow 
//for the object to perform custom rendering
typedef void	(*TCustomRenderObjectRenderCB)(ILTCustomRenderCallback* pInterface, const LTRigidTransform& tCamera, void* pUser);

//Prototype for a function that will be called by the engine once it is determined that
//this object is in the visible scene, and will be called once prior to rendering. This can return
//false in order to indicate that the object should not be rendered in this scene. This is provided
//with the scene camera, the sky camera, and the user data associated with the object
typedef bool	(*TCustomRenderVisibleCB)(const LTRigidTransform& tCamera, const LTRigidTransform& tSkyCamera, void* pUser);

//enumeration representing the different types of vertex declaration data
enum EVertexDeclDataType
{
	eVertexDeclDataType_Float1,
	eVertexDeclDataType_Float2,
	eVertexDeclDataType_Float3,
	eVertexDeclDataType_Float4,
	eVertexDeclDataType_PackedColor,
};

//enumeration representing the different vertex declaration usages
enum EVertexDeclUsage
{
	eVertexDeclUsage_Position,		//Float3 only
	eVertexDeclUsage_Normal,		//Float3 only
	eVertexDeclUsage_Tangent,		//Float3 only
	eVertexDeclUsage_Binormal,		//Float3 only
	eVertexDeclUsage_TexCoord,		//Float1..4
	eVertexDeclUsage_Color,			//Float4 or PackedColor
	eVertexDeclUsage_BlendWeight,	//Float4 or PackedColor
};

//class representing a single vertex declaration element. A vertex declaration consists of a collection
//of these
struct SVertexDeclElement
{
	//the stream that this vertex declaration item comes from
	uint32					m_nStream;

	//the type that is used to represent this element
	EVertexDeclDataType		m_eDataType;

	//the usage of this element
	EVertexDeclUsage		m_eUsage;

	//the usage index of this element. For example color can have two usage types, 0 and 1, 
	//texture coordinates can have a device determined number, and so on.
	uint32					m_nUsageIndex;	
};

//rendering space used by the object
enum ERenderSpace
{
	eRenderSpace_Object,
	eRenderSpace_World,
	eRenderSpace_View
};

/*!
The ILTCustomRender provides access to all custom render object management functions
and all related operations such as creating static buffers and vertex declarations.

Define a holder to get this interface like this:
\code
define_holder(ILTCustomRender, your_var);
\endcode
*/

class ILTCustomRender :
	public IBase
{
public:

	interface_version(ILTCustomRender, 0);
	virtual ~ILTCustomRender()			{}

/*!
\param hObject			A custom render object to set the visibility information of
\param vMin				The minimum bounding box of this object (relative to the object position)
\param vMax				The maximum extents of the bounding box (relative to the object position)

Sets the visibility primitive associated with the custom render object to be a bounding box
of the specified dimensions

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetVisBoundingBox(HOBJECT hObject, const LTVector& vMin, const LTVector& vMax) = 0;

/*!
\param hObject			A custom render object to set the visibility information of
\param vCenter			The center of the bounding sphere
\param fRadius			The radius of the bounding sphere

Sets the visibility primitive associated with the custom render object to be a bounding sphere
of the specified dimensions

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetVisBoundingSphere(HOBJECT hObject, const LTVector& vCenter, float fRadius) = 0;

/*!
\param hObject			A custom render object to set the callback information of
\param pCBFn			The callback function that will be called when the object is in the scene

Sets the callback information of the associated custom render object and will override any
previously set values. This must be set whenever an object is created or loaded as this data
cannot be saved and restored.

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetVisibleCallback(HOBJECT hObject, TCustomRenderVisibleCB pCBFn) = 0;


/*!
\param hObject			A custom render object to set the callback information of
\param pCBFn			The callback function that will be called when the object is to render

Sets the callback information of the associated custom render object and will override any
previously set values. This must be set whenever an object is created or loaded as this data
cannot be saved and restored.

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetRenderCallback(HOBJECT hObject, TCustomRenderObjectRenderCB pCBFn) = 0;

/*!
\param hObject			A custom render object to set the callback information of
\param pUser			The user data that will be provided to any callbacks

Sets the callback data that will be provided to any callbacks each time they are called. This will
override any previously set values and is not serialized.

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetCallbackUserData(HOBJECT hObject, void* pUser) = 0;

/*!
\param hObject			A custom render object to set the material of
\param hMaterial		The material to set on this custom render object

Sets the material associated with the custom render object

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetMaterial(HOBJECT hObject, HMATERIAL hMaterial) = 0;

/*!
\param hObject			A custom render object to set the material of
\param hMaterial		A handle to the current material handle. This must be released.

Gets the material associated with the custom render object

Used For: Custom Render Objects
*/
	virtual LTRESULT	GetMaterial(HOBJECT hObject, HMATERIAL& hMaterial) = 0;

/*!
\param hObject			A custom render object to set the world space flag of
\param bWorldSpace		Rendering space for the rendering data

Sets the space used for the object when rendering

Used For: Custom Render Objects
*/
	virtual LTRESULT	SetRenderingSpace(HOBJECT hObject, ERenderSpace eSpace) = 0;

/*!
\param hObject			A custom render object to get the world space flag of
\param eSpace			Rendering space for the rendering data

Gets the space used for the object when rendering

Used For: Custom Render Objects
*/
	virtual LTRESULT	GetRenderingSpace(HOBJECT hObject, ERenderSpace& eSpace) = 0;

/*!
\param nNumElements		The number of vertex declaration elements in the following list
\param pDeclElements	The list of vertex declaration elements
\param hDecl			Returned handle to the vertex declaration

Given a list of declaration elements, this will attempt to create a vertex declaration object
that can be used for rendering

Used For: Vertex Declaration
*/
	virtual LTRESULT	CreateVertexDeclaration(uint32 nNumElements, const SVertexDeclElement* pDeclElements, HVERTEXDECL& hDecl) = 0;

/*!
\param hDecl			The declaration to release

Called to release a vertex declaration object. The handle passed into here should not be used again.

Used For: Vertex Declaration
*/
	virtual LTRESULT	ReleaseVertexDeclaration(HVERTEXDECL hDecl) = 0;

/*!
\param hDecl			The declaration to add a reference to

Called to add a reference to a vertex declaration

Used For: Vertex Declaration
*/
	virtual LTRESULT	AddRefVertexDeclaration(HVERTEXDECL hDecl) = 0;

/*!
\param hDecl			The declaration to get use to determine stream size
\param nStream			The stream to query for size
\return					The size of a vertex on the specified stream

Given a compiled vertex declaration, this will determine how many bytes an item on the specified
stream occupies. If the object is invalid, or that is not a referenced stream, it will return 0

Used For: Vertex Declaration
*/
	virtual uint32		GetBytesPerVertexElementInStream(HVERTEXDECL hDecl, uint32 nStream) = 0;

/*!
\param nNumIndices		The number of indices that the static index buffer should contain
\param hIndexBuffer		The returned handle to the created static index buffer

This will attempt to create a static index buffer that contains the specified number of indices

Used For: Index Buffers
*/
	virtual LTRESULT	CreateStaticIndexBuffer(uint32 nNumIndices, HINDEXBUFFER& hIndexBuffer) = 0;

/*!
\param hIndexBuffer		The index buffer to lock
\param nStartIndex		The starting index to lock
\param nNumIndices		The number of indices to lock starting from the initial offset
\param pIndices			Returns the pointer to the index list. This is write only!

This will attempt to lock the specified subset of the index buffer for writing

Used For: Index Buffers
*/
	virtual LTRESULT	LockIndexBuffer(HINDEXBUFFER hIndexBuffer, uint32 nStartIndex, uint32 nNumIndices, uint16*& pIndices) = 0;

/*!
\param hIndexBuffer		The index buffer to unlock

Unlocks a previously locked index buffer. Any data returned from the index buffer lock should
not be used after this function is called.

Used For: Index Buffers
*/
	virtual LTRESULT	UnlockIndexBuffer(HINDEXBUFFER hIndexBuffer) = 0;

/*!
\param hIndexBuffer		The index buffer to release

Called to release an index buffer object. The handle passed into here should not be used again.

Used For: Index Buffers
*/
	virtual LTRESULT	ReleaseIndexBuffer(HINDEXBUFFER hIndexBuffer) = 0;

/*!
\param hIndexBuffer		The index buffer to add a reference to

Called to add a reference to an index buffer

Used For: Index Buffers
*/
	virtual LTRESULT	AddRefIndexBuffer(HINDEXBUFFER hIndexBuffer) = 0;

/*!
\param nBufferSize		The size of the vertex buffer to be created
\param hVertexBuffer	The returned handle to the created static vertex buffer

This will attempt to create a static vertex buffer of the specified size

Used For: Vertex Buffers
*/
	virtual LTRESULT	CreateStaticVertexBuffer(uint32 nSize, HVERTEXBUFFER& hVertexBuffer) = 0;

/*!
\param hVertexBuffer	The vertex buffer to lock
\param nStartLocation	The starting byte offset to lock
\param nNumBytes		The number of bytes to lock starting from the initial offset
\param pData			Returns the pointer to the block of memory. This is write only!

This will attempt to lock the specified subset of the index buffer for writing

Used For: Vertex Buffers
*/
	virtual LTRESULT	LockVertexBuffer(HVERTEXBUFFER hVertexBuffer, uint32 nStartLocation, uint32 nNumBytes, uint8*& pData) = 0;

/*!
\param hVertexBuffer	The vertex buffer to unlock

Unlocks a previously locked vertex buffer. Any data returned from the vertex buffer lock should
not be used after this function is called.

Used For: Vertex Buffers
*/
	virtual LTRESULT	UnlockVertexBuffer(HVERTEXBUFFER hVertexBuffer) = 0;

/*!
\param hVertexBuffer	The vertex buffer to release

Called to release an vertex buffer object. The handle passed into here should not be used again.

Used For: Vertex Buffers
*/
	virtual LTRESULT	ReleaseVertexBuffer(HVERTEXBUFFER hVertexBuffer) = 0;

/*!
\param hVertexBuffer	The vertex buffer to add a reference to

Called to add a reference to an vertex buffer

Used For: Vertex Buffers
*/
	virtual LTRESULT	AddRefVertexBuffer(HVERTEXBUFFER hVertexBuffer) = 0;


};

#endif
