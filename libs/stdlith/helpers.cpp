//------------------------------------------------------------------
//
//	FILE	  : Helpers.cpp
//
//	PURPOSE	  : Implements the CHelpers class.
//
//	CREATED	  : November 26 1996
//
//	COPYRIGHT : Microsoft 1996 All Rights Reserved
//
//------------------------------------------------------------------

// Includes....
#include "helpers.h"
#include "ltinteger.h"
#include <stdlib.h>
#include <string.h>

bool CHelpers::ExtractPathAndFileName( const char *pInputPath, char *pPathName, char *pFileName )
{
	pPathName[0] = pFileName[0] = 0;

	uint32 len = strlen(pInputPath);
	int lastDelimiter = -1;
	
	for(uint32 i=0; i < len; i++ )
	{
		if( pInputPath[i] == '\\' || pInputPath[i] == '/' )
		{
			lastDelimiter = i;
		}
	}

	if( lastDelimiter == -1 )
	{
		pPathName[0] = 0;
		strcpy( pFileName, pInputPath );
	}
	else
	{
		memcpy( pPathName, pInputPath, lastDelimiter );
		pPathName[lastDelimiter] = 0;
	
		memcpy( pFileName, &pInputPath[lastDelimiter+1], len-lastDelimiter );
		pFileName[len-lastDelimiter] = 0;
	}

	return true;
}


bool CHelpers::ExtractFileNameAndExtension( const char *pInputFilename, char *pFilename, char *pExtension )
{
	uint32 len = strlen(pInputFilename);
	uint32 lastDot = len;

	for(uint32 i=0; i < len; i++ )
	{
		if( pInputFilename[i] == '.' )
		{
			lastDot = i;
		}
	}
	
	memcpy( pFilename, pInputFilename, lastDot );
	pFilename[lastDot] = 0;

	memcpy( pExtension, &pInputFilename[lastDot+1], len-lastDot );
	pExtension[len-lastDot] = 0;

	return true;
}


bool CHelpers::ExtractNames( const char *pFullPath, char *pPathname, char *pFilename, char *pFileTitle, char *pExt )
{
	char		pathName[256], fileName[256], fileTitle[256], ext[256];


	ExtractPathAndFileName( pFullPath, pathName, fileName );
	ExtractFileNameAndExtension( fileName, fileTitle, ext );

	if( pPathname )
		strcpy( pPathname, pathName );

	if( pFilename )
		strcpy( pFilename, fileName );

	if( pFileTitle )
		strcpy( pFileTitle, fileTitle );

	if( pExt )
		strcpy( pExt, ext );
	
	return true;
}

//determines if the file is relative, or is an absolute path (uses the : as a key)
bool CHelpers::IsFileAbsolute(const char* pFilename)
{
	for(uint32 nCurrChar = 0; (pFilename[nCurrChar] != '\0') && (pFilename[nCurrChar] != '\\') && (pFilename[nCurrChar] != '/'); nCurrChar++)
	{
		//we hit a colon before the slash or end of the string,
		//this means that it is absolute
		if(pFilename[nCurrChar] == ':')
		{
			return true;
		}
	}

	return false;
}

