// *********************************************************************** //
//
// MODULE  : ltfileoperations.cpp
//
// PURPOSE : Implementation file for file operations.  The implementation of 
//			 these operations are mostly platform independent, and can be
//           shared across all platforms.  Platform specific functionality
//           is handled by a call to a "PlatformHook" function, or if this is
//           not possible, by conditional compilation.
//
// CREATED : 08/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //
	
#include <sys/stat.h>

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTSTRUTILS_H__
#include "ltstrutils.h"
#endif

#if defined(PLATFORM_WIN32)
#include "windows.h"
#endif

#include "ltfileoperations.h"
#include "ltfileread.h"

const char   CR_CHARACTER_CODE		  = 13;
const char   LF_CHARACTER_CODE		  = 10;
const char   SPACE_CHARACTER_CODE	  = 32;
const char   TAB_CHARACTER_CODE		  = 20;
const char   COMMENT_CHARACTER_CODE	  = 59; // semicolon character
const uint32 MAX_LINE_LENGTH		  = 1024;

char g_szUserDirectory[MAX_PATH] = { 0 };

bool LTFileOperations::DirectoryExists(const char* pDirectoryName)
{
	if (!pDirectoryName || !*pDirectoryName) 
	{
		return false;
	}

	bool bDirectoryExists = false;

	// remove trailing separator if present
	char szPath[_MAX_PATH + 1];
	LTStrCpy(szPath, pDirectoryName, LTARRAYSIZE(szPath));

	if (LTStrCmp(&szPath[LTStrLen(szPath) - 1], FILE_PATH_SEPARATOR) == 0)
	{
		szPath[LTStrLen(szPath) - 1] = '\0';
	}

#if defined(PLATFORM_WIN32)
	// save the current error mode
	UINT nPreviousErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
#endif 

	// test for existence 
	struct stat statInfo;
	int nError = ::stat(szPath, &statInfo);

#if defined(PLATFORM_WIN32)
	// restore the error mode
	SetErrorMode(nPreviousErrorMode);
#endif

	if (nError != -1) 
	{
		bDirectoryExists = true;
	}

	return bDirectoryExists;
}

bool LTFileOperations::CreateNewDirectory(const char* pDirectoryName)
{
	// test to see if it already exists
	if (DirectoryExists(pDirectoryName)) 
	{
		return true;
	}

	// make sure there's more than just a drive specified
	if (pDirectoryName[LTStrLen(pDirectoryName) - 1] == ':') 
	{
		return false;
	}

	char szPartialPath[_MAX_PATH + 1];
	szPartialPath[0] = '\0';
	
	// preserve the first character if it's a path separator
	if (LTSubStrEquals(pDirectoryName, FILE_PATH_SEPARATOR, LTStrLen(FILE_PATH_SEPARATOR)))
	{
		LTStrCat(szPartialPath, FILE_PATH_SEPARATOR, LTARRAYSIZE(szPartialPath));
	}

	// copy input path
	char szPathCopy[_MAX_PATH + 1];
	LTStrCpy(szPathCopy, pDirectoryName, LTARRAYSIZE(szPathCopy));

	// process each part of the path
	char* token = ::strtok(szPathCopy, FILE_PATH_SEPARATOR);
	while (token)
	{
		LTStrCat(szPartialPath, token, LTARRAYSIZE(szPartialPath));
		if (!DirectoryExists(szPartialPath) && szPartialPath[LTStrLen(szPartialPath) - 1] != ':')
		{
			// call platform-specific hook function to create the directory
			if (!CreateNewDirectoryPlatformHook(szPartialPath))
			{
				return false;
			}
		}

		LTStrCat(szPartialPath, FILE_PATH_SEPARATOR, LTARRAYSIZE(szPartialPath));

		// move to next subdirectory
		token = ::strtok(NULL, FILE_PATH_SEPARATOR);
	}

	return true;
}

