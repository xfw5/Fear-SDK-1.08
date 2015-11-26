//-----------------------------------------------------------------------------------
// LTAABBUtils.h
//
// This file contains a collection of utilities for dealing with axis-aligned bounding
// boxes and planes. These include quickly determining which point is closest to a plane
// and also determining which side of a plane an AABB lies on.
//
//-----------------------------------------------------------------------------------

#ifndef __LTAABBUTILS_H__
#define __LTAABBUTILS_H__

//-----------------------------------------------------------------------------
// AABB to plane classifcation utilities
//-----------------------------------------------------------------------------

//An enumeration that refers to the different points of an AABB
enum EAABBCorner 
{
	eAABB_NearTopLeft		= 0,
	eAABB_NearTopRight		= 1,
	eAABB_NearBottomLeft	= 2,
	eAABB_NearBottomRight	= 3,
	eAABB_FarTopLeft		= 4,
	eAABB_FarTopRight		= 5,
	eAABB_FarBottomLeft		= 6,
	eAABB_FarBottomRight	= 7,
	eAABB_None				= 8,
};

// Get the closest AABB corner for a plane with the given normal
inline EAABBCorner GetAABBPlaneCorner(const LTVector &vNormal)
{
	//the way this function works is by finding out the most dominant diagonal facing, which can
	//be one of the following 8 vectors that corresponds to the equivelant value:
	//0: (-1.0f, 1.0f, -1.0f), Near top-left
	//1: (1.0f, 1.0f, -1.0f), Near top-right
	//2: (-1.0f, -1.0f, -1.0f), Near bottom-left
	//3: (1.0f, -1.0f, -1.0f), Near bottom-right
	//4: (-1.0f, 1.0f, 1.0f), Far top-left
	//5: (1.0f, 1.0f, 1.0f), Far top-right
	//6: (-1.0f, -1.0f, 1.0f), Far bottom-left
	//7: (1.0f, -1.0f, 1.0f), Far bottom-right
	//so essentially we just want to find out which one maximizes the dot product which is
	//basically just figuring out which one makes all components positive, so instead we can
	//just do a check on the sign of each component and create an index into the list
	uint32 nBestCorner = 0;

	if(vNormal.x >= 0.0f)
		nBestCorner += 1;
	if(vNormal.y < 0.0f)
		nBestCorner += 2;
	if(vNormal.z >= 0.0f)
		nBestCorner += 4;

	return (EAABBCorner)nBestCorner;
}

// Get the position of the provided AABB corner index
inline LTVector GetAABBCornerPos(EAABBCorner nCorner, const LTRect3f& rBox)
{
	//we can use the fact that a rect is stored as vMin, vMax in order to perform this
	//function with no logic as we can just treat it as an array of floats and index based upon the corner
	static const uint32 knMinX = 0, knMinY = 1, knMinZ = 2;
	static const uint32 knMaxX = 3, knMaxY = 4, knMaxZ = 5;

    //this works by just using indices to map to the components and then indexing to those
	//indices with the corner
	static const uint32 knIndexTable[8][3] =
	{
		{ knMinX, knMaxY, knMinZ },		//eAABB_NearTopLeft
		{ knMaxX, knMaxY, knMinZ },		//eAABB_NearTopRight
		{ knMinX, knMinY, knMinZ },		//eAABB_NearBottomLeft
		{ knMaxX, knMinY, knMinZ },		//eAABB_NearBottomRight
		{ knMinX, knMaxY, knMaxZ },		//eAABB_FarTopLeft
		{ knMaxX, knMaxY, knMaxZ },		//eAABB_FarTopRight
		{ knMinX, knMinY, knMaxZ },		//eAABB_FarBottomLeft
		{ knMaxX, knMinY, knMaxZ },		//eAABB_FarBottomRight
	};

	//now treat the rectangle as a float array, and build up our return vector
	const float* pRect = (const float*)&rBox;
	return LTVector(pRect[knIndexTable[nCorner][0]], pRect[knIndexTable[nCorner][1]], pRect[knIndexTable[nCorner][2]]);
}

// Get the position of the provided AABB corner index
inline LTVector GetAABBCornerPos(EAABBCorner nCorner, const LTVector &vBoxMin, const LTVector &vBoxMax)
{
	switch (nCorner)
	{
		case eAABB_NearTopLeft :
			return LTVector(vBoxMin.x, vBoxMax.y, vBoxMin.z);
		case eAABB_NearTopRight :
			return LTVector(vBoxMax.x, vBoxMax.y, vBoxMin.z);
		case eAABB_NearBottomLeft :
			return LTVector(vBoxMin.x, vBoxMin.y, vBoxMin.z);
		case eAABB_NearBottomRight :
			return LTVector(vBoxMax.x, vBoxMin.y, vBoxMin.z);
		case eAABB_FarTopLeft :
			return LTVector(vBoxMin.x, vBoxMax.y, vBoxMax.z);
		case eAABB_FarTopRight :
			return LTVector(vBoxMax.x, vBoxMax.y, vBoxMax.z);
		case eAABB_FarBottomLeft :
			return LTVector(vBoxMin.x, vBoxMin.y, vBoxMax.z);
		case eAABB_FarBottomRight :
			return LTVector(vBoxMax.x, vBoxMin.y, vBoxMax.z);
		default :
			LTASSERT(false, "Error: Invalid box corner requested in GetAABBCornerPos");
			return vBoxMin;
	}
}

