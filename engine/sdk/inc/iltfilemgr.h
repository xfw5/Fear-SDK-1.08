//----------------------------------------------------------------------------------------
// ILTFileMgr.h
//
// Provides the definition for the ILTFileMgr interface, which provides access to various
// underlying file functionality. 
//
//----------------------------------------------------------------------------------------

#ifndef __ILTFILEMGR_H__
#define __ILTFILEMGR_H__

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

class ILTFileMgr:
	public IBase
{
public:

	//Module support
	interface_version(ILTFileMgr, 0);

	virtual ~ILTFileMgr()		{}

	//-------------------------------------------------------------------------
	// File Access
	//
	// Provides access to files within the directory trees
	//-------------------------------------------------------------------------

	//given a filename, this will attempt to open the specified file from one of the registered
	//directory trees. Upon success this will return the stream that can be used to read from
	//that file, and this object must be released. If the file cannot be found or opened for reading
	//NULL will be returned from this function
	virtual ILTInStream*	OpenFile(const char* pszFilename) = 0;

	//given a filename, it will find this file in the file tree. If the file exists within a directory
	//tree, this will simply provide the full absolte name to the file. If it exists within a file
	//archive, it will extract the file to a temporary location and will fill in the name of that
	//temporary file. This can fail, and if so, the contents of the provided filename will be left
	//unaltered.
	virtual LTRESULT		ExtractFile(const char* pszFilename, char* pszFullFilename, uint32 nBufferLen, bool& bExtracted) = 0;

	//given the filename of a file that was extracted (ExtractFile succeeded, and the extracted output
	//parameter was set to true) this will handle deleting that file which is recommended to clean
	//up temporary file names that might otherwise be left around
	virtual LTRESULT		DeleteExtractedFile(const char* pszExtractedFile) = 0;

	//given a filename, this will determine the absolute path to the matching file. This will return
	//false if the file cannot be found, or if the file is within an archive. Note: This is intended
	//for debugging tools ONLY as this will almost always fail in final builds
	virtual LTRESULT		GetAbsoluteFilename(const char* pszFilename, char* pszAbsolute, uint32 nBufferLen) = 0;


	//-------------------------------------------------------------------------
	// File List
	//
	// Used to enumerate files within a directory within the file trees. This will not enumerate user
	// files.
	//-------------------------------------------------------------------------

	//given a directory, this will enumerate the files and subdirectories within that directory, and
	//return them through a list of FileEntry's which can be traversed like a linked list. This
	//returned list must be freed through the function FreeFileList. This will return the head of
	//the linked list if successful, otherwise NULL
	virtual FileEntry*		GetFileList(const char* pszDirectory) = 0;

	//given a file list acquired through GetFileList, this will free the list appropriately. The
	//provided pointer or elements within the list must not be used after this call is made
	virtual void			FreeFileList(FileEntry* pFileList) = 0;

	//-------------------------------------------------------------------------
	// User Files
	//
	// These files are files that are dynamically created by the game. These are not interchangeable with
	// normal files since these can be mapped to various locations such as memory cards, user directories, 
	// etc.
	//-------------------------------------------------------------------------

	//called to open up a custom user file for writing. These files are relative to a platform
	//specific root. This file can then be written to. In order to open this up again, the function
	//OpenUserFileForReading must be used. This returned stream object must be released.
	virtual ILTOutStream*	OpenUserFileForWriting(const char* pszFilename) = 0;

	//called to open of a file that was previously written using OpenUserFileForWriting. This will
	//return NULL if the file could not be found. This will use the same platform specific root
	//that was used by OpenUserFileForWriting. This returned stream object must be released
	virtual ILTInStream*	OpenUserFileForReading(const char* pszFilename) = 0;

	//given two filenames, this will perform a copy of the source file to the destination file. This
	//will return LT_OK upon success or an appropriate error code otherwise. This will not delete
	//the source file, and will also fail if the target filename is not writeable
	virtual LTRESULT		CopyUserFile(const char* pszSourceFile, const char* pszDestFile) = 0;

	//called to delete a user file given a relative user file name
	virtual LTRESULT		DeleteUserFile(const char* pszRelativeName) = 0;

	//given a relative user file name, this will return the absolute user file name
	virtual LTRESULT		GetAbsoluteUserFileName(const char* pszRelative, char* pszAbsolute, uint32 nAbsoluteLen) = 0;
};

#endif