// test if the buffer is at EOL at the specified offset
static bool IsEOL(const char* pData, const uint32 nOffset, const uint32 nDataLength, uint32& nEOLLength)
{
	// check for end of buffer
	if (pData[nOffset] == 0) 
	{
		return true;
	}
	// check for CR character
	else if (pData[nOffset] == CR_CHARACTER_CODE) 
	{
		// check for the following LF
		if (nOffset + 1 != nDataLength)
		{
			if (pData[nOffset + 1] == LF_CHARACTER_CODE)
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
	else if (pData[nOffset] == LF_CHARACTER_CODE)
	{
		// end of line
		nEOLLength = 1;

		return true;
	}

	// not end of line
	return false;
}

//Given a filename, this will run through and remove any cases of .\ and ..\ that it finds. Note
//that it doesn't need the string length, since it can only reduce the string size, not expand it
void LTFileOperations::RemoveRelativePaths(char* pPath)
{
	//first off run through looking for .\. If we find one, remove it
	for(uint32 nCurrChar = 0; pPath[nCurrChar] != '\0'; )
	{
		//check for the .\ directive, which should be preceded by either the string
		//beginning, or a slash
		if( (strncmp(&pPath[nCurrChar], ".\\", 2) == 0) && 
			((nCurrChar == 0) || (pPath[nCurrChar - 1] == '\\')))
		{
			strcpy(pPath + nCurrChar, pPath + nCurrChar + 2);
		}

		//check for a ..\ directive
		else if((strncmp(&pPath[nCurrChar], "..\\", 3) == 0) &&
			((nCurrChar == 0) || (pPath[nCurrChar - 1] == '\\')))
		{
			//if this is at the root, just skip it
			if(nCurrChar < 2)
			{
				//the checking for 0 or 1 is to facilitate the cases of "\..\" or "..\"
				strcpy(pPath, &pPath[nCurrChar + 3]);
				nCurrChar = 0;
			}
			else
			{
				//alright, we need to search backwards to the preceding slash and remove that portion of
				//the string
				uint32 nEndRemovalPos = nCurrChar + 2;

				//skip over the slash right before us
				nCurrChar -= 2;
				while(1)
				{
					if((pPath[nCurrChar] == '\\') || (nCurrChar == 0))
					{
						//we hit a stopping point
						break;
					}
					nCurrChar--;
				}

				//now remove that portion
				strcpy(pPath + nCurrChar, pPath + nEndRemovalPos);
			}
		}
		else
		{
			//nothing here, move to the next character
			nCurrChar++;
		}
	}
}

bool LTFileOperations::ParseTextBuffer(const char* pData, uint32 nDataLength, TProcessLineFn pfnProcessLine, void* pProcessLineUserData)
{
	// process the buffer, parsing line-by-line.  Note that the line break character can
	// be either CRLF (Windows) or LF (Linux), depending on the where the file was created
	char   LineBuffer[MAX_LINE_LENGTH] = { 0 };
	uint32 nLineIndex = 0;
	bool   bIsCommentLine = false;

	for (uint32 nIndex = 0; nIndex <= nDataLength; ++nIndex)
	{
		uint32 nEOLLength = 0;
		if ((nIndex == nDataLength) || (IsEOL(pData, nIndex, nDataLength, nEOLLength)))
		{
			if (bIsCommentLine)
			{
				// don't process if it's a comment line
				bIsCommentLine = false;
			}
			else if (nLineIndex == 0)
			{
				// don't process if it's a newline only
			}
			else
			{
				// null terminate the line
				LineBuffer[nLineIndex] = 0;

				// call the process line callback
				pfnProcessLine(LineBuffer, pProcessLineUserData);
			}

			// reset line index
			nLineIndex = 0;

			// adjust the offset if necessary
			if (nEOLLength > 1)
			{
				nIndex += (nEOLLength - 1);
			}

			// move to the next line
			continue;
		}

		// check for comment line
		if ((nLineIndex == 0) && (pData[nIndex] == COMMENT_CHARACTER_CODE))
		{
			bIsCommentLine = true;
		}
		else
		{
			// append the character
			LineBuffer[nLineIndex++] = pData[nIndex];

			// check for maximum line length
			if (nLineIndex == MAX_LINE_LENGTH)
			{
				// exceeded maximum line length
				return false;
			}
		}
	}

	return true;
}

bool LTFileOperations::ParseTextFile(const char* pszFilename, TProcessLineFn pfnProcessLine, void* pProcessLineUserData)
{
	// open the specified file from the specified file name
	CLTFileRead cFileRead;
	if (!cFileRead.Open(pszFilename))
	{
		return false;
	}

	// read the length of the file and allocate a buffer
	uint64 nFileSize = 0;
	if (!cFileRead.GetFileSize(nFileSize))
	{
		return false;
	}

	char* pData = NULL;
	LT_MEM_TRACK_ALLOC(pData = new char[(uint32)nFileSize], LT_MEM_TYPE_MISC);

	// read the data into the buffer
	if (!cFileRead.Read(pData, (uint32)nFileSize))
	{
		return false;
	}

	// call the buffer-based parse method
	bool bResult = LTFileOperations::ParseTextBuffer(pData, (uint32)nFileSize, pfnProcessLine, pProcessLineUserData);

	// free the buffer
	delete [] pData;

	return bResult;
}

void LTFileOperations::SetUserDirectory(const char* pszDirectoryName)
{
	LTStrCpy(g_szUserDirectory, pszDirectoryName, LTARRAYSIZE(g_szUserDirectory));
}

