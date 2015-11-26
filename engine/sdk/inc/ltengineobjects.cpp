

/*!

 MODULE  : ltengineobjects.cpp

 PURPOSE : C++ LT engine objects

 CREATED : 9/17/97

*/

#include "iltmessage.h"
#include "ltengineobjects.h"
#include <stdio.h>
#include "iltsoundmgr.h"
#include "iltcommon.h"
#include "iltphysics.h"
#include "ltserverobj.h"
#include "iobjectplugin.h"
#include "ltobjectcreate.h"

// Define all the game classes and the gameservershell.
DEFINE_CLASSES()

//Utility function that simplifies the adding of strings to a plugin object
struct SStringListParams
{
	char **m_aszStrings;
	uint32 *m_pcStrings;
	uint32 m_cMaxStrings;
	uint32 m_cMaxStringLength;
};

static void AddString(const char *pString, SStringListParams &sParams) 
{
	if (((*sParams.m_pcStrings) + 1) < sParams.m_cMaxStrings)
	{
		LTStrCpy(sParams.m_aszStrings[(*sParams.m_pcStrings)++], pString, sParams.m_cMaxStringLength);
	}
}

/*!

 BaseClass.

*/
BEGIN_CLASS(BaseClass)
	ADD_STRINGPROP(Name, "noname", "The name of this object")
	ADD_VECTORPROP_FLAG(Pos, PF_DISTANCE, "The position in the world of this object.")
	ADD_ROTATIONPROP(Rotation, "The orientation of this object represented by euler angles in degrees")
END_CLASS_NOPARENT(BaseClass, "The default base class for all objects")

BaseClass::~BaseClass()
{
}

uint32
BaseClass::EngineMessageFn(
	uint32 messageID, 
	void *pData, 
	float fData
	)
{
	ILTServer *pServerLT = g_pLTServer;

	//Handle ReadProp.
	if (messageID == MID_PRECREATE)
	{
		ObjectCreateStruct *pStruct = (ObjectCreateStruct*)pData;

		//If they haven't already set the type, set it to whatever the constructor set.
		if(pStruct->m_ObjectType == OT_NORMAL)
		{
			pStruct->m_ObjectType = m_nType;
		}

		//Get the props.
		if( fData == PRECREATE_WORLDFILE || fData == PRECREATE_STRINGPROP )
		{
			const GenericPropList *pProps = &pStruct->m_cProperties;
			
			const char *pName = pProps->GetString("Name", NULL);
			if( pName != NULL )
				LTStrCpy(pStruct->m_Name, pName, LTARRAYSIZE(pStruct->m_Name));

			pStruct->m_Pos		= pProps->GetVector("Pos", pStruct->m_Pos);
			pStruct->m_Rotation = pProps->GetRotation("Rotation", pStruct->m_Rotation);
			pStruct->m_Flags	= pProps->GetLongInt("Flags", (int32)pStruct->m_Flags);

			if( pProps->GetBool("Visible", (pStruct->m_Flags & FLAG_VISIBLE) != 0) )
				pStruct->m_Flags |= FLAG_VISIBLE;
			else
				pStruct->m_Flags &= ~FLAG_VISIBLE;

			if( pProps->GetBool("Solid", (pStruct->m_Flags & FLAG_SOLID) != 0) )
				pStruct->m_Flags |= FLAG_SOLID;
			else
				pStruct->m_Flags &= ~FLAG_SOLID;

			if( pProps->GetBool("Gravity", (pStruct->m_Flags & FLAG_GRAVITY) != 0) )
				pStruct->m_Flags |= FLAG_GRAVITY;
			else
				pStruct->m_Flags &= ~FLAG_GRAVITY;

			if( pProps->GetBool("TouchNotify", (pStruct->m_Flags & FLAG_TOUCH_NOTIFY) != 0) )
				pStruct->m_Flags |= FLAG_TOUCH_NOTIFY;
			else
				pStruct->m_Flags &= ~FLAG_TOUCH_NOTIFY;

			if( pProps->GetBool("Rayhit", (pStruct->m_Flags & FLAG_RAYHIT) != 0) )
				pStruct->m_Flags |= FLAG_RAYHIT;
			else
				pStruct->m_Flags &= ~FLAG_RAYHIT;
			
			uint32 nMaterial = 0;

			ConParse conParse;
			conParse.Init(pProps->GetString("Material", ""));
			while(pServerLT->Common()->Parse(&conParse) == LT_OK)
			{
				if(conParse.m_nArgs > 0)
				{
					pStruct->SetMaterial(nMaterial, conParse.m_Args[0]);
					nMaterial++;
				}

				if(nMaterial >= MAX_MATERIALS_PER_MODEL)
					break;
			}
			
			for(uint32 nCurrMaterial = 0; nCurrMaterial < MAX_MATERIALS_PER_MODEL; nCurrMaterial++)
			{
				char pszSkinPropName[32];
				LTSNPrintF(pszSkinPropName, LTARRAYSIZE(pszSkinPropName), "Material%d", nCurrMaterial);
				const char* pszMaterial = pProps->GetString(pszSkinPropName, "");

				if(LTStrLen(pszMaterial) > 0)
				{
					pStruct->SetMaterial(nCurrMaterial, pszMaterial);
				}
			}
		}
	}
	return ILTBaseClass::EngineMessageFn(messageID, pData, fData);
} // BaseClass::EngineMessageFn

/*!

 Decal

*/

class Decal : public BaseClass 
{
};


BEGIN_CLASS(Decal)
	
	ADD_VECTORPROP_VAL_FLAG(ProjectDims, 64, 64, 50, PF_ORTHOFRUSTUM, "The dimensions of the area the decal will project onto")
	ADD_REALPROP_FLAG(NearClip, 0.0f, 0, "The near clip plane where the decal will begin projection from")
	ADD_REALPROP_FLAG(Offset, 0.3f, 0, "The amount to offset from the surface when generating the decal geometry")
	ADD_BOOLPROP_FLAG(Occlude, false, 0, "Should the decal occlude itself when generating the surface.")
	ADD_BOOLPROP_FLAG(HitWorldModels, true, 0, "If true, this will cast decals onto world models, otherwise it will ignore them")
	ADD_STRINGPROP_FLAG(Material, "", PF_FILENAME, "The material that will be associated with the generated decal geometry")
	ADD_STRINGPROP(IgnoreBrushes, "", "A list of brushes that should be ignored. This can use wild card sets as explained in the decal documentation")

	PROP_DEFINEGROUP(TextureCoords, PF_GROUP(1), "Allows control over the texture coordinate generation of the decal")
		ADD_REALPROP_FLAG(UOffset0, 0.0f, PF_GROUP(1), "Specifies the U offset to apply to the corner of the first mapping")
		ADD_REALPROP_FLAG(VOffset0, 0.0f, PF_GROUP(1), "Specifies the V offset to apply to the corner of the first mapping")
		ADD_REALPROP_FLAG(URepeats0, 1.0f, PF_GROUP(1), "Specifies the number of times the texture should repeat in the U direction for the first mapping")
		ADD_REALPROP_FLAG(VRepeats0, 1.0f, PF_GROUP(1), "Specifies the number of times the texture should repeat in the V direction for the first mapping")
		ADD_REALPROP_FLAG(UOffset1, 0.0f, PF_GROUP(1), "Specifies the U offset to apply to the corner of the second mapping")
		ADD_REALPROP_FLAG(VOffset1, 0.0f, PF_GROUP(1), "Specifies the V offset to apply to the corner of the second mapping")
		ADD_REALPROP_FLAG(URepeats1, 1.0f, PF_GROUP(1), "Specifies the number of times the texture should repeat in the U direction for the second mapping")
		ADD_REALPROP_FLAG(VRepeats1, 1.0f, PF_GROUP(1), "Specifies the number of times the texture should repeat in the V direction for the second mapping")

END_CLASS_FLAGS( Decal, BaseClass, CF_NORUNTIME, "An object that will project onto geometry in the world and create a decal")

/*!

 Brush plug-in

*/

class CBrushPlugin : public IObjectPlugin
{
public:
	CBrushPlugin() {}

	virtual LTRESULT PreHook_EditStringList(
		const char* /*szRezPath*/,				
		const char* szPropName,				
		char** aszStrings,					
		uint32* pcStrings,	
		const uint32 cMaxStrings,		
		const uint32 cMaxStringLength)		
	{
	   if (!aszStrings || !pcStrings) 
		   return LT_UNSUPPORTED;

	   SStringListParams sParams;
	   sParams.m_aszStrings = aszStrings;
	   sParams.m_pcStrings = pcStrings;
	   sParams.m_cMaxStrings = cMaxStrings;
	   sParams.m_cMaxStringLength = cMaxStringLength;

	   if (LTStrIEquals(szPropName, "Type"))
	   {
		   AddString("Normal", sParams);
		   AddString("Sector", sParams);
		   AddString("ExtrudeSector", sParams);
		   AddString("SemiExtrudeSector", sParams);
		   AddString("RenderOnly", sParams);
		   AddString("NonSolid", sParams);
		   AddString("AINavMeshCarver", sParams);
		   AddString("Visualization", sParams);
		   AddString("ShadowMaker", sParams);
		   AddString("Blocker", sParams);
		   AddString("SkyPortal", sParams);
		   AddString("ParticleBlocker", sParams);
		   AddString("NonPhysics", sParams);
		   AddString("PhysicsOnly", sParams);
		   AddString("PhysicsWorldBsp", sParams);
		   AddString("WorldBspOnly", sParams);
	   }
	   else if (LTStrIEquals(szPropName, "TangentMethod"))
	   {
		   AddString("World", sParams);
		   AddString("Smooth", sParams);
		   AddString("Kaldera", sParams);
	   }
	   else if (LTStrIEquals(szPropName, "ShadowLOD"))
	   {
		   AddString("Low", sParams);
		   AddString("Medium", sParams);
		   AddString("High", sParams);
		   AddString("Never", sParams);
	   }
	   else
		   return LT_UNSUPPORTED;
	   
	   return LT_OK;
	}
};

/*!

 Brush.
 
*/

class Brush : public BaseClass 
{
};

BEGIN_CLASS(Brush)
	ADD_VECTORPROP_FLAG(Pos, PF_DISTANCE | PF_HIDDEN, "The position in the world of this object.")
	ADD_ROTATIONPROP_FLAG(Rotation, PF_HIDDEN, "The orientation of this object represented by euler angles in degrees")

	ADD_STRINGPROP_FLAG(Type, "Normal", PF_STATICLIST, "Indicates the type of brush which indicates what sort of behavior should be assoicated with it")

	ADD_BOOLPROP(NotAStep, 0, "Determines if this brush can act as a step when performing stair stepping with the player")

	ADD_BOOLPROP_FLAG(ClipLight, 1, 0, "Specifies if this brush should cast shadows or not")
	ADD_REALPROP_FLAG(CreaseAngle, 45.0f, 0, "The angle in degrees where it will not try and smooth polygons across an edge")
	ADD_STRINGPROP_FLAG(TangentMethod, "World", PF_STATICLIST, "Indicates the method that the tangent space will be generated for this brush. World should be used for most level geometry. Smooth should be used when it should be smoothed regardless of texture orientation such as pipes. Kaldera should be used whenever the brush is using a normal map created by Kaldera.")
	ADD_STRINGPROP_FLAG(ShadowLOD, "Low", PF_STATICLIST, "Indicates the LODs that this brush will cast shadows in. For example Low indicates cast a shadow in Low, Med, and High, while High only casts it in High. Never indicates that no shadow should be cast.")

END_CLASS_FLAGS_PLUGIN(Brush, BaseClass, CF_NORUNTIME | CF_HIDDEN, CBrushPlugin, "A collection of polygons and their associated properties")

/*!

 SkyPointer.
 This objects adds other objects to the sky.  Objects in the sky are rendered in indexed order.

*/

class SkyPointer : public BaseClass {
public:
	virtual uint32  EngineMessageFn(uint32 messageID, void *pData, float lData);
private:

	//the maximum number of objects we support
	static const uint32 knMaxObjects = 8;

	//the maximum length of an object name
	static const uint32 knObjectNameLen = 128;

	//the listing of the objects that we need to place in the sky
	char m_ObjectName[knMaxObjects][knObjectNameLen];
};

BEGIN_CLASS(SkyPointer)
	ADD_STRINGPROP_FLAG(SkyObject0, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject1, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject2, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject3, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject4, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject5, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject6, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
	ADD_STRINGPROP_FLAG(SkyObject7, "", PF_OBJECTLINK, "The name of the object that you want to render in the sky.")
END_CLASS_FLAGS(SkyPointer, BaseClass, 0, "An object that allows specifying another object for rendering in the sky")


uint32 SkyPointer::EngineMessageFn(uint32 messageID, void *pData, float fData)
{
	switch(messageID)
	{
		case MID_PRECREATE:
		{
			ObjectCreateStruct *pStruct = (ObjectCreateStruct*)pData;
			const GenericPropList *pProps = &pStruct->m_cProperties;
			pStruct->m_ObjectType = OT_NORMAL;

			if( fData == PRECREATE_WORLDFILE )
			{
				pStruct->SetFileName(pProps->GetString("Name", ""));

				char pszPropName[32];
				for(uint32 nCurrObject = 0; nCurrObject < knMaxObjects; nCurrObject++)
				{
					LTSNPrintF(pszPropName, LTARRAYSIZE(pszPropName), "SkyObject%d", nCurrObject);
					LTStrCpy(m_ObjectName[nCurrObject], pProps->GetString(pszPropName, ""), LTARRAYSIZE(m_ObjectName[nCurrObject]));
				}
			}
			else
			{
				for(uint32 nCurrObject = 0; nCurrObject < knMaxObjects; nCurrObject++)
					m_ObjectName[nCurrObject][0] = '\0';
			}
			break;
		}

		case MID_ALLOBJECTSCREATED:
		{
			ObjArray <HOBJECT, 1> objArray;

			for(uint32 nCurrObject = 0; nCurrObject < knMaxObjects; nCurrObject++)
			{
				if(!LTStrEmpty(m_ObjectName[nCurrObject]))
				{
					g_pLTServer->FindNamedObjects(m_ObjectName[nCurrObject], objArray);
					if(objArray.NumObjects() > 0)
					{
						HOBJECT hObject = objArray.GetObject(0);						
						g_pLTServer->Common()->SetObjectFlags(hObject, OFT_Flags, FLAG_NOTINWORLDTREE, FLAG_NOTINWORLDTREE);
						g_pLTServer->Common()->SetObjectFlags(hObject, OFT_Flags2, FLAG2_SKYOBJECT, FLAG2_SKYOBJECT);
					}
				}
			}

			g_pLTServer->RemoveObject(m_hObject);
			break;
		}
	}

	return BaseClass::EngineMessageFn(messageID, pData, fData);
}

/*!

SkyCamera.
This object provides the base transform and relative orientation when rendering the sky in the scene

*/

class SkyCamera : public BaseClass {
public:
	virtual uint32  EngineMessageFn(uint32 messageID, void *pData, float lData);
};


BEGIN_CLASS(SkyCamera)
END_CLASS_FLAGS(SkyCamera, BaseClass, 0, "An object that indicates the base position and relative rotation to be used when rendering the sky.")


uint32 SkyCamera::EngineMessageFn(uint32 messageID, void *pData, float fData)
{
	switch(messageID)
	{
	case MID_PRECREATE:
		{
			ObjectCreateStruct *pStruct = (ObjectCreateStruct*)pData;
			const GenericPropList *pProps = &pStruct->m_cProperties;

			//read in the transform of this object from the properties
			LTRigidTransform tSkyCamera;
			tSkyCamera.m_vPos = pProps->GetVector("Pos", LTVector::GetIdentity());
			tSkyCamera.m_rRot = pProps->GetRotation("Rotation", LTRotation::GetIdentity());

			//install this as the transform of the sky camera
			g_pLTServer->SetSkyCamera(tSkyCamera);

			//and at this point we can fail the creation, which will allow us to be destroyed
			return 0;
		}
		break;
	}

	return BaseClass::EngineMessageFn(messageID, pData, fData);
}

/*!
 
 MainWorld.

*/

class MainWorld : public BaseClass {
public:
	virtual uint32  EngineMessageFn(uint32 messageID, void *pData, float lData);

};

BEGIN_CLASS(MainWorld)
END_CLASS_FLAGS(MainWorld, BaseClass, CF_DONTSAVE | CF_HIDDEN, "Internal world class")


uint32 MainWorld::EngineMessageFn(uint32 messageID, void *pData, float fData)
{
	// Turn off updates for the main world
	if (messageID == MID_OBJECTCREATED)
	{
		g_pLTServer->SetObjectState(m_hObject, OBJSTATE_INACTIVE);
	}

	return BaseClass::EngineMessageFn(messageID, pData, fData);
}

/*!

ShadowOverride.
This object is for tools purposes and contains the ability to override shadow LOD settings on brushes
and world models that are contained beneath it in the node hierarchy

*/

class CShadowOverridePlugin : public IObjectPlugin
{
public:
	virtual LTRESULT PreHook_EditStringList(
		const char* /*szRezPath*/,				
		const char* szPropName,				
		char** aszStrings,					
		uint32* pcStrings,	
		const uint32 cMaxStrings,		
		const uint32 cMaxStringLength)		
	{
		if (!aszStrings || !pcStrings) 
			return LT_UNSUPPORTED;

		SStringListParams sParams;
		sParams.m_aszStrings = aszStrings;
		sParams.m_pcStrings = pcStrings;
		sParams.m_cMaxStrings = cMaxStrings;
		sParams.m_cMaxStringLength = cMaxStringLength;

		if (LTStrIEquals(szPropName, "BrushOverride") || 
			LTStrIEquals(szPropName, "WMOverride"))
		{
			AddString("Low", sParams);
			AddString("Medium", sParams);
			AddString("High", sParams);
			AddString("Never", sParams);
			AddString("No Override", sParams);
		}
		else
			return LT_UNSUPPORTED;

		return LT_OK;
	}
};

class ShadowOverride : public BaseClass {
public:
};

BEGIN_CLASS(ShadowOverride)
	ADD_STRINGPROP_FLAG(BrushOverride, "No Override", PF_STATICLIST, "Indicates the shadow LOD that should be set on brushes beneath this object in the hierarchy. This will not effect brushes if set to 'No Override'")
	ADD_STRINGPROP_FLAG(WMOverride, "No Override", PF_STATICLIST, "Indicates the shadow LOD that should be set on world models beneath this object in the hierarchy. This will not effect world models if set to 'No Override'")
END_CLASS_FLAGS_PLUGIN(ShadowOverride, BaseClass, CF_NORUNTIME, CShadowOverridePlugin, "This object allows overriding shadow LOD settings on brushes and world models that are contained beneath it in the node hierarchy")

/*!

/*!

StreamingInfo.
This object serves as a placeholder to indicate the total memory limits available for the level
and also the position from which it should start streaming data at level load

*/

class StreamingInfo : public BaseClass {
public:
};

BEGIN_CLASS(StreamingInfo)
	ADD_REALPROP_FLAG(GlobalLimit, 50.0f, 0, "Indicates in megabytes the maximum amount of global resources that can be loaded for this level")
	ADD_REALPROP_FLAG(StreamingLimit, 150.0f, 0, "Indicates in megabytes the maximum amount of memory that will be set aside for all simultaniously loaded streaming regions")
END_CLASS_FLAGS(StreamingInfo, BaseClass, CF_NORUNTIME | CF_HIDDEN, "This object indicates both the total memory limits that exist for the level but also uses the positioning of this object to determine which streaming regions should be loaded when the level is initially started. Typically this means the object should be placed next to the starting position of the player but can be different for cinematics or special conditions.")

/*!

StreamingRegion.
This object serves as a placeholder to indicate to the tools that all brushes below this define a
streaming region

*/

class StreamingRegion : public BaseClass {
public:
};

BEGIN_CLASS(StreamingRegion)
	ADD_REALPROP_FLAG(MemoryLimit, 75.0f, 0, "Indicates in megabytes the amount of memory that can be consumed by resources for this streaming region")
END_CLASS_FLAGS(StreamingRegion, BaseClass, CF_NORUNTIME | CF_HIDDEN, "This object indicates that the child brushes define a region of space that should be used as a streaming region")

/*!

StreamingLink.
This object serves as a placeholder to indicate to the tools that all brushes below this define a
linking region that link to another streaming region

*/

class StreamingLink : public BaseClass {
public:
};

BEGIN_CLASS(StreamingLink)
	ADD_STRINGPROP_FLAG(LinkTo, "", PF_OBJECTLINK, "This property indicates the streaming region that this volume of space should start preloading. This must not be the same streaming region that this linking region is a child of")
END_CLASS_FLAGS(StreamingLink, BaseClass, CF_NORUNTIME | CF_HIDDEN, "This object indicates that the child brushes define a region of space wherein the linked streaming region should begin prefetching. This must be placed beneath a streaming region, and the child brushes are treated as children of that streaming region as well")

/*!

ObjectResourceArea.

*/

class ObjectResourceArea : public BaseClass {
public:
};

BEGIN_CLASS(ObjectResourceArea)
	ADD_STRINGPROP_FLAG(StreamingRegions, "", 0, "This property is a comma-delimited list of streaming regions that all child objects and resources will be needed.")
	ADD_VECTORPROP_VAL_FLAG(Dims, 0.0f, 0.0f, 0.0f, PF_DIMS | PF_LOCALDIMS, "Half dimensions of this resource container.")
	ADD_BOOLPROP_FLAG(GlobalResourceOverride, false, 0, "Should all child objects have their resources put in the global streaming region, ignoring the individual child object settings?")
END_CLASS_FLAGS(ObjectResourceArea, BaseClass, CF_NORUNTIME | CF_HIDDEN, "This organizational object indicates that all child objects will inherit the streaming properties of this container.")


/*!

 PhysicsShapeSphere.
 
*/

class PhysicsShapeSphere : public BaseClass {
public:
};

BEGIN_CLASS(PhysicsShapeSphere)
	ADD_REALPROP_FLAG(Radius, 100.0f, PF_RADIUS, "The radius of this physics sphere in centimeters")
	ADD_REALPROP_FLAG(MassKg, 1.0f, 0, "The mass of this physical shape in kilograms")
	ADD_REALPROP_FLAG(DensityG, 1.1f, 0, "The density of this object measured in grams per cubic centimeter that is used for buoyancy. Water has a density of 1 gram per cubic centimeter.")
END_CLASS_FLAGS(PhysicsShapeSphere, BaseClass, CF_NORUNTIME, "A physics shape that can be placed beneath world models to add a sphere to the world model's physical shape")

/*!

 PhysicsShapeOBB.
 
*/

class PhysicsShapeOBB : public BaseClass {
public:
};

BEGIN_CLASS(PhysicsShapeOBB)
	ADD_VECTORPROP_VAL_FLAG(Dims, 20.0f, 20.0f, 20.0f, PF_DIMS | PF_LOCALDIMS, "The half dimensions of this physics box in centimeters")
	ADD_REALPROP_FLAG(MassKg, 1.0f, 0, "The mass of this physical shape in kilograms")
	ADD_REALPROP_FLAG(DensityG, 1.1f, 0, "The density of this object measured in grams per cubic centimeter that is used for buoyancy. Water has a density of 1 gram per cubic centimeter.")
END_CLASS_FLAGS(PhysicsShapeOBB, BaseClass, CF_NORUNTIME, "A physics shape that can be placed beneath world models to add an oriented bounding box to the world model's physical shape")

/*!

 PhysicsShapeCapsule.
 
*/

class PhysicsShapeCapsule : public BaseClass {
public:
};

BEGIN_CLASS(PhysicsShapeCapsule)
	ADD_REALPROP_FLAG(Radius, 100.0f, PF_RADIUS, "The radius of this physics capsule in centimeters")
	ADD_REALPROP_FLAG(Length, 100.0f, 0, "The length of the main cylinder of the capsule in centimeters. This is aligned with the up axis of the object.")
	ADD_REALPROP_FLAG(MassKg, 1.0f, 0, "The mass of this physical shape in kilograms")
	ADD_REALPROP_FLAG(DensityG, 1.1f, 0, "The density of this object measured in grams per cubic centimeter that is used for buoyancy. Water has a density of 1 gram per cubic centimeter.")
END_CLASS_FLAGS(PhysicsShapeCapsule, BaseClass, CF_NORUNTIME, "A physics shape that can be placed beneath world models to add a capsule primitive to the world model. This capsule is centered around the object position and extends along the up axis of the object.")

