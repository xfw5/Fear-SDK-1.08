//----------------------------------------------------------------------------------------------------
// IDatabaseMgr.h
//
// Provides the definition for the IDatabaseMgr interface. This interface is used to load and manage
// game databases. Each database contains a collection of categories. Each category represents a 
// type of entry, and contains a collection of records. Each record is an instance of a category
// and holds a list of attributes. Each attribute is of a certain type and contains an array of values.
// The database manager handles referencing of databases so that if multiple game objects are using
// the same database it will only be loaded and stored once. This interface only supports read access
// to databases. The IDatabaseCreator should be used for creating databases and saving them to disk.
//
//----------------------------------------------------------------------------------------------------

#ifndef __IDATABASEMGR_H__
#define __IDATABASEMGR_H__

//handles for the different database objects
class CDatabase;
class CCategory;
class CRecord;
class CAttribute;

typedef CDatabase*			HDATABASE;
typedef const CCategory*	HCATEGORY;
typedef const CRecord*		HRECORD;
typedef const CAttribute*	HATTRIBUTE;

//handles for the different database creator objects
class CDatabaseCreator;
class CCategoryCreator;
class CRecordCreator;
class CAttributeCreator;

typedef CDatabaseCreator*	HDATABASECREATOR;
typedef CCategoryCreator*	HCATEGORYCREATOR;
typedef CRecordCreator*		HRECORDCREATOR;
typedef CAttributeCreator*	HATTRIBUTECREATOR;

//defnition for an invalid database index
#define INVALID_GAME_DATABASE_INDEX		0xFFFFFFFF

//forward declarations
class ILTInStream;

//An enumeration that specifies the different attribute types that are available
enum EAttributeType
{
	eAttributeType_Invalid,
	eAttributeType_Bool,
	eAttributeType_Float,
	eAttributeType_Int32,
	eAttributeType_String,
	eAttributeType_WString,
	eAttributeType_Vector2,
	eAttributeType_Vector3,
	eAttributeType_Vector4,
	eAttributeType_RecordLink,
	eAttributeType_Struct,
};

//an enumeration that specifies the different usages a property can have. For example, a string
//can be used as just a string or the name of an animation, a filename, or a reference to a ClientFX
//This is only provided for informational purposes and does not effect database access at all
enum EAttributeUsage
{
	//the standard default attribute usage, no special usage
	eAttributeUsage_Default,
	//this attribute is intended to serve as a reference to a file resource
	eAttributeUsage_Filename,
	//this attribute is intended to serve as a reference to a client fx
	eAttributeUsage_ClientFX,
	//this attribute is intended to serve as a reference to a model animation
	eAttributeUsage_Animation,
};

//IDatabaseMgr interface for managing game databases
class IDatabaseMgr
{
public:

	//------------------------------------
	// Database Management
	//------------------------------------

    //Given the name of a database this will determine if the database is loaded, and if it is
	//it will return a new handle to that database. This handle must be released with ReleaseDatabase
	//otherwise it will be leaked. This will return NULL if the database has not already been loaded.
	virtual HDATABASE		OpenExistingDatabase(const char* pszFileName) = 0;

	//given the name of a database that this will be referred to as and an ILTInStream that is opened
	//for read access to the database to be read, this will load the database from the file and
	//return a handle to the new database. If the database already exists, it will not be read
	//again and instead a reference to the existing database will be returned. This returned handle
	//must be released with ReleaseDatabase otherwise it will result in a leak. The caller is responsible
	//for closing the passed in file stream.
	virtual HDATABASE		OpenNewDatabase(const char* pszFileName, ILTInStream& InFile) = 0;

	//given an ILTInStream to an unpacked file, this will load the database from the stream and
	//return a handle.  This method does not perform any reference counting on databases, so calling
	//twice with the same stream will result in two separate instances. The returned handle must be
	//released with ReleaseDatabase otherwise it will result in a leak.  The caller is responsible for
	//closing the passed in stream.
	virtual HDATABASE		OpenUnpackedDatabase(ILTInStream& UnpackedFile, ILTInStream& ConstraintFile) = 0;

