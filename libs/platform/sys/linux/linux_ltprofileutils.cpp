// *********************************************************************** //
//
// MODULE  : linux_ltprofileutils.cpp
//
// PURPOSE : Linux implementation of LTProfileUtils.
//
// CREATED : 06/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <map>
#include <string>
#include "ltprofileutils.h"
#include "sys/win/mpstrconv.h"
#include "ltfileread.h"

// container for complete profile section
typedef std::map<std::string, std::string> KeyValueMap;
struct ProfileSectionEntries
{
	KeyValueMap m_cKeyValueMap;
};

// container of sections to KeyValueMaps
typedef std::map<std::string, ProfileSectionEntries> SectionMap;
struct ProfileSections
{
	SectionMap m_cSectionMap;
};

// map of files to sections
typedef std::map<std::string, ProfileSections> ProfileCache; 

// declare profile cache
static ProfileCache g_cProfileCache;

// helper for processing the file
static ProfileCache::iterator ProcessProfileFile(const char* pszFileName);

void LTProfileUtils::ReadString(const char*  pszSectionName, 
								const char*  pszKeyName,
								const char*  pszDefaultValue,
								char* 	   pszValueString,
								uint32	   nValueStringBufferSize,
								const char*  pszFileName)
{
	// resolve file to absolute path
	char pszAbsoluteFileName[MAX_PATH];
	::realpath(pszFileName, pszAbsoluteFileName);
	
	// check profile cache for this file
	ProfileCache::iterator itCachedProfile = g_cProfileCache.find(pszAbsoluteFileName);
	if (itCachedProfile == g_cProfileCache.end())
	{
		// it's not in the cache, process the file
		itCachedProfile = ProcessProfileFile(pszAbsoluteFileName);
		
		if (itCachedProfile == g_cProfileCache.end())
		{
			// unable to open or process the file - return the default value if specified
			if (pszDefaultValue)
			{
				LTStrCpy(pszValueString, pszDefaultValue, nValueStringBufferSize);
			}
			else
			{
				pszValueString = NULL;
			}
			return;
		}
	}
	
	// look for the section
	ProfileSections cProfile = (*itCachedProfile).second;
	SectionMap::iterator itSection = cProfile.m_cSectionMap.find(pszSectionName);
	
	// if it's not there, return default value
	if (itSection == cProfile.m_cSectionMap.end())
	{
		LTStrCpy(pszValueString, pszDefaultValue, nValueStringBufferSize);
		return;
	}
	
	// look for the key
	KeyValueMap::iterator itKeyValue = (*itSection).second.m_cKeyValueMap.find(pszKeyName);
	
	// if it's not there, return default value
	if (itKeyValue == (*itSection).second.m_cKeyValueMap.end())
	{		
		LTStrCpy(pszValueString, pszDefaultValue, nValueStringBufferSize);
		return;
	}		
	
	// return the value
	LTStrCpy(pszValueString, (*itKeyValue).second.c_str(), nValueStringBufferSize);
}

void LTProfileUtils::ReadString(const wchar_t* pszSectionName, 
								const wchar_t* pszKeyName,
								const wchar_t* pszDefaultValue,
								wchar_t*	     pszValueString,
								uint32	     nValueStringBufferSize,
								const wchar_t* pszFileName)
{
	// get ANSI versions of strings
	char pszSectionNameANSI[MAX_PATH];
	LTStrCpy(pszSectionNameANSI, MPW2A(pszSectionName).c_str(), MAX_PATH);
	
	char pszKeyNameANSI[MAX_PATH];
	LTStrCpy(pszKeyNameANSI, MPW2A(pszKeyName).c_str(), MAX_PATH);
	
	char pszDefaultValueANSI[MAX_PATH];
	LTStrCpy(pszDefaultValueANSI, MPW2A(pszDefaultValue).c_str(), MAX_PATH);
	
	char pszFileNameANSI[MAX_PATH];
	LTStrCpy(pszFileNameANSI, MPW2A(pszFileName).c_str(), MAX_PATH);
	
	char* pszValueStringANSI = NULL;
	LT_MEM_TRACK_ALLOC(pszValueStringANSI = new char[nValueStringBufferSize], LT_MEM_TYPE_MISC);
	
	// call ANSI version
	ReadString(pszSectionNameANSI, pszKeyNameANSI, pszDefaultValueANSI, pszValueStringANSI, nValueStringBufferSize, pszFileNameANSI);
	
	// convert output value
	LTStrCpy(pszValueString, MPA2W(pszValueStringANSI), nValueStringBufferSize);
	
	delete [] pszValueStringANSI;
}

uint32 LTProfileUtils::ReadUint32(const char* pszSectionName, 
								const char* pszKeyName,
								uint32 nDefault,
								const char* pszFileName)
{
	char szValue[64];
	LTSNPrintF( szValue, LTARRAYSIZE( szValue ), "%d", nDefault );
	ReadString( pszSectionName, pszKeyName, szValue, szValue, LTARRAYSIZE( szValue ), pszFileName);
	
	return ( uint32 )atoi( szValue );
}


