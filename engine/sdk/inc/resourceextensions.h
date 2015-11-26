//-----------------------------------------------------------------------------------------
// ResourceExtensions.h
//
// Maintains a listing of the different resource type extensions that are used by the tools
// and engine so that they can be easily changed in a single location for different products.
// This is intended to be changed for each product to allow for uniquely identifying resources
// to products and insulate the codebase from resource extension changes.
//
//-----------------------------------------------------------------------------------------
#ifndef __RESOURCEEXTENSIONS_H__
#define __RESOURCEEXTENSIONS_H__

//include common definitions for types and string operations
#ifndef __LTBASEDEFS_H__
#	include "ltbasedefs.h"
#endif
#include <string>

// Define used to uniquely identify this product from other products.
#define		RESEXT_GAMENAME					"FEAR"
// Name of client window and class name
#define		RESEXT_WINDOWNAME				RESEXT_GAMENAME
// Used to encode a world with a hash value to make different products incompatible.
#define		RESEXT_WORLDHASH				RESEXT_GAMENAME
// Used to define the root name of databases.
#define		RESEXT_DATABASE_ROOT			RESEXT_GAMENAME
// Used to name the socket for networked games.
#define		RESEXT_NETWORKSESSIONNAME		RESEXT_GAMENAME

// Used to define the root registry name.
#define		RESEXT_REGISTRY_ROOT_RETAIL		"FEAR"
#define		RESEXT_REGISTRY_ROOT_MPFREE		"FEARCombat"
// Used to define the root user folder name.
#define		RESEXT_USERFOLDER_ROOT_RETAIL	"FEAR"
#define		RESEXT_USERFOLDER_ROOT_MPFREE	"FEARCombat"


// These defines are only used by the Engine runtime.  This is used to support
// the MPFree version of the main executable.
#if !defined( USE_MP_RESEXT_VALUES )
	// this string must be unique so a GUID is a nice string to use
	// this string is limited to MAX_PATH characters. comparison is case sensitive. 
	#define		RESEXT_UNIQUE_PROCESS_STRING	"{CB7DD3FC-417D-4c29-9BFD-CA781BF1B207}"
	// Default caption for main window.
	#define		RESEXT_GAMEDISPLAYNAME			"F.E.A.R."
#else // !defined( USE_MP_RESEXT_VALUES )
	// this string must be unique so a GUID is a nice string to use
	// this string is limited to MAX_PATH characters. comparison is case sensitive. 
	#define		RESEXT_UNIQUE_PROCESS_STRING	"{573B4145-8689-4c94-B1D8-8FACAF6010F6}"
	// Default caption for main window.
	#define		RESEXT_GAMEDISPLAYNAME			"F.E.A.R."
#endif // !defined( USE_MP_RESEXT_VALUES )

#define		RESEXT_PUBLISHERNAME			"Monolith Productions"
// the ID of this game
#define		RESEXT_GAMEID					0
#define		RESEXT_GAMEID_STRING			"00"

//-----------------------------------------------------------------------------------------
// File Extensions
//  This part of the file contains the bare extensions for the different resource types
// grouped by the resource they correspond to. None of these extension
//-----------------------------------------------------------------------------------------

//--------------------------------------------------
//World extension types
#define		RESEXT_WORLD_UNCOMPRESSED		"World00a"
#define		RESEXT_WORLD_COMPRESSED			"World00c"
#define		RESEXT_WORLD_PACKED				"World00p"

//--------------------------------------------------
//Mesh extension types
#define		RESEXT_MESH_PACKED				"Mesh00p"

//--------------------------------------------------
//Packed object types
#define		RESEXT_OBJECTS_PACKED			"Objects00p"

//--------------------------------------------------
//Model extension types
#define		RESEXT_MODEL_UNCOMPRESSED		"Model00a"
#define		RESEXT_MODEL_COMPRESSED			"Model00c"
#define		RESEXT_MODEL_PACKED				"Model00p"

//--------------------------------------------------
//Effect extension types
#define		RESEXT_EFFECT_UNCOMPRESSED		"Fx00a"
#define		RESEXT_EFFECT_COMPRESSED		"Fx00c"
#define		RESEXT_EFFECT_PACKED			"Fx00p"

//--------------------------------------------------
//Key extension types
#define		RESEXT_KEY_UNCOMPRESSED			"Key00a"
#define		RESEXT_KEY_COMPRESSED			"Key00c"

//--------------------------------------------------
//Texture extension types
#define		RESEXT_TEXTURE					"dds"

