// ----------------------------------------------------------------------- //
//
// MODULE  : istringeditmgr.h
//
// PURPOSE : Declares the IStringEditMgr interface.  This interface is
//           used to read from string keeper packed files.  Given an
//           opened string keeper database a string value can be
//           retrieved.  The string values returned are constant
//           strings in memory.  To create new string keeper packed
//           files use the IStringEditCreatorMgr interface.
//
// CREATED : 08/06/04
//
// (c) 1999-2004 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __ISTRINGEDITMGR_H__
#define __ISTRINGEDITMGR_H__

class CStringEdit;
typedef CStringEdit* HSTRINGEDIT;

#define MAX_STRINGID_LENGTH				64
#define INVALID_STRINGEDIT_INDEX		((uint32)(-1))

// hard-coded none value
#define STRINGID_NONE					"<none>"
#define STRINGID_NONE_INDEX				((uint32)(-2))

typedef void (*PFNSTRINGEDITERRORCALLBACK)( HSTRINGEDIT hStringEdit, const char* szStringID, const char* szError, void* pUserData );
typedef void (*PFNSTRINGEDITSTRINGIDCALLBACK)( const char* szStringID, void* pUserData );

class IStringEditMgr
{
public:
	// creates a string keeper handle from an already opened file stream
	virtual HSTRINGEDIT		CreateFromFile( ILTInStream* pFileStream ) = 0;

	// releases the string keeper database handle
	virtual void			ReleaseStringEdit( HSTRINGEDIT hStringEdit ) = 0;

	// registers a callback that gets called with error messages
	//
	// the strings that are passed into this call are temporary 
	// and their pointers should not be saved. if you plan to save 
	// the error string copy them over to your own string buffers
	//
	// only one callback can be registered at a time
	virtual bool			RegisterErrorCallback( PFNSTRINGEDITERRORCALLBACK pfnCallback, void* pUserData ) = 0;

	// unregisters the callback.
	// once a callback is unregistered a new callback can be registered
	virtual bool			UnregisterErrorCallback( PFNSTRINGEDITERRORCALLBACK pfnCallback ) = 0;

	// returns the index of a string in the table
	// this value can be used to access the string table much quicker 
	// when retrieving the string multiple times using the string value.
	// never save this index between sessions of the game
	// returns INVALID_STRINGEDIT_INDEX if not found
	virtual uint32			GetIndex( HSTRINGEDIT hStringEdit, const char* szStringID ) = 0;

	// returns the String ID at the specified index.  The index must have 
	// been previously retrieved by calling GetIndex
	virtual const char*		GetStringID( HSTRINGEDIT hStringEdit, uint32 nIndex ) = 0;

	// returns the value of a string given a string keeper handle and a string ID
	// if the string is not found a default error string is returned instead
	// to check if the string was found call DoesStringExists
	virtual const wchar_t*	GetString( HSTRINGEDIT hStringEdit, const char* szStringID ) = 0;
	// returns the value using an index returned from GetIndex
	virtual const wchar_t*	GetString( HSTRINGEDIT hStringEdit, uint32 nIndex ) = 0;

	// uses the string stored in the database as a printf format string
	// a user allocated buffer must be passed in to store the result
	// the return value indicates the number of characters written or -1 if an error occurs
	virtual int32			FormatString( HSTRINGEDIT hStringEdit, const char* szStringID, wchar_t* wszBuffer, uint32 nBufferSize, ... ) = 0;
	virtual int32			FormatString( HSTRINGEDIT hStringEdit, const char* szStringID, wchar_t* wszBuffer, uint32 nBufferSize, va_list argptr ) = 0;
	// uses an index returned from GetIndex
	virtual int32			FormatString( HSTRINGEDIT hStringEdit, uint32 nIndex, wchar_t* wszBuffer, uint32 nBufferSize, ... ) = 0;
	virtual int32			FormatString( HSTRINGEDIT hStringEdit, uint32 nIndex, wchar_t* wszBuffer, uint32 nBufferSize, va_list argptr ) = 0;

	// uses the string stored in the database as a FormatMessage format string
	// these methods mimic the Win32 FormatMessage function call
	// a user allocated buffer must be passed in to store the result
	// the return value indicates an error
	virtual bool			FormatMessage( HSTRINGEDIT hStringEdit, const char* szStringID, wchar_t* wszBuffer, uint32 nBufferSize, ... ) = 0;
	virtual bool			FormatMessage( HSTRINGEDIT hStringEdit, const char* szStringID, wchar_t* wszBuffer, uint32 nBufferSize, va_list argptr ) = 0;
	// uses an index returned from GetIndex
	virtual bool			FormatMessage( HSTRINGEDIT hStringEdit, uint32 nIndex, wchar_t* wszBuffer, uint32 nBufferSize, ... ) = 0;
	virtual bool			FormatMessage( HSTRINGEDIT hStringEdit, uint32 nIndex, wchar_t* wszBuffer, uint32 nBufferSize, va_list argptr ) = 0;

	// returns the value of a string given a string keeper handle and a string ID
	// if the string is not found a default error string is returned instead
	// to check if the voice path was found call DoesVoicePathExists
	virtual const char*		GetVoicePath( HSTRINGEDIT hStringEdit, const char* szStringID ) = 0;
	// returns the voice path using an index returned from GetIndex
	virtual const char*		GetVoicePath( HSTRINGEDIT hStringEdit, uint32 nIndex ) = 0;

	// determines if the string id exists
	virtual bool			DoesIDExist( HSTRINGEDIT hStringEdit, const char* szStringID ) = 0;
	// returns true if the string value exists in the string table
	virtual bool			DoesValueExist( HSTRINGEDIT hStringEdit, const char* szStringID ) = 0;
	// determines if id is valid and whether it has a value
	virtual bool			DoesValueExist( HSTRINGEDIT hStringEdit, uint32 nIndex ) = 0;
	// returns true if the voice path exists in the string table
	virtual bool			DoesVoicePathExist( HSTRINGEDIT hStringEdit, const char* szStringID ) = 0;
	// determines if id is valid and whether it has a voice path
	virtual bool			DoesVoicePathExist( HSTRINGEDIT hStringEdit, uint32 nIndex ) = 0;

	// enumerates string IDs that start with the specified string
	virtual bool			EnumerateStringIDBeginWith( HSTRINGEDIT hStringEdit, const char* szStartWith, PFNSTRINGEDITSTRINGIDCALLBACK pfnCallback, void* pUserData ) = 0;
};

#endif  // __ISTRINGEDITMGR_H__


