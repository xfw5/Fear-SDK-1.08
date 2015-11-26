// *********************************************************************** //
//
// MODULE  : linux_ltfileoperations.cpp
//
// PURPOSE : Linux implementation of LTFileOperations.
//
// CREATED : 05/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltfileoperations.h"
#include "ltbasetypes.h"
#include "ltstrutils.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <dirent.h>

const mode_t DEFAULT_PERMISSIONS = 484; // rwxr--r--
extern char g_szUserDirectory[MAX_PATH];

struct Linux_FindFileHandle
{
	DIR* pHandle;
	char pszDirectory[_MAX_DIR + 1];
	char pszFullFileName[_MAX_FNAME + _MAX_EXT + 1];
};

static void BuildFindFileResult(const char* pszDirectory, const char* pszFullFileName, LTFINDFILEINFO* pFindFileInfo);
static void AdjustPathCaseForLinux(char* pszPath);

bool LTFileOperations::DeleteFile(const char* pFileName)
{
	bool returnValue = false;

	if (::remove(pFileName) == 0)
	{
		returnValue = true;
	}

	return returnValue;
}

bool LTFileOperations::GetFileSize(const char* pszFilename, uint64& nFilesize)
{
	struct stat64 statInfo;
	int nResult = ::stat64(pszFilename, &statInfo);
	if (nResult == -1)
	{
		return false;
	}
	
	nFilesize = statInfo.st_size;
	
	return true;
}

void LTFileOperations::GetCurrentWorkingDirectory(char* pDirectoryBuffer, uint32 nDirectoryBufferSize)
{
	::getcwd(pDirectoryBuffer, nDirectoryBufferSize);
}

void LTFileOperations::GetUserDirectory(char* pDirectoryBuffer, uint32 nDirectoryBufferSize)
{
	GetUserDirectory( "", pDirectoryBuffer, nDirectoryBufferSize );
}

void LTFileOperations::GetUserDirectory(char const* pszGameName, char* pDirectoryBuffer, uint32 nDirectoryBufferSize)
{
	if (LTStrEmpty(g_szUserDirectory))
	{
		// set to the current working directory
		GetCurrentWorkingDirectory(pDirectoryBuffer, nDirectoryBufferSize);
	}
	else
	{
		// use the specified user directory
		LTStrCpy(pDirectoryBuffer, g_szUserDirectory, nDirectoryBufferSize);
	}

    // add a trailing slash
    LTStrCat(pDirectoryBuffer, "/", nDirectoryBufferSize);
}

bool LTFileOperations::GetTempFilePath(char* pszTempPathBuffer, uint32 nTempPathBufferSize)
{
	// use current directory
	::getcwd(pszTempPathBuffer, nTempPathBufferSize);
	
	return true;
}
	
bool LTFileOperations::GetTempFileName(const char* pszTempFilePath, const char* pszTempFileNamePrefix, char* pszTempFileNameBuffer, uint32 nTempFileNameBufferSize)
{
	// build the template for mktemp
	char szTemplate[MAX_PATH+1];
	LTSNPrintF(szTemplate, LTARRAYSIZE(szTemplate), "%s%stmp.XXXXXX", pszTempFilePath, FILE_PATH_SEPARATOR, pszTempFileNamePrefix);
	
	// call mkstemp
	int fd = ::mkstemp(szTemplate);
	if (fd == -1)
	{
		return false;
	}
	
	// close the file
	::close(fd);
	
	// copy the result
	LTStrCpy(pszTempFileNameBuffer, szTemplate, nTempFileNameBufferSize);

	return true;
}

bool LTFileOperations::CreateNewDirectoryPlatformHook(const char* pDirectoryName)
{
	return ::mkdir(pDirectoryName, DEFAULT_PERMISSIONS) == -1 ? false : true;
}

bool LTFileOperations::FileExists(const char* pFileName)
{
	struct stat64 statbuf;
	int error = ::stat64(pFileName, &statbuf);
	return (error == 0);
}

