// *********************************************************************** //
//
// MODULE  : win32_ltfileoperations.cpp
//
// PURPOSE : Win32 implementation of LTFileOperations.
//
// CREATED : 05/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltfileoperations.h"
#include <io.h>
#include <direct.h>
#include <shlobj.h>
#include "resourceextensions.h"
#include "ltgamecfg.h"

extern char g_szUserDirectory[MAX_PATH];

// helper function for transferring find file results
static void ProcessFindFileResult(const _finddata_t& fileInfo, LTFINDFILEINFO* pFindFileInfo)
{
	// transfer results to LTFINDFILEINFO
	pFindFileInfo->bIsSubdir   = fileInfo.attrib & _A_SUBDIR ? true : false;
	pFindFileInfo->time_access = fileInfo.time_access;
	pFindFileInfo->time_write  = fileInfo.time_write;
	pFindFileInfo->size        = (uint64)fileInfo.size;

	LTStrCpy(pFindFileInfo->name, fileInfo.name, LTARRAYSIZE(pFindFileInfo->name));
}

bool LTFileOperations::DeleteFile(const char* pFileName)
{
	return !!::DeleteFile(pFileName);
}

bool LTFileOperations::GetFileSize(const char* pFileName, uint64& nFileSize)
{
	nFileSize = 0;

	WIN32_FILE_ATTRIBUTE_DATA fad;
	if( !::GetFileAttributesExA(pFileName, GetFileExInfoStandard, &fad) )
		return false;

	nFileSize = ((uint64)fad.nFileSizeHigh << 32) | fad.nFileSizeLow;

	return true;
}

void LTFileOperations::GetCurrentWorkingDirectory(char* pDirectoryBuffer, uint32 nDirectoryBufferSize)
{
	::_getcwd(pDirectoryBuffer, nDirectoryBufferSize);
}

#define USER_SUB_DIRECTORY FILE_PATH_SEPARATOR RESEXT_PUBLISHERNAME FILE_PATH_SEPARATOR 

void LTFileOperations::GetUserDirectory(char* pDirectoryBuffer, uint32 nDirectoryBufferSize)
{
	char szUserDirRootName[MAX_PATH] = "";
	if( LTGameCfg::IsMPFreeProduct())
	{
		LTStrCpy( szUserDirRootName, RESEXT_USERFOLDER_ROOT_MPFREE, LTARRAYSIZE( szUserDirRootName ));
	}
	else
	{
		LTStrCpy( szUserDirRootName, RESEXT_USERFOLDER_ROOT_RETAIL, LTARRAYSIZE( szUserDirRootName ));
	}
	GetUserDirectory( szUserDirRootName, pDirectoryBuffer, nDirectoryBufferSize );
}

void LTFileOperations::GetUserDirectory(char const* pszRootName, char* pDirectoryBuffer, uint32 nDirectoryBufferSize)
{
	if (LTStrEmpty(g_szUserDirectory))
	{
		// if the incoming buffer is larger then the needed size, then copy it directly into
		//  that buffer
		char pTmpDirectoryBuffer[MAX_PATH];
		if ( SUCCEEDED( SHGetFolderPath( NULL, CSIDL_COMMON_DOCUMENTS|CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, pTmpDirectoryBuffer ) ) )
		{
			// Only copy it into the incoming buffer if there is room for all of it.
			//  otherwise leave that buffer unchanged.
			if ( LTStrLen( pTmpDirectoryBuffer ) + LTStrLen( USER_SUB_DIRECTORY ) + LTStrLen( pszRootName ) + LTStrLen( FILE_PATH_SEPARATOR ) < nDirectoryBufferSize )
			{
				LTStrCpy( pDirectoryBuffer, pTmpDirectoryBuffer, nDirectoryBufferSize );
				LTStrCat( pDirectoryBuffer, USER_SUB_DIRECTORY, nDirectoryBufferSize );

				// Write out the game name if provided.
				if( !LTStrEmpty( pszRootName ))
				{
					LTStrCat( pDirectoryBuffer, pszRootName, nDirectoryBufferSize );
					LTStrCat( pDirectoryBuffer, FILE_PATH_SEPARATOR, nDirectoryBufferSize );
				}

				// Make sure the full path exists.
				CreateNewDirectory( pDirectoryBuffer );
			}
		}
	}
	else
	{
		LTStrCpy(pDirectoryBuffer, g_szUserDirectory, nDirectoryBufferSize);
		LTStrCat(pDirectoryBuffer, FILE_PATH_SEPARATOR, nDirectoryBufferSize);
	}
}

