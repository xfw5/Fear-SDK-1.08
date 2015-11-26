//------------------------------------------------------------------
//
//  FILE      : Helpers.h
//
//  PURPOSE   : Defines the CHelpers class, which contains lots of
//              helpful little functions.
//
//  CREATED   : November 26 1996
//
//  COPYRIGHT : Microsoft 1996 All Rights Reserved
//
//------------------------------------------------------------------

#ifndef __HELPERS_H__
#define __HELPERS_H__

class CHelpers
{
public: 

	static bool	ExtractPathAndFileName(const char *pInputPath, char *pPathName, char *pFileName);
    static bool	ExtractFileNameAndExtension(const char *pInputFilename, char *pFilename, char *pExtension);
    static bool	ExtractNames(const char *pFullPath, char *pPathname, char *pFilename, char *pFiletitle, char *pExt);

	//determines if the file is relative, or is an absolute path (uses the : as a key)
	static bool	IsFileAbsolute(const char* pFilename);
};


#endif  // __HELPERS_H__