bool LTFileOperations::FindFirst(const char* pFindPath, LTFINDFILEHANDLE& Handle, LTFINDFILEINFO* pFindFileInfo)
{
	// break out the find path into its parts
	char pszDirectory[_MAX_DIR + 1];
	char pszFileName[_MAX_FNAME + 1];
	char pszSuffix[_MAX_EXT + 1];
	::memset(pszDirectory, 0, _MAX_DIR + 1);
	::memset(pszFileName, 0, _MAX_FNAME + 1);
	::memset(pszSuffix, 0, _MAX_EXT + 1);
	
	// adjust separators
	char pszFindPathCopy[MAX_PATH];
	::memset(pszFindPathCopy, 0, MAX_PATH);
	LTStrCpy(pszFindPathCopy, pFindPath, MAX_PATH);
	AdjustPathSeparatorsForLinux(pszFindPathCopy);
	LTFileOperations::SplitPath(pszFindPathCopy, pszDirectory, pszFileName, pszSuffix);
	
	// open the directory if supplied, or current directory if not
	if (pszDirectory[0] == (char)NULL)
	{
		LTSNPrintF(pszDirectory, LTARRAYSIZE(pszDirectory), "%s", ".");
	}
	
	DIR* dirHandle = ::opendir(pszDirectory);
	if (dirHandle == NULL)
	{
		// directory does not exist
		return false;
	}
	
	// reassemble the filename
	char pszFullFileName[_MAX_FNAME + _MAX_EXT + 1];
	::memset(pszFullFileName, 0, LTARRAYSIZE(pszFullFileName));
	if ((pszFileName[0] != (char)NULL) && (pszSuffix[0] != (char)NULL))
	{
		LTSNPrintF(pszFullFileName, LTARRAYSIZE(pszFullFileName), "%s%s", pszFileName, pszSuffix);
	}
	else if (pszFileName[0] != (char)NULL)
	{
		LTSNPrintF(pszFullFileName, LTARRAYSIZE(pszFullFileName), "%s", pszFileName);
	}
	else
	{		
		LTSNPrintF(pszFullFileName, LTARRAYSIZE(pszFullFileName), "%s", pszSuffix);
	}
	
	// try to find the first file matching the name
	struct dirent* dirEntry = ::readdir(dirHandle);
	if (dirEntry == NULL)
	{
		::rewinddir(dirHandle);
		dirEntry = ::readdir(dirHandle);
	}
		
	while (dirEntry != NULL)
	{
		if (::fnmatch(pszFullFileName, dirEntry->d_name, FNM_CASEFOLD) == 0)
		{
			// found it
			break;
		}
		
		dirEntry = ::readdir(dirHandle);
	}
	
	if (dirEntry == NULL)
	{
		// no match	
		::closedir(dirHandle);
		return false;
	}
	else
	{
		// build results
		BuildFindFileResult(pszDirectory, dirEntry->d_name, pFindFileInfo);
		
		// return directory handle
		Linux_FindFileHandle* pFindFileHandle = NULL;
		LT_MEM_TRACK_ALLOC(pFindFileHandle = new Linux_FindFileHandle, LT_MEM_TYPE_MISC);
		
		pFindFileHandle->pHandle = dirHandle;
		LTStrCpy(pFindFileHandle->pszDirectory, pszDirectory, LTARRAYSIZE(pFindFileHandle->pszDirectory));		
		LTStrCpy(pFindFileHandle->pszFullFileName, pszFullFileName, LTARRAYSIZE(pFindFileHandle->pszFullFileName));
		
		Handle = (LTFINDFILEHANDLE)pFindFileHandle;
		return true;
	}
}

