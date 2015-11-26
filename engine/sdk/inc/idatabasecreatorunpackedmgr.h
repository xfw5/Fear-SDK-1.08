//----------------------------------------------------------------------------------------------------
// IDatabaseCreatorUnpackedMgr.h
//
// Provides the definition for the IDatabaseCreatorUnpackedMgr interface. This interface is used to
// create unpacked (text) databases.
//
//----------------------------------------------------------------------------------------------------

#ifndef __IDATABASECREATORUNPACKEDMGR_H__
#define __IDATABASECREATORUNPACKEDMGR_H__

//we rely upon the attribute type enumeration and the handle definitions from DatabaseMgr.h
#ifndef __IDATABASEMGR_H__
#	include "idatabasemgr.h"
#endif

// forward declarations
class CDatabaseCreatorUnpacked;
class CAttributeCreatorUnpacked;

// typedefs
typedef CDatabaseCreatorUnpacked*  HDATABASECREATORUNPACKED;
typedef CAttributeCreatorUnpacked* HATTRIBUTECREATORUNPACKED;

// IDatabaseUnpackedMgr interface
class IDatabaseCreatorUnpackedMgr
{
public:

	virtual ~IDatabaseCreatorUnpackedMgr()  {}

	//This will create a new database for construction. This returned handle must be freed with
	//ReleaseDatabase otherwise memory will be leaked.
	virtual HDATABASECREATORUNPACKED CreateNewDatabase() = 0;

	//called to release a reference to the database. Once this call is made, none of the category,
	//record, or attribute handles that were obtained from this database are valid and must not be used.
	virtual void				     ReleaseDatabase(HDATABASECREATORUNPACKED hDatabase) = 0;

	//called to save the specified database to the provided filename. This will return true if
	//successful, and false otherwise
	virtual bool				     SaveDatabase(HDATABASECREATORUNPACKED hDatabase, const char* pszDatabaseFilename, const char* pszConstraintsFilename) = 0;

	//------------------------------------
	// Attribute Management
	//------------------------------------
		
	// create an attribute with the specified properties - this will return NULL if the attribute cannot be created.
	virtual HATTRIBUTECREATORUNPACKED CreateAttribute(HDATABASECREATORUNPACKED hDatabase,
													  const std::string&	   strAttributePath,
													  const EAttributeType	   eType,
													  const std::string&       strUserTypeName,
													  const std::string&	   strUserConstraints,
													  const std::string&	   strDescription, 
													  const bool			   bCommented,
													  const uint32			   nMaxNumberOfValues) = 0;

	// specify constraints for an attribute - these will return false if the attribute type is different
	// from the type specified in the method name.
	virtual bool SetInt32MinMaxValues(HATTRIBUTECREATORUNPACKED hAttribute, int32 nMinValue, int32 nMaxValue) = 0;
	virtual bool SetFloatMinMaxValues(HATTRIBUTECREATORUNPACKED hAttribute, float nMinValue, float nMaxValue) = 0;
	virtual bool SetStringMinMaxLengths(HATTRIBUTECREATORUNPACKED hAttribute, uint32 nMinLength, uint32 nMaxLength) = 0;	

	// specify a value for an attribute - each call will append a value onto the list of values for
	// the attribute.
	virtual bool SetInt32(HATTRIBUTECREATORUNPACKED hAttribute, int32 nValue) = 0;
	virtual bool SetFloat(HATTRIBUTECREATORUNPACKED hAttribute, float fValue) = 0;
	virtual bool SetString(HATTRIBUTECREATORUNPACKED hAttribute, const char* pszValue) = 0;
	virtual bool SetBool(HATTRIBUTECREATORUNPACKED hAttribute, bool bValue) = 0;
	virtual bool SetVector2(HATTRIBUTECREATORUNPACKED hAttribute, LTVector2& vValue) = 0;
	virtual bool SetVector3(HATTRIBUTECREATORUNPACKED hAttribute, LTVector& vValue) = 0;
	virtual bool SetVector4(HATTRIBUTECREATORUNPACKED hAttribute, LTVector4& vValue) = 0;
};


#endif