bool LTProfileUtils::WriteString(const char* pszSectionName, 
								const char* pszKeyName,
								const char* pszValueString,
								const char* pszFileName)
{
	// temporary stub
	return false;
}

bool LTProfileUtils::WriteString(const wchar_t* pszSectionName, 
								const wchar_t* pszKeyName,
								const wchar_t* pszValueString,
								const wchar_t* pszFileName)
{
	// temporary stub
	return false;
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

static bool IsEOL(const char* pData, uint32 nOffset, uint32& nEOLLength)
{
	// check for end of buffer
	if (pData[nOffset] == 0) 
	{
		return true;
	}
	// check for CR character
	else if (pData[nOffset] == '\r') 
	{
		// check for the following LF
		if (pData[nOffset + 1] != 0)
		{
			if (pData[nOffset + 1] == '\n')
			{
				// end of line
				nEOLLength = 2;

				return true;
			}
		}
		else
		{
			// found CR + end of buffer
			nEOLLength = 1;

			return true;
		}
	}
	// check for LF character
	else if (pData[nOffset] == '\n')
	{
		// end of line
		nEOLLength = 1;

		return true;
	}

	// not end of line
	return false;
}

static bool ReadLineFromProfileFile(CLTFileRead& cFileRead, char* pszLineBuffer, uint32 nLineBufferLength)
{
	::memset(pszLineBuffer, 0, nLineBufferLength);
	
	uint64 nPos = 0;
	if (!cFileRead.GetPos(nPos))
	{
		return false;
	}
	
	uint64 nSize = 0;
	if (!cFileRead.GetFileSize(nSize))
	{
		return false;
	}
		
	uint64 nAmountRemaining = nSize - nPos;
	
	if (nAmountRemaining == 0)
	{
		return false;
	}
	
	// read from the file until we either run out of buffer space or file data
	for (uint32 nBufferIndex = 0; nBufferIndex < nLineBufferLength && nAmountRemaining > 0; ++nBufferIndex, --nAmountRemaining)
	{
		char cChar;
		

		if (!cFileRead.Read(&cChar, 1))
		{
			// read failure
			return false;
		}
		
		*(pszLineBuffer + nBufferIndex) = cChar;
		
		if (*(pszLineBuffer + nBufferIndex) == '\n')
		{
			// end of line
			return true;
		}
	}
	
	return true;
}	
		

ProfileCache::iterator ProcessProfileFile(const char* pszFileName)
{
	ProfileCache::iterator itCachedProfile = g_cProfileCache.end();
	
	// open the file	
	CLTFileRead cFileRead;
	if (!cFileRead.Open(pszFileName))
	{
		// can't open it
		return itCachedProfile;
	}
	
	ProfileSections cProfileSections;

	// process the file line-by-line
	char pszLine[MAX_PATH];
	char pszCurrentSection[MAX_PATH];
	char pszKey[MAX_PATH];
	char pszValue[MAX_PATH];
	
	bool bIsEOF = false;

	while (ReadLineFromProfileFile(cFileRead, pszLine, MAX_PATH))
	{
		if ((pszLine[0] == '\n') || (pszLine[0] == '\r') || (pszLine[0] == '#'))
		{
			// newline or comment
			continue;
		}
		else if (pszLine[0] == '[')
		{
			// section tag - find ending bracket
			char* pszRightBracket = ::index(pszLine, ']');
			
			LTSubStrCpy(pszCurrentSection, pszLine + 1, MAX_PATH, LTStrLen(pszLine) - LTStrLen(pszRightBracket) - 1);			
		}
		else
		{
			// key and value line - find the equal sign
			char* pszEqual = ::index(pszLine, '=');
			if (!pszEqual)
			{
				// no equal sign
				continue;
			}
			
			// extract the key and value
			LTSubStrCpy(pszKey, pszLine, MAX_PATH, LTStrLen(pszLine) - LTStrLen(pszEqual));
			char* pszEOL = ::strpbrk(pszLine, "\r\n");
			LTSubStrCpy(pszValue, pszEqual + 1, MAX_PATH, LTStrLen(pszEqual + 1) - LTStrLen(pszEOL));
			
			// add them to the cache			
			SectionMap::iterator itSection = cProfileSections.m_cSectionMap.find(pszCurrentSection);
			if (itSection == cProfileSections.m_cSectionMap.end())
			{
				// add new section
				ProfileSectionEntries cNewProfileSectionEntries;
				cNewProfileSectionEntries.m_cKeyValueMap.insert(std::pair<std::string, std::string>(pszKey, pszValue));
				cProfileSections.m_cSectionMap.insert(std::pair<std::string, ProfileSectionEntries>(pszCurrentSection, cNewProfileSectionEntries));
			}
			else
			{
				// add to current section
				(*itSection).second.m_cKeyValueMap.insert(std::pair<std::string, std::string>(pszKey, pszValue));
			}						
		}
	}
	
	// add to profile cache
	return (g_cProfileCache.insert(std::pair<std::string, ProfileSections>(pszFileName, cProfileSections))).first;	
}
	
	
	