// finds the next occurrence of a file matching the path specified to FindFirst
bool LTFileOperations::FindNext(LTFINDFILEHANDLE hFindFile, LTFINDFILEINFO* pFindFileInfo)
{
	bool bResult = false;
	
	if ((hFindFile != (LTFINDFILEHANDLE)NULL) && (hFindFile != (LTFINDFILEHANDLE)-1))
	{
		Linux_FindFileHandle* pFindFileHandle = (Linux_FindFileHandle*)hFindFile;

		struct dirent* dirEntry = NULL;
		while ((dirEntry = ::readdir(pFindFileHandle->pHandle)) != NULL)
		{
			if (::fnmatch(pFindFileHandle->pszFullFileName, dirEntry->d_name, FNM_CASEFOLD) == 0)
			{			
				// found it
				break;
			}
		}
		
		if (dirEntry != NULL)
		{
			// found matching entry - build result
			BuildFindFileResult(pFindFileHandle->pszDirectory, dirEntry->d_name, pFindFileInfo);			
			bResult = true;
		}
	}		
	
	return bResult;
}

// closes the find file operations
void LTFileOperations::FindClose(LTFINDFILEHANDLE hFindFile)
{
	if (hFindFile != (LTFINDFILEHANDLE)-1)
	{
		Linux_FindFileHandle* pFindFileHandle = (Linux_FindFileHandle*)hFindFile;
		
		if (pFindFileHandle->pHandle != (DIR*)NULL)
		{
			::closedir(pFindFileHandle->pHandle);
		}
		
		delete pFindFileHandle;
	}
}

void LTFileOperations::SplitPath(const char* pPath, char* pDirectory, char* pFileName, char* pSuffix)
{
	if (pPath == NULL)
	{
		// no data
		return;
	}

	char szPathCopy[MAX_PATH];
	LTStrCpy(szPathCopy, pPath, LTARRAYSIZE(szPathCopy));
	
	AdjustPathSeparatorsForLinux(szPathCopy);
	
	int nPathLength = LTStrLen(szPathCopy);
	int nDirectorySeparatorIndex;
	
	if (pDirectory)
	{
		pDirectory[0] = (char)NULL;
	}
	
	if (pFileName)
	{
		pFileName[0] = (char)NULL;
	}
	
	if (pSuffix)
	{
		pSuffix[0] = (char)NULL;
	}
	
	bool bFoundParentDirectory = false;
	
	// search right-to-left for directory separator
	for (nDirectorySeparatorIndex = nPathLength - 1; nDirectorySeparatorIndex >= 0; --nDirectorySeparatorIndex)
	{
		if (szPathCopy[nDirectorySeparatorIndex] == '/')
		{
			// found separator - everything to the left is the path
			if (pDirectory)
			{
				LTSubStrCpy(pDirectory, szPathCopy, _MAX_DIR, nDirectorySeparatorIndex + 1);
			}
			
			bFoundParentDirectory = true;
			break;
		}
	}
	
	// break remainder into filename and suffix
	bool bFoundSuffix = false;
	int nSuffixSeparatorIndex;
	for (nSuffixSeparatorIndex = nPathLength - 1; nSuffixSeparatorIndex != nDirectorySeparatorIndex; --nSuffixSeparatorIndex)
	{
		if (szPathCopy[nSuffixSeparatorIndex]  == '.')
		{
			// found separator - everything to the right is the suffix
			if (pSuffix)
			{
				LTSubStrCpy(pSuffix, szPathCopy + nSuffixSeparatorIndex, _MAX_EXT, nPathLength - nSuffixSeparatorIndex);			
			}
			
			bFoundSuffix = true;
			break;
		}
	}

	// copy file name if necessary
	if (pFileName)
	{
		int nStartFileNameIndex = 0;

		if (bFoundParentDirectory)
		{
			// path contained a directory, put filename start after the directory separator
			nStartFileNameIndex = nDirectorySeparatorIndex + 1;
		}
	
		uint32 nSuffixLength = 0;
		if (bFoundSuffix)
		{
			nSuffixLength = nPathLength - nSuffixSeparatorIndex;
		}
		
		LTSubStrCpy(pFileName, szPathCopy + nStartFileNameIndex, _MAX_FNAME, LTStrLen(szPathCopy + nStartFileNameIndex) - nSuffixLength);	
	}
}

