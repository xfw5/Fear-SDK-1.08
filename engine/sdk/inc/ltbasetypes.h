#ifndef __LTBASETYPES_H__
#define __LTBASETYPES_H__

#ifndef __LTMATH_H__
#include "ltmath.h"
#endif

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

#include <vector>

/*!
Base types.
*/
typedef unsigned int LTBOOL;
typedef float LTFLOAT;
typedef double LTDOUBLE;
typedef uint32 LTRESULT;

/*!
Vector types
*/
typedef std::vector<uint> TuintList;
typedef std::vector<uint8> Tuint8List;
typedef std::vector<uint16> Tuint16List;
typedef std::vector<uint32> Tuint32List;
typedef std::vector<int> TintList;
typedef std::vector<int8> Tint8List;
typedef std::vector<int16> Tint16List;
typedef std::vector<int32> Tint32List;
typedef std::vector<float> TfloatList;

/*!
Base type values (enumeration emulation).
LTBOOL.
*/
#define LTFALSE		0
#define LTTRUE		1


/*!
LTRESULT (defined in ltcodes.h).
*/



/*!
Forward declaration of LTObject.
*/
class LTObject;



/*!
Handles.
Object handle definition.
*/
typedef LTObject* HOBJECT;
typedef LTObject* HLOCALOBJ;

#define INVALID_HOBJECT ((LTObject *)0)

/*!
Model handles.
*/
typedef uint32 HMODELPIECE;
#define INVALID_MODEL_PIECE		((HMODELPIECE)-1)

typedef uint32 HMODELSOCKET;
#define INVALID_MODEL_SOCKET	((HMODELSOCKET)-1)

typedef uint32 HMODELNODE;
#define INVALID_MODEL_NODE		((HMODELNODE)-1)

typedef uint32 HMODELWEIGHTSET;
#define INVALID_MODEL_WEIGHTSET ((HMODELWEIGHTSET)-1)

typedef uint32 HMODELANIM;
#define INVALID_MODEL_ANIM		((HMODELANIM)-1)

class CMaterialInstance;
typedef CMaterialInstance* HMATERIAL;
#define INVALID_MATERIAL		((HMATERIAL)NULL)

//note that this class does not actually exist, but serves to provide typesafe
//access to textures
class CDummyTextureHandleClass;
typedef CDummyTextureHandleClass* HTEXTURE;
#define INVALID_TEXTURE			((HTEXTURE)NULL)

class CModelDecal;
typedef CModelDecal* HMODELDECAL;
#define INVALID_MODELDECAL		((HMODELDECAL)NULL)

/*!
Poly reference.
*/
struct HPOLY
{
	HPOLY() : m_nWorldIndex(0xFFFFFFFF), m_nPolyIndex(0xFFFFFFFF) {}
	HPOLY(uint32 nWorldIndex, uint32 nPolyIndex) : m_nWorldIndex(nWorldIndex), m_nPolyIndex(nPolyIndex) {}
	HPOLY(const HPOLY& hPoly) : m_nWorldIndex(hPoly.m_nWorldIndex), m_nPolyIndex(hPoly.m_nPolyIndex) {}
	
	HPOLY &operator=(const HPOLY &hOther)
	{
		m_nWorldIndex = hOther.m_nWorldIndex;
		m_nPolyIndex = hOther.m_nPolyIndex;
		return *this;
	}

	bool operator==(const HPOLY &hOther) const
	{
		return (m_nWorldIndex == hOther.m_nWorldIndex) && (m_nPolyIndex == hOther.m_nPolyIndex);
	}
	bool operator!=(const HPOLY &hOther) const
	{
		return (m_nWorldIndex != hOther.m_nWorldIndex) || (m_nPolyIndex != hOther.m_nPolyIndex);
	}

	uint32 m_nPolyIndex, m_nWorldIndex;
};
#define INVALID_HPOLY HPOLY(0xFFFFFFFF, 0xFFFFFFFF)

/*!
Color types (different byte ordering for cross platform development).
*/
struct LTRGB			{ uint8 b, g, r, a; };

// Helper-template for defining templatized floating point parameters
// Use TFloatType<T>::Type to retrieve an at-least-floating point type on a parameterized type value
template<class T> 
struct TFloatType {
	typedef float Type;
};

template<>
struct TFloatType<double> {
	typedef double Type;
};

/*!
More complicated types.
*/

#ifndef __LTVECTOR_H__
#include "ltvector.h"
#endif

typedef std::vector<LTVector> TLTVectorList;

#ifndef __LTROTATION_H__
#include "ltrotation.h"
#endif

typedef std::vector<LTRotation> TLTRotationList;

#ifndef __LTPOLARCOORD_H__
#include "ltpolarcoord.h"
#endif

#ifndef __LTPLANE_H__
#include "ltplane.h"
#endif

typedef std::vector<LTPlane> TLTPlaneList;

#ifndef __LTSPHERE_H__
#include "ltsphere.h"
#endif

typedef std::vector<LTSphere> TLTSphereList;

#ifndef __LTOBB_H__
#include "ltobb.h"
#endif

typedef std::vector<LTOBB> TLTOBBList;

#ifndef __LTRECT_H__
#include "ltrect.h"
#endif

#ifndef __LTMATRIX_H__
#include "ltmatrix.h"
#endif

typedef std::vector<LTMatrix> TLTMatrixList;

#ifndef __LTTRANSFORM_H__
#include "lttransform.h"
#endif

typedef std::vector<LTRigidTransform> TLTRigidTransformList;
typedef std::vector<LTTransform> TLTTransformList;

#ifndef __LTLINK_H__
#include "ltlink.h"
#endif

struct ArgList
{
	char **argv;
	int argc;
};


#endif //! __LTBASETYPES_H__










