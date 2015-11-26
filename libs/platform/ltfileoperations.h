// *********************************************************************** //
//
// MODULE  : ltfileoperations.h
//
// PURPOSE : Helpers for filesystem operations.
//
// CREATED : 05/24/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTFILEOPERATIONS_H__
#define __LTFILEOPERATIONS_H__

#include "platform.h"
#include "ltinteger.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// types
typedef void* LTFINDFILEHANDLE;

struct LTFINDFILEINFO 
{
	bool	bIsSubdir;
	time_t	time_access;
	time_t	time_write;
	uint64	size;
	char	name[_MAX_PATH];
};

// function prototype for the text file parsing callback
typedef bool (*TProcessLineFn)(const char* strLine, void* pProcessLineUserData);

class LTFileOperations
{
public:

	// deletes the specified file.  Returns false if an error occurs.
	static bool DeleteFile(const char* pFileName);
	
	// gets the size of an open file - note that this may reset the file pointer to the
	// start of the file
	static bool GetFileSize(const char* pFileName, uint64& nFileSize);

	// gets the current working directory of the process
	static void GetCurrentWorkingDirectory(char* pDirectoryBuffer, uint32 nDirectoryBufferSize);

	// gets the base directory to be used to create runtime writable files.
	//  i.e. profiles, save game files, screen-shots.
	static void GetUserDirectory(char* pDirectoryBuffer, uint32 nDirectoryBufferSize);

	// gets the base directory to be used to create runtime writable files based on provided gamename.
	//  i.e. profiles, save game files, screen-shots.
	static void GetUserDirectory(char const* pszGameName, char* pDirectoryBuffer, uint32 nDirectoryBufferSize);

	// override the user directory for runtime writable files 
	static void SetUserDirectory(const char* pszDirectoryName);

	// gets the path for temporary files
	static bool GetTempFilePath(char* pszTempPathBuffer, uint32 nTempPathBufferSize);
	
	// gets a temporary file name
	static bool GetTempFileName(const char* pszTempFilePath, const char* pszTempFileNamePrefix, char* pszTempFileNameBuffer, uint32 nTempFileNameBufferSize);
	
	// tests if the specified directory exists
	static bool DirectoryExists(const char* pDirectoryName);

	// creates the specified directory
	static bool CreateNewDirectory(const char* pDirectoryName);

	// deletes the specified directory
	static bool DeleteDirectory(const char* pDirectoryName);

	// tests if the specified file exists
	static bool FileExists(const char* pFileName);

	// finds the first occurrence of a file matching the specified path.
	static bool FindFirst(const char* pFindPath, LTFINDFILEHANDLE& Handle, LTFINDFILEINFO* pFindFileInfo);

	// finds the next occurrence of a file matching the path specified to FindFirst
	static bool FindNext(LTFINDFILEHANDLE hFindFile, LTFINDFILEINFO* pFindFileInfo);

	// closes the find file operations
	static void FindClose(LTFINDFILEHANDLE hFindFile);

	// splits a path into its parts
	static void SplitPath(const char* pPath, char* pDirectory, char* pFileName, char* pSuffix);
	
	// removes relative directories from a path.  performed in-place.
	static void RemoveRelativePaths(char* pPath);

	// parses a text buffer and calls the specified callback function for each line - note that
	// this is platform neutral with respect to newline sequences.  The user data argument is 
	// passed to the callback function each time a line is successfully read from the stream.
	// Note that in addition to the filename based method, a method is provided to process a text
	// buffer, which can be used to parse lines out of an ILTInStream.
	static bool ParseTextFile(const char* pszFilename, TProcessLineFn pfnProcessLine, void* pProcessLineUserData);
	static bool ParseTextBuffer(const char* pBuffer, uint32 nBufferLength, TProcessLineFn pfnProcessLine, void* pProcessLineUserData);

#if defined(PLATFORM_LINUX)
	
	// adjust path separators to make a Linux compatible path
	static void AdjustPathSeparatorsForLinux(char* pszPath);
	
#endif  

private: 

	// platform-specific implementation helper for CreateNewDirectory
	static bool CreateNewDirectoryPlatformHook(const char* pDirectory);

};

#endif // __LTFILEOPERATIONS_H__