	//this will convert a database that was created for writing over to a database that can be used
	//for reading. This is a costly operation and therefore should not be done at runtime. This will
	//return NULL if the conversion operation fails. It also takes a name that this database can be
	//identified by when using OpenExistingDatabase. If a database with that name is already opened,
	//it will use that database in place of converting. The returned handle must be freed with
	//ReleaseDatabase.
	virtual HDATABASE		ConvertDatabase(HDATABASECREATOR hCreator, const char* pszDatabaseName) = 0;

	//this will swap the values of two databases.  Note that the values in the first database (hDatabase1)
	//must all be present in the second database (hDatabase2), meaning that the first is a subset of the
	//second.  If this is not the case, this will return false.  To restore the databases to their original
	//states, simply call this a second time.
	virtual bool            SwapDatabaseValues(HDATABASE hDatabase1, HDATABASE hDatabase2) = 0;

	//this will return the name of the database, or NULL if the provided database
	//is invalid. Note that this returned string must not be referenced after the database has been
	//released.
	virtual const char*		GetDatabaseName(HDATABASE hDatabase) = 0;

	//called to release a reference to the database. Once this call is made, none of the category,
	//record, or attribute handles that were obtained from this database are valid and must not be used.
	virtual void			ReleaseDatabase(HDATABASE hDatabase) = 0;

	//------------------------------------
	// Category Management
	//------------------------------------

	//given a database and the name of a category, this will return a handle to the category within
	//the database or NULL if it cannot be found. 
	virtual HCATEGORY		GetCategory(HDATABASE hDatabase, const char* pszCategoryName) = 0;

	//given a database, this will return the number of categories that exist within the database,
	//or 0 if the database is invalid
	virtual uint32			GetNumCategories(HDATABASE hDatabase) = 0;

	//given a database and an index, this will return a handle to the category within
	//the database or NULL if the index is out of range or the database invalid. 
	virtual HCATEGORY		GetCategoryByIndex(HDATABASE hDatabase, uint32 nIndex) = 0;

	//given a category, this will return the parent database of the category. This is guaranteed to
	//be valid as long as the category handle is valid.
	virtual HDATABASE		GetCategoryParent(HCATEGORY hCategory) = 0;

	//given a category, this will return the index of the category in the parent database. This
	//will return INVALID_GAME_DATABASE_INDEX if the category is invalid.
	virtual uint32			GetCategoryIndex(HCATEGORY hCategory) = 0;

	//given a category this will return the name of the category, or NULL if the provided category
	//is invalid. Note that this returned string must not be referenced after the database has been
	//released.
	virtual const char*		GetCategoryName(HCATEGORY hCategory) = 0;

	//------------------------------------
	// Record Management
	//------------------------------------

	//given a category, this allows for obtaining a record from the category by a string name. This
	//will return NULL if the category is invalid or the record cannot be found.
	virtual HRECORD			GetRecord(HCATEGORY hCategory, const char* pszRecordName) = 0;

	//allows for a more condensed form of access to an individual record given only a database. This
	//will return NULL if it cannot match any of the parameters in the hierarchy to get to the record.
	virtual HRECORD			GetRecord(	HDATABASE hDatabase, const char* pszCategory, 
										const char* pszRecord) = 0;

	//given a category, this returns how many records it contains, or 0 if the category
	//is invalid.
	virtual uint32			GetNumRecords(HCATEGORY hCategory) = 0;

	//given a category, this allows for obtaining a record from the category by an index. This will return
	//NULL if the category is invalid or if the index is out of range
	virtual HRECORD			GetRecordByIndex(HCATEGORY hCategory, uint32 nIndex) = 0;

	//given a record, this will return the parent category of the record. This is guaranteed to
	//be valid as long as the record handle is valid.
	virtual HCATEGORY		GetRecordParent(HRECORD hRecord) = 0;

	//given a record, this will return the index of the record in the parent category. This
	//will return INVALID_GAME_DATABASE_INDEX if the record is invalid.
	virtual uint32			GetRecordIndex(HRECORD hRecord) = 0;

	//given a record, this will return the record name, or NULL if the provided record
	//is invalid. Note that this returned string must not be referenced after the database has been
	//released.
	virtual const char*		GetRecordName(HRECORD hRecord) = 0;

	//------------------------------------
	// Attribute Management
	//------------------------------------

	//given a record this will get an attribute by name. This will return NULL if the record is invalid
	//or if the attribute cannot be found.
	virtual HATTRIBUTE		GetAttribute(HRECORD hRecord, const char* pszAttributeName) = 0;