//--------------------------------------------------
//Texture animation extension types
#define		RESEXT_TEXTUREANIM				"TxAnm00"
#define		RESEXT_TEXTUREANIM_UNCOMPRESSED	"TxAnm00a"
#define		RESEXT_TEXTUREANIM_COMPRESSED	"TxAnm00c"
#define		RESEXT_TEXTUREANIM_PACKED		"TxAnm00p"
#define		RESEXT_TEXTUREANIM_DEPENDENCY_COMPRESSED	"TxAnm00dc"
#define		RESEXT_TEXTUREANIM_DEPENDENCY_UNCOMPRESSED	"TxAnm00da"

//--------------------------------------------------
//Video extension types
#define		RESEXT_VIDEO_BINK				"bik"
#define		RESEXT_VIDEO_UNCOMPRESSED		"Video00a"
#define		RESEXT_VIDEO_COMPRESSED			"Video00c"
#define		RESEXT_VIDEO_PACKED				"Video00p"

//--------------------------------------------------
//Material extension types
#define		RESEXT_MATERIAL					"Mat00"
#define		RESEXT_MATERIAL_UNCOMPRESSED	"Mat00a"
#define		RESEXT_MATERIAL_COMPRESSED		"Mat00c"
#define		RESEXT_MATERIAL_PACKED			"Mat00p"
#define		RESEXT_MATERIAL_LIBRARY_SOURCE	"MatLib00s"
#define		RESEXT_MATERIAL_LIBRARY_PACKED	"MatLib00p"
#define		RESEXT_MATERIAL_DEPENDENCY_COMPRESSED	"Mat00dc"
#define		RESEXT_MATERIAL_DEPENDENCY_UNCOMPRESSED	"Mat00da"

//--------------------------------------------------
//Shader extension types
#define		RESEXT_SHADER_SOURCE			"fx"
#define		RESEXT_SHADER_COMPILED			"fxo"
#define		RESEXT_SHADER_HEADER			"fxh"
#define		RESEXT_SHADER_INTERNAL			"fxi"
#define		RESEXT_SHADER_UNCOMPRESSED		"Shader00a"
#define		RESEXT_SHADER_COMPRESSED		"Shader00c"

//--------------------------------------------------
//Game Database extension types
#define		RESEXT_GAMEDATABASE_SOURCE		"Gamdb00s"
#define		RESEXT_GAMEDATABASE_PACKED		"Gamdb00p"
#define		RESEXT_GAMEDATABASE_DEPENDENCY_COMPRESSED	"Gamdb00dc"
#define		RESEXT_GAMEDATABASE_DEPENDENCY_UNCOMPRESSED	"Gamdb00da"

//--------------------------------------------------
//StringEdit extension types
#define		RESEXT_STRINGEDIT_SOURCE		"Strdb00s"
#define		RESEXT_STRINGEDIT_EXTERNAL		"Strdb00e"
#define		RESEXT_STRINGEDIT_PACKED		"Strdb00p"
#define		RESEXT_STRINGEDIT_DEPENDENCY_COMPRESSED		"Strdb00dc"
#define		RESEXT_STRINGEDIT_DEPENDENCY_UNCOMPRESSED	"Strdb00da"
// location where packed files are saved
#define		RESEXT_STRINGEDIT_PACKLOC		"Strdb00l"

//--------------------------------------------------
//Sound extension types
#define		RESEXT_SOUND					"wav"
#define		RESEXT_SOUND_UNCOMPRESSED		"Sound00a"
#define		RESEXT_SOUND_COMPRESSED			"Sound00c"
#define		RESEXT_SOUND_XSB				"xsb"
#define		RESEXT_SOUND_XWB				"xwb"

//--------------------------------------------------
//Asset list extension types
#define		RESEXT_ASSETLIST				"Asset00"

//--------------------------------------------------
//Animation Tree extension types
#define		RESEXT_ANIMTREE_PACKED			"AnmTree00p"

//--------------------------------------------------
//Lip Sync extension types
#define		RESEXT_LIPSYNC					"lip00"

//--------------------------------------------------
//Archive extension types
#define		RESEXT_ARCHIVE					"Arch00"
#define		RESEXT_ARCHIVE_SCRIPT			"Arch00s"

//--------------------------------------------------
//Project extension types
#define		RESEXT_PROJECT_LOCATOR			"proj00"

//--------------------------------------------------
//Resource extension types
#define		RESEXT_TEXTURE_UNCOMPRESSED		"Texture00a"
#define		RESEXT_TEXTURE_COMPRESSED		"Texture00c"
#define		RESEXT_TEXTURE_PACKED			"Texture00p"

//--------------------------------------------------
//Asset database (dependencies)
#define		RESEXT_ASSETDATABASE_SOURCE		"AssetDB00s"


//------------------------------------------------------------------------------------------
// Extension utilities
//------------------------------------------------------------------------------------------

#define		RESEXT_INTERNAL_UNICODE(x)	L##x