// Get the AABB corner index
inline EAABBCorner GetAABBOtherCorner(EAABBCorner nCorner)
{
	static EAABBCorner aTransCorner[8] = {
		eAABB_FarBottomRight,
		eAABB_FarBottomLeft,
		eAABB_FarTopRight,
		eAABB_FarTopLeft,
		eAABB_NearBottomRight,
		eAABB_NearBottomLeft,
		eAABB_NearTopRight,
		eAABB_NearTopLeft
	};

	LTASSERT((uint32)nCorner < 8, "Error: Invalid corner provided to GetAABBOtherCorner");
	return aTransCorner[nCorner];
}

// Determine the side of the plane that the box is on
inline PolySide GetAABBPlaneSide(EAABBCorner eCorner, const LTPlane &cPlane, const LTVector &vBoxMin, const LTVector &vBoxMax)
{
	float fNearDist = cPlane.DistTo(GetAABBCornerPos(eCorner, vBoxMin, vBoxMax));
	if (fNearDist < 0.0f)
		return BackSide;
	float fFarDist = cPlane.DistTo(GetAABBCornerPos(GetAABBOtherCorner(eCorner), vBoxMin, vBoxMax));
	if (fFarDist > 0.0f)
		return FrontSide;
	else
		return Intersect;	
}

// Short-cut to determine if the box is on the back side of the plane
inline bool GetAABBPlaneSideBack(EAABBCorner eCorner, const LTPlane &cPlane, const LTVector &vBoxMin, const LTVector &vBoxMax)
{
	float fNearDist = cPlane.DistTo(GetAABBCornerPos(eCorner, vBoxMin, vBoxMax));
	return fNearDist < 0.0f;
}

inline bool GetAABBPlaneSideBack(EAABBCorner eCorner, const LTPlane &cPlane, const LTRect3f &rBox)
{
	float fNearDist = cPlane.DistTo(GetAABBCornerPos(eCorner, rBox));
	return fNearDist < 0.0f;
}

// Short-cut to determine if the box is on the front side of the plane
inline bool GetAABBPlaneSideFront(EAABBCorner eCorner, const LTPlane &cPlane, const LTVector &vBoxMin, const LTVector &vBoxMax)
{
	float fFarDist = cPlane.DistTo(GetAABBCornerPos(GetAABBOtherCorner(eCorner), vBoxMin, vBoxMax));
	return fFarDist > 0.0f;
}

inline bool GetAABBPlaneSideFront(EAABBCorner eCorner, const LTPlane &cPlane, const LTRect3f &rBox)
{
	float fFarDist = cPlane.DistTo(GetAABBCornerPos(GetAABBOtherCorner(eCorner), rBox));
	return fFarDist > 0.0f;
}

//-----------------------------------------------------------------------------
// CAABBPlane
//
// Provides a utility class that encompasses a plane and an AABB corner. This
// corner represents the closest corner in an AABB to the front side of this plane
// which can be used for efficient AABB culling. 
//-----------------------------------------------------------------------------
class CAABBPlane
{
public:

	//constructors
	CAABBPlane()								{}
	CAABBPlane(const LTPlane& Plane)			{ SetPlane(Plane); }
	CAABBPlane(const CAABBPlane& Plane) :		m_Plane(Plane.m_Plane), m_eCorner(Plane.m_eCorner) {}

	//operators
	CAABBPlane& operator=(const LTPlane& Plane)	{ SetPlane(Plane); return *this; }
    
	//provides access to the plane and the closest AABB corner
	const LTPlane&		GetPlane() const		{ return m_Plane; }
	EAABBCorner			GetCorner() const		{ return m_eCorner; }

	//called to set the plane. This will also update the corner variable as well
	void SetPlane(const LTPlane& Plane)
	{
		m_Plane = Plane;
		m_eCorner = GetAABBPlaneCorner(Plane.Normal());
	}

private:

	//the plane portion of this AABB plane
	LTPlane			m_Plane;

	//the AABB corner that is closest to the front side of this plane
	EAABBCorner		m_eCorner;
};

#endif //__LTAABBUTILS_H__