//////////////////////////////////////////////////////////////////////////////
// Object creation structure declaration and related constants and macros

#ifndef __LTOBJECTCREATE_H__
#define __LTOBJECTCREATE_H__

#include "ltproperty.h"
#include "ltphysicsgroup.h"

/*!
This structure is used when creating objects.  When you want to
create an object, you call ILTServer::CreateObject with one of these.
The structure you pass in is passed to the object's PostPropRead function,
where it can override whatever it wants to.
*/
struct ObjectCreateStruct 
{

    ObjectCreateStruct() 
	{
        Clear();
    }

    void Clear() 
	{

        m_ObjectType			= 0;
        m_ContainerCode			= 0;
        m_Flags					= 0;
        m_Flags2				= 0;
        m_UserData				= 0;
        m_hClass				= 0;
        m_NextUpdate			= 0.0f;
		m_Scale					= 1.0f;

		// Default to an invalid value.  If this is not set, then 
		// the default engine behavior will be used.
		m_eGroup				= ePhysicsGroup_MaxGroups;
		
        m_Name[0]				= '\0';
        m_Filename[0]			= '\0';

        uint32 i;
		for (i=0; i < MAX_CHILD_MODELS ; i++ )
		{
			m_ChildModels[i][0] = '\0';
		}

		for (i=0; i < MAX_MATERIALS_PER_MODEL; i++ )
		{
			m_Materials[i][0] = '\0';
		}
        
        m_Rotation.Init();
        m_Pos.Init();

		m_cProperties.Reset();
    }

/*!
Functions to aid in setting of the string values. This helps ensure that buffers
don't overflow
*/
	void SetName(const char* pszName)						{ LTStrCpy(m_Name, pszName, LTARRAYSIZE(m_Name)); }
	void SetFileName(const char* pszName)					{ LTStrCpy(m_Filename, pszName, LTARRAYSIZE(m_Filename)); }
	void SetChildModel(uint32 nFile, const char* pszName)	{ LTASSERT(nFile < MAX_CHILD_MODELS, "Invalid File Index"); LTStrCpy(m_ChildModels[nFile], pszName, LTARRAYSIZE(m_ChildModels[nFile])); }
	void SetMaterial(uint32 nMat, const char* pszName)		{ LTASSERT(nMat < MAX_MATERIALS_PER_MODEL, "Invalid Material Index"); LTStrCpy(m_Materials[nMat], pszName, LTARRAYSIZE(m_Materials[nMat])); }

/*!
Main info.
*/
    uint16 m_ObjectType;

/*!
Container code if it's a container.  It's in here because
you can only set it at creation time.
*/
    uint16 m_ContainerCode;

    uint32 m_Flags;
    uint32 m_Flags2;

	LTVector	m_Pos;
    float		m_Scale;
    LTRotation	m_Rotation;

	// Initial physics group to belong to.
	EPhysicsGroup m_eGroup;

//! User data.
    uint32 m_UserData;

/*!
NOTE: this is ONLY used during OnPreCreate processing, and is filled in by the engine.
*/
    HCLASS m_hClass;

/*!
This is the model, or WorldModel name. This can be zero-length when it's not needed.
*/
    char m_Filename[MAX_CS_FILENAME_LEN + 1]; 

/*!
This is a listing of the child models that should be bound to this model. These can be left empty.
*/
	char m_ChildModels[MAX_CHILD_MODELS][MAX_CS_FILENAME_LEN + 1];

/*!
A list of materials that will be copied and used for the object
*/
	char m_Materials[MAX_MATERIALS_PER_MODEL][MAX_CS_FILENAME_LEN + 1];

/*!
Server only info.
*/

//! This object's name.
    char m_Name[MAX_CS_FILENAME_LEN + 1];

//! This will be the object's starting NextUpdate.
    float m_NextUpdate;

//! List of properties
	GenericPropList m_cProperties;
};

#endif //__LTOBJECTCREATE_H__