void BuildFindFileResult(const char* pszDirectory, const char* pszFullFileName, LTFINDFILEINFO* pFindFileInfo)
{
	char pszFullFileNameWithPath[_MAX_DIR + _MAX_FNAME + _MAX_EXT + 1];
	::memset(pszFullFileNameWithPath, 0, LTARRAYSIZE(pszFullFileNameWithPath));
	if (pszDirectory[0] != (char)NULL)
	{
		LTSNPrintF(pszFullFileNameWithPath, LTARRAYSIZE(pszFullFileNameWithPath), "%s/%s", pszDirectory, pszFullFileName);
	}
	else
	{
		LTSNPrintF(pszFullFileNameWithPath, LTARRAYSIZE(pszFullFileNameWithPath), "%s", pszFullFileName);
	}
	
	struct stat statInfo;
	::stat(pszFullFileNameWithPath, &statInfo);
	
	pFindFileInfo->bIsSubdir   = S_ISDIR(statInfo.st_mode) ? true : false;
	pFindFileInfo->time_access = statInfo.st_atime;
	pFindFileInfo->time_write  = statInfo.st_mtime;
	pFindFileInfo->size        = (uint64)statInfo.st_size;
	LTStrCpy(pFindFileInfo->name, pszFullFileName, LTARRAYSIZE(pFindFileInfo->name));	
}

void LTFileOperations::AdjustPathSeparatorsForLinux(char* pszPath)
{
	// convert backslashes to forwardslashes
	for (uint32 index = 0; index < LTStrLen(pszPath); ++index)
	{
		if (pszPath[index] == '\\')
		{
			pszPath[index] = '/';
		}
	}
}	

void AdjustPathCaseForLinux(char* pszPath)
{
	// attempt to access the file
	struct stat64 statInfo;
	if (::stat64(pszPath, &statInfo) == -1)
	{
		// walk path components, doing case-insensitive find for each part
		char szNewPath[MAX_PATH];
		char szPathComponent[MAX_PATH];
		uint32 nPathIndex = 0;
		uint32 nNewPathIndex = 0;
		uint32 nPathComponentIndex = 0;
		
		::memset(szNewPath, 0, MAX_PATH);
		::memset(szPathComponent, 0, MAX_PATH);
		
		// handle first character specifying root directory
		if (pszPath[0] == '/')
		{
			szPathComponent[nPathComponentIndex++] = pszPath[nPathIndex];
			szNewPath[nNewPathIndex++] 			   = pszPath[nPathIndex++];
		}
		
		for (nPathIndex; nPathIndex <= LTStrLen(pszPath); ++nPathIndex)
		{
			if ((pszPath[nPathIndex] == '/') || (pszPath[nPathIndex] == (char)NULL))
			{
				// attempt to find this component
				szPathComponent[nPathComponentIndex] = (char)NULL;
				
				LTFINDFILEINFO cFileInfo;
				LTFINDFILEHANDLE hFindHandle;
				if (!LTFileOperations::FindFirst(szPathComponent, hFindHandle, &cFileInfo))
				{
					// directory does not exist even with case insensitive compare
					return;
				}
				else
				{
					LTFileOperations::FindClose(hFindHandle);
					
					// get the actual file name and add it to new path
					LTStrCat(szNewPath, cFileInfo.name, MAX_PATH - LTStrLen(szNewPath));
					if (cFileInfo.bIsSubdir)
					{
						LTStrCat(szNewPath, "/", MAX_PATH - LTStrLen(szNewPath));					
					}
					else
					{
						// found a leaf node so we're finished
						break;
					}
					
					// copy the new path to our working buffer as well
					LTSNPrintF(szPathComponent, LTARRAYSIZE(szPathComponent), "%s", szNewPath);
					nPathComponentIndex = LTStrLen(szPathComponent);				
				}	
			}
			else
			{
				// add the next character to our path component
				szPathComponent[nPathComponentIndex++] = pszPath[nPathIndex];
			}
		}
				
		// replace with correct case version (new version will be the same length as original)
		LTStrCpy(pszPath, szNewPath, LTStrLen(szNewPath) + 1);
	}
}