	//allows for a more condensed form of access to an individual attribute given only a database. This
	//will return NULL if it cannot match any of the parameters in the hierarchy to get to the attribute.
	virtual HATTRIBUTE		GetAttribute(	HDATABASE hDatabase, const char* pszCategory, 
											const char* pszRecord, const char* pszAttribute) = 0;

	//allows for a more condensed form of access to an individual attribute given only a database. This
	//will return NULL if it cannot match any of the parameters in the hierarchy to get to the attribute.
	virtual HATTRIBUTE		GetAttribute(	HCATEGORY hCategory, const char* pszRecord, 
											const char* pszAttribute) = 0;

	//given a record this will return how many attributes it contains
	virtual uint32			GetNumAttributes(HRECORD hRecord) = 0;

	//given a record and an index, this will return the appropriate attribute, or NULL if the record
	//is invalid or the index out of range
	virtual HATTRIBUTE		GetAttributeByIndex(HRECORD hRecord, uint32 nIndex) = 0;

	//given an attribute, this will return the parent record of the attribute. This is guaranteed to
	//be valid as long as the attribute handle is valid.
	virtual HRECORD			GetAttributeParent(HATTRIBUTE hAttribute) = 0;

	//given an attribute, this will return the index of the attribute in the parent record. This
	//will return INVALID_GAME_DATABASE_INDEX if the attribute is invalid.
	virtual uint32			GetAttributeIndex(HATTRIBUTE hAttribute) = 0;

	//given an attribute, this will return the attribute name, or NULL if the provided attribute
	//is invalid. Note that this returned string must not be referenced after the database has been
	//released.
	virtual const char*		GetAttributeName(HATTRIBUTE hAttribute) = 0;

	//------------------------------------
	// Value Management
	//------------------------------------

	//given an attribute this will determine how many values it contains. All valid attributes are
	//guaranteed to contain at least one value. An invalid attribute handle will return 0
	virtual uint32			GetNumValues(HATTRIBUTE hAttribute) = 0;

	//provides access to the type of the attribute. This will return eAttributeType_Invalid if the
	//attribute handle is invalid.
	virtual EAttributeType	GetAttributeType(HATTRIBUTE hAttribute) = 0;

	//provides access to the usage of this attribute. This can be used to get more information about
	//how the field is supposed to be used and is provided for informational purposes only. This will return
	//the default usage if the provided attribute is invalid
	virtual EAttributeUsage	GetAttributeUsage(HATTRIBUTE hAttribute) = 0;

	//provides access to an integer value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual int32			GetInt32(HATTRIBUTE hAttribute, uint32 nValueIndex, int32 nDefault) = 0;

	//provides access to a floating point value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual float			GetFloat(HATTRIBUTE hAttribute, uint32 nValueIndex, float fDefault) = 0;

	//provides access to a string value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual const char*		GetString(HATTRIBUTE hAttribute, uint32 nValueIndex, const char* pszDefault) = 0;

	//provides access to a unicode string value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual const wchar_t*	GetWString(HATTRIBUTE hAttribute, uint32 nValueIndex, const wchar_t* pszDefault) = 0;

	//provides access to a vector 3 value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual LTVector2		GetVector2(HATTRIBUTE hAttribute, uint32 nValueIndex, const LTVector2& vDefault) = 0;

	//provides access to a vector 3 value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual LTVector		GetVector3(HATTRIBUTE hAttribute, uint32 nValueIndex, const LTVector& vDefault) = 0;

	//provides access to a vector 4 value. This will return the default should the attribute
	//be invalid, the value index out of range, or the attribute type is different than expected.
	virtual LTVector4		GetVector4(HATTRIBUTE hAttribute, uint32 nValueIndex, const LTVector4& vDefault) = 0;

	//provides access to a boolean value. This will return the default should the attribute
	//be invalid, or the value index out of range.
	virtual bool			GetBool(HATTRIBUTE hAttribute, uint32 nValueIndex, bool bDefault) = 0;

	//provides access to a record link value. This will return the default should the attribute
	//be invalid, or the value index out of range.
	virtual HRECORD			GetRecordLink(HATTRIBUTE hAttribute, uint32 nValueIndex, HRECORD hDefault) = 0;
};


#endif