//utility macro that given an extension listed above, will create a static string pointer that 
//points to the name of the extension with a '.' prefix on it, an example would be
//RESEXT_DOT(RESEXT_GAMEDATABASE_SOURCE)
#define		RESEXT_DOT(Extension)			"." Extension
#define		RESEXT_DOT_UNICODE(Extension)	L"." Extension

//utility macro that converts the extension to a Unicode version, an example of this would
//be RESEXT_UNICODE(RESEXT_GAMEDATABASE_SOURCE)
#define		RESEXT_UNICODE(Extension)		RESEXT_INTERNAL_UNICODE(Extension)


//all utility functions are wrapped in a class to prevent global namespace conflicts
class CResExtUtil
{
public:

	//utility function that will determine the index of the extension of a file. This index
	//will be the position after the '.' if one is found, otherwise it will return -1
	template <class CharType>
	static int32	GetExtensionIndex(const CharType* pszFilename)
	{
		//start at the end of the string
		uint32 nFileLen = LTStrLen(pszFilename);

		//run backwards to find the ., but bail if we hit a slash as that indicates the end of the
		//file title
		for(int32 nCurrFilePos = (int32)nFileLen - 1; nCurrFilePos >= 0; nCurrFilePos--)
		{
			//see if we hit a dot, indicating the extension
			if(pszFilename[nCurrFilePos] == (CharType)'.')
				return nCurrFilePos + 1;

			//see if we hit a directory delimiter
			if((pszFilename[nCurrFilePos] == (CharType)'/') || (pszFilename[nCurrFilePos] == (CharType)'\\'))
				return -1;
		}

		//no match, meaning we just had a file title with no extension
		return -1;
	}

	//utility function that determines if the passed in file name matches the specified extentsion
	template <class CharType>
	static bool	IsFileOfType(const CharType* pszFilename, const CharType* pszExtension)
	{
		if(!pszFilename || !pszExtension)
			return false;

		//find out the extension string
		int32 nExtensionIndex = GetExtensionIndex(pszFilename);

		//see if an index is provided
		if(nExtensionIndex < 0)
			return false;

		//now determine the extension and test it
		const CharType* pszFileExt = pszFilename + nExtensionIndex;
		return LTStrIEquals(pszFileExt, pszExtension);
	}

	//called to set the extension of a filename. This will change the current extension if there
	//is one, or add one if there isn't.
	template <class CharType>
	static bool SetFileExtension(CharType* pszFilename, uint32 nFileBufferLen, const CharType* pszExtension)
	{
		if(!pszFilename || !pszExtension)
			return false;

		//first off, find the extension
		int32 nExtensionIndex = GetExtensionIndex(pszFilename);

		//and now see if we are just adding one or not
		if(nExtensionIndex < 0)
		{
			const CharType szPeriod[2] = {'.', '\0'};
			//we are adding one
			LTStrCat(pszFilename, szPeriod, nFileBufferLen);
			LTStrCat(pszFilename, pszExtension, nFileBufferLen);
		}
		else
		{
			//we are changing it from an existing extension
			LTStrCpy(pszFilename + nExtensionIndex, pszExtension, nFileBufferLen - (uint32)nExtensionIndex);
		}

		//success
		return true;
	}

	// std::string version
	static bool SetFileExtension(std::string& strFilename, const char* pszExtension)
	{
		//first off, find the extension
		int32 nExtensionIndex = GetExtensionIndex(strFilename.c_str());

		//and now see if we are just adding one or not
		if(nExtensionIndex < 0)
		{
			//we are adding one
			strFilename += '.';
			strFilename += pszExtension;
		}
		else
		{
			//we are changing it from an existing extension
			strFilename.erase( nExtensionIndex, strFilename.npos );
			strFilename += pszExtension;
		}

		//success
		return true;
	}

	//called to strip the extension of a filename. 
	template <class CharType>
	static bool StripFileExtension(CharType* pszFilename, uint32 nFileBufferLen)
	{
		if(!pszFilename)
			return false;

		//first off, find the extension
		int32 nExtensionIndex = GetExtensionIndex(pszFilename);

		//see if it even has an extension to begin with
		if(nExtensionIndex < 0)
			return true;

		//the index returned is always after the '.'
		pszFilename[nExtensionIndex-1] = (CharType)'\0';

		//success
		return true;
	}

	//called to strip the extension of a filename. 
	//std::string version 
	static bool StripFileExtension(std::string& strFilename)
	{
		//first off, find the extension
		int32 nExtensionIndex = GetExtensionIndex(strFilename.c_str());

		//see if it even has an extension to begin with
		if(nExtensionIndex < 0)
			return true;

		//the index returned is always after the '.'
		strFilename.erase( nExtensionIndex - 1 );

		//success
		return true;
	}
};


#endif
