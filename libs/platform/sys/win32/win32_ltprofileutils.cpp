// *********************************************************************** //
//
// MODULE  : win32_ltprofileutils.cpp
//
// PURPOSE : Win32 implementation of LTProfileUtils.
//
// CREATED : 06/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltprofileutils.h"

void LTProfileUtils::ReadString(const char*  pszSectionName, 
								const char*  pszKeyName,
								const char*  pszDefaultValue,
								char* 	      pszValueString,
								uint32		  nValueStringBufferSize,
								const char*  pszFileName)
{
	::GetPrivateProfileString(pszSectionName, pszKeyName, pszDefaultValue, pszValueString, nValueStringBufferSize, pszFileName);
}

void LTProfileUtils::ReadString(const wchar_t* pszSectionName, 
						 		const wchar_t* pszKeyName,
						  		const wchar_t* pszDefaultValue,
								wchar_t*	    pszValueString,
								uint32		    nValueStringBufferSize,
								const wchar_t* pszFileName)
{
	::GetPrivateProfileStringW(pszSectionName, pszKeyName, pszDefaultValue, pszValueString, nValueStringBufferSize, pszFileName);
}

uint32 LTProfileUtils::ReadUint32(const char* pszSectionName, 
								const char* pszKeyName,
								uint32 nDefault,
								const char* pszFileName)
{
	return ::GetPrivateProfileInt( pszSectionName, pszKeyName, ( int32 )nDefault, pszFileName );
}

bool LTProfileUtils::WriteString(const char* pszSectionName, 
								const char* pszKeyName,
								const char* pszValueString,
								const char* pszFileName)
{
	return (::WritePrivateProfileStringA(pszSectionName, pszKeyName, pszValueString, pszFileName) != 0);
}

bool LTProfileUtils::WriteString(const wchar_t* pszSectionName, 
							 	const wchar_t* pszKeyName,
								const wchar_t* pszValueString,
								const wchar_t* pszFileName)
{
	return (::WritePrivateProfileStringW(pszSectionName, pszKeyName, pszValueString, pszFileName) != 0);
}


bool LTProfileUtils::WriteUint32(const char* pszSectionName, 
								const char* pszKeyName,
								uint32 nValue,
								const char* pszFileName)
{
	char szValue[64];
	LTSNPrintF( szValue, LTARRAYSIZE( szValue ), "%d", nValue );
	return (WriteString(pszSectionName, pszKeyName, szValue, pszFileName) != 0);
}
