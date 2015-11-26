// *********************************************************************** //
//
// MODULE  : ltprofileutils.h
//
// PURPOSE : Helpers for profile operations.
//
// CREATED : 06/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTPROFILEUTILS_H__
#define __LTPROFILEUTILS_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

class LTProfileUtils
{
public:

	// Returns the value of the key in the specified section of the file as a
	// string.  If the value is not present, the supplied default value is returned.
	// ANSI and wide-string versions are provided.
	static void ReadString(const char*  pszSectionName, 
							const char*  pszKeyName,
							const char*  pszDefaultValue,
							char* 	     pszValueString,
							uint32		 nValueStringBufferSize,
							const char*  pszFileName);

	static void ReadString(const wchar_t* pszSectionName, 
							const wchar_t* pszKeyName,
							const wchar_t* pszDefaultValue,
							wchar_t*	   pszValueString,
							uint32		   nValueStringBufferSize,
							const wchar_t* pszFileName);

	static uint32 ReadUint32(const char* pszSectionName, 
							const char* pszKeyName,
							uint32 nDefault,
							const char* pszFileName);


	// Writes the value of the key to the specified file and section.
	// ANSI and wide-string versions are provided.
	static bool WriteString(const char* pszSectionName, 
							const char* pszKeyName,
							const char* pszValueString,
							const char* pszFileName);

	static bool WriteString(const wchar_t* pszSectionName, 
							const wchar_t* pszKeyName,
							const wchar_t* pszValueString,
							const wchar_t* pszFileName);

	static bool WriteUint32(const char* pszSectionName, 
							const char* pszKeyName,
							uint32 nValue,
							const char* pszFileName);

};

#endif // __LTPROFILEUTILS_H__