bool LTFileOperations::GetTempFilePath(char* pszTempPathBuffer, uint32 nTempPathBufferSize)
{
	if(!GetTempPath(nTempPathBufferSize, pszTempPathBuffer)) 
	{
		return false;
	}
	
	return true;
}
	
bool LTFileOperations::GetTempFileName(const char* pszTempFilePath, const char* pszTempFileNamePrefix, char* pszTempFileNameBuffer, uint32 nTempFileNameBufferSize)
{
	if(!::GetTempFileName(pszTempFilePath, pszTempFileNamePrefix, 0, pszTempFileNameBuffer))
	{
		return false;
	}
	
	return true;
}

bool LTFileOperations::CreateNewDirectoryPlatformHook(const char* pDirectoryName)
{
	return !!CreateDirectory(pDirectoryName, NULL);
}

bool LTFileOperations::DeleteDirectory(const char* pDirectoryName)
{
	return !!RemoveDirectory(pDirectoryName);
}

bool LTFileOperations::FileExists(const char* pFileName)
{
	return (0xFFFFFFFF != ::GetFileAttributes(pFileName));
}

bool LTFileOperations::FindFirst(const char* pFindPath, LTFINDFILEHANDLE& Handle, LTFINDFILEINFO* pFindFileInfo)
{
	// call win32 _findfirst
	_finddata_t fileInfo;
	intptr_t pFindHandle = _findfirst(pFindPath, &fileInfo);

	// put the results into the outgoing structure
	if (pFindHandle == -1)
		return false;

	ProcessFindFileResult(fileInfo, pFindFileInfo);
	Handle = (LTFINDFILEHANDLE)pFindHandle;
	return true;
}

// finds the next occurrence of a file matching the path specified to FindFirst
bool LTFileOperations::FindNext(LTFINDFILEHANDLE hFindFile, LTFINDFILEINFO* pFindFileInfo)
{
	// call win32 _findnext
	_finddata_t fileInfo;
	int nResult = _findnext((intptr_t)hFindFile, &fileInfo);

	// if we got a hit, process the result
	if (nResult == -1)
		return false;

	ProcessFindFileResult(fileInfo, pFindFileInfo);
	return true;
}

// closes the find file operations
void LTFileOperations::FindClose(LTFINDFILEHANDLE hFindFile)
{
	_findclose((intptr_t)hFindFile);
}

void LTFileOperations::SplitPath(const char* pPath, char* pDirectory, char* pFileName, char* pSuffix)
{
	// the drive letter, if present, will be stored in this buffer
	char szDriveLetter[MAX_PATH];
	memset(szDriveLetter, 0, LTARRAYSIZE(szDriveLetter));

	// call win32 splitpath
	_splitpath(pPath, szDriveLetter, pDirectory, pFileName, pSuffix);

	// if the path included a drive letter, prepend it onto the returned directory
	if (pDirectory && szDriveLetter[0])
	{
		LTASSERT(LTStrLen(pDirectory) < (MAX_PATH - _MAX_DRIVE), "insufficient buffer space");

		// append the directory returned by splitpath to the drive letter
		LTStrCat(szDriveLetter, pDirectory, LTARRAYSIZE(szDriveLetter));

		// now copy it back to the original buffer
		LTStrCpy(pDirectory, szDriveLetter, LTStrLen(pDirectory) + _MAX_DRIVE);
	}
}
