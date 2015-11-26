//----------------------------------------------------------------------------------------------------
// IDatabaseCreatorMgr.h
//
// Provides the definition for the IDatabaseCreator interface. This interface is used to create 
// databases which can be saved to the binary format. These databases that are opened for creation
// cannot be used for reading, only writing.
//
//----------------------------------------------------------------------------------------------------

#ifndef __IDATABASECREATORMGR_H__
#define __IDATABASECREATORMGR_H__

//we rely upon the attribute type enumeration and the handle definitions from DatabaseMgr.h
#ifndef __IDATABASEMGR_H__
#	include "idatabasemgr.h"
#endif

//forward declarations
class ILTOutConverter;

//IDatabaseMgr interface for managing game databases
class IDatabaseCreatorMgr
{
public:

	virtual ~IDatabaseCreatorMgr()  {}

	//------------------------------------
	// Database Management
	//------------------------------------

	//This will create a new database for construction. This returned handle must be freed with
	//ReleaseDatabase otherwise memory will be leaked.
	virtual HDATABASECREATOR	CreateNewDatabase() = 0;

	//called to release a reference to the database. Once this call is made, none of the category,
	//record, or attribute handles that were obtained from this database are valid and must not be used.
	virtual void				ReleaseDatabase(HDATABASECREATOR hDatabase) = 0;

	//called to save the specified database to the provided filename. This will return true if
	//successful, and false otherwise
	virtual bool				SaveDatabase(HDATABASECREATOR hDatabase, ILTOutConverter& OutConverter) = 0;

	//------------------------------------
	// Category Management
	//------------------------------------

	//this will create a category with the specified name. It will return a valid category handle if
	//successful that can be used to modify the category, or return NULL if unsuccessful
	virtual HCATEGORYCREATOR	CreateCategory(HDATABASECREATOR hDatabase, const char* pszCategoryName, const char* pszSchemaName) = 0;

	//------------------------------------
	// Record Management
	//------------------------------------

	//this will create a record with the specified name. It will return a valid record handle if
	//successful that can be used to modify the category, or return NULL if unsuccessful
	virtual HRECORDCREATOR		CreateRecord(HCATEGORYCREATOR hCategory, const char* pszRecordName) = 0;

	//------------------------------------
	// Attribute Management
	//------------------------------------

	//this will create an attribute with the specified name, type, and number of values. This will
	//return NULL if the attribute cannot be created.
	virtual HATTRIBUTECREATOR	CreateAttribute(HRECORDCREATOR hRecord, const char* pszAttributeName,
												EAttributeType eType, EAttributeUsage eUsage, uint32 nNumValues) = 0;

	//------------------------------------
	// Value Management
	//------------------------------------

	//specifies a value for an integer attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetInt32(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, int32 nValue) = 0;

	//specifies a value for a float attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetFloat(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, float fValue) = 0;

	//specifies a value for a string attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetString(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, const char* pszValue) = 0;

	//specifies a value for a unicode string attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetWString(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, const wchar_t* pszValue) = 0;

	//specifies a value for a vector 2 attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetVector2(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, const LTVector2& vValue) = 0;

	//specifies a value for a vector 3 attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetVector3(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, const LTVector& vValue) = 0;

	//specifies a value for a vector 4 attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetVector4(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, const LTVector4& vValue) = 0;

	//specifies a value for a boolean attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetBool(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, bool bValue) = 0;

	//specifies a value for a record link attribute type. This will return false if the attribute is
	//of a different type, is invalid, or the value index is outside of range.
	virtual bool			SetRecordLink(HATTRIBUTECREATOR hAttribute, uint32 nValueIndex, HRECORDCREATOR hRecord) = 0;
};


#endif
