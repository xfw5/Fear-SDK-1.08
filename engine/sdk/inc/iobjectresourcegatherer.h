//--------------------------------------------------------------------------------------------------
// IObjectResourceGatherer.h
//
// Provides the definition for the IObjectResourceGatherer interface, which is implemented by the
// prefetching tools in order to hook into the game code at compile time and allow the game objects
// to examine themselves and report a listing of which resources are used and where in order to 
// determine all of the resources that should be prefetched for different areas of the world
//
//--------------------------------------------------------------------------------------------------
#ifndef __IOBJECTRESOURCEGATHERER_H__
#define __IOBJECTRESOURCEGATHERER_H__

//--------------------------------------------------------------------------------------------------

class IObjectResourceGatherer
{
public:

	IObjectResourceGatherer()			{}
	virtual ~IObjectResourceGatherer()	{}

	//-----------------------------------------
	// Region Support
	//-----------------------------------------

	//provide the relative path name of the world with extension
	virtual const char*	GetWorldName() = 0;

	//provide support for region iteration. Note that this does not include the 'Global' region
	virtual uint32		GetNumRegions() = 0;
	virtual const char*	GetRegionName(uint32 nRegion) = 0;

	//given a region, this will determine if it contains a sky portal or not. This will return false
	//if the region name is not valid
	virtual bool		DoesRegionHaveSkyPortal(const char* pszRegion) = 0;

	//given a region, this will determine the number of surface types that are included in that
	//region. Note that this can increase if assets are added to the streaming region
	virtual uint32		GetNumRegionSurfaceTypes(const char* pszRegion) = 0;

	//given a region, and an index into the surface types, this will return the surface type of
	//that index. This will return zero if the region or index is invalid
	virtual uint32		GetRegionSurfaceType(const char* pszRegion, uint32 nIndex) = 0;

	//-----------------------------------------
	// Resource Areas
	//-----------------------------------------

	//called to determine if an object is below an object resource area
	//returns false if no area is found.
	virtual bool		ContainedByResourceArea(const char* pszObject) = 0;

	//the return value for these functions is undefined if no resource area exists
	//for the specified object.

	//returns the name of the resource area that the specified object is below.
	virtual const char*	GetResourceAreaName(const char* pszObject) = 0;

	//returns whether the resource area for the specified object is configured to make resources global
	virtual bool		GetResourceAreaIsGlobal(const char* pszObject) = 0;

	//called to determine if the resource area for this object has non-zero dimensions
	virtual bool		ResourceAreaHasDimensions(const char* pszObject) = 0;

	//returns the actual dimensions of the resource area.
	virtual void		GetResourceAreaDimensions(const char* pszObject, LTRect3f& rDims) = 0;

	//these functions allow iteration through the list of streaming regions that the resource area was configured with.
	virtual uint32		GetNumResourceAreaRegions(const char* pszObject) = 0;
	virtual const char*	GetResourceAreaRegionName(const char* pszObject, uint32 nRegion) = 0;

	//called to report a warning with the asset gathering. This should be used for significant issues
	//that should be corrected but would not hold up further resource gathering
	virtual void		WarningMsg(const char* pszError, ...) = 0;

	//called to report an error with the asset gathering. This should be used for critical issues
	//that could prevent further resource gathering
	virtual void		ErrorMsg(const char* pszError, ...) = 0;

	//-----------------------------------------
	// Asset Reporting
	//-----------------------------------------

	//called to add a specific resource to the specified region. Note that the region
	//name can be left NULL to indicate the global region
	virtual bool		AddResourceToRegion(const char* pszResource, const char* pszRegion) = 0;

	//called to add the specified world model's resources to the specified region. Note that the region
	//name can be left NULL to indicate the global region
	virtual bool		AddWorldModelToRegion(const char* pszWorldModel, const char* pszRegion) = 0;

	//called to add the specified resource to the rectangular region provided. This takes an OBB 
	//representation but internally is free to convert it to an encompassing AABB representation. This
	//will add the resource to the global region if no other regions touch it
	virtual bool		AddResourceToOBB(const char* pszResource, const LTRigidTransform& tCenter, const LTVector& vHalfDims) = 0;

	//called to add the specified resource to the rectangular region provided. This
	//will add the resource to the global region if no other regions touch it
	virtual bool		AddResourceToAABB(const char* pszResource, const LTRect3f& rAABB) = 0;

	//called to add the resources associated with the specified world model to the AABB provided.
	virtual bool		AddWorldModelToAABB(const char* pszWorldModel, const LTRect3f& rAABB) = 0;

	//called to get the transform for the specified object (don't access the position and rotation properties
	//because this will handle the appropriate prefab transformations as well)
	virtual LTRigidTransform	GetTransform(const char* pszObject) = 0;

	//called to get the transform of the parent prefab. This might be needed for transforming of
	//properties or other data for objects within prefabs
	virtual LTRigidTransform	GetPrefabTransform(const char* pszObject) = 0;

	//called to get the world space AABB for the world model. This will fail if the object can't
	//be found or if no vertices are contained beneath the world model. The rectangle is undefined
	//if the call does not succeed
	virtual bool		GetWorldModelAABB(const char* pszWorldModel, LTRect3f& rRect) = 0;

	//-----------------------------------------
	// Object Property Support
	//
	// All of these will return the value of the property as long as the property exists
	// for the provided object and is of the appropriate type. If not, it will return the default
	// value provided. Also supports determination of whether or not an object is valid or if
	// a property exists. Note that these functions will handle replacing # with the current prefab nam
	// for string and command properties.
	//
	// For the property types not listed here but have ADD_XXXXPROP macros, the mappings are:
	// Rotation->Vector
	// Color->Vector
	// Object->String
	//-----------------------------------------

	virtual bool		GetPropBool(const char* pszObject, const char* pszProp, bool bDefault) = 0;
	virtual int32		GetPropLongInt(const char* pszObject, const char* pszProp, int32 nDefault) = 0;
    virtual float		GetPropReal(const char* pszObject, const char* pszProp, float fDefault) = 0;
	virtual LTVector	GetPropVector(const char* pszObject, const char* pszProp, const LTVector& vDefault) = 0;

	//these two properties unfortunately must operate in a different manner than the above since the 
	//strings must be formatted so that they do not contain any # characters. Therefore a buffer
	//must be provided to fill the properties into
	virtual bool		GetPropString(const char* pszObject, const char* pszProp, char* pszBuffer, uint32 nBuffSize, const char* pszDefault) = 0;
	virtual bool		GetPropCommand(const char* pszObject, const char* pszProp, char* pszBuffer, uint32 nBuffSize, const char* pszDefault) = 0;

	//given the name of an object, this will return the class of the object, or NULL if the object
	//cannot be found
	virtual const char*	GetObjectClass(const char* pszObject) = 0;

	//given a class name, this will find all of the names of the objects that use that class. This will
	//return the number of objects added to the array, and can optionally take the number of objects
	//found that use that class. The returned value will always be <= the size of the list whereas
	//pnTotalObjectsOfClass will be the total number of objects of that class regardless of the list
	//size
	virtual uint32		GetObjectsOfClass(const char* pszClass, const char** pszOutObjectList, uint32 nObjectListSize, uint32* pnTotalObjectsOfClass) = 0;

	//given the name of an object, this will determien whether or not that object exists
	virtual bool		DoesObjectExist(const char* pszObject) = 0;

	//given a property, this will determine if the property exists
	virtual bool		DoesPropExist(const char* pszObject, const char* pszProp) = 0;
};

#endif
