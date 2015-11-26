// *********************************************************************** //
//
// MODULE  : linux_ltfilebase.h
//
// PURPOSE : Base Linux implementation class for ILTFileRead and ILTFileWrite.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LINUX_LTFILEBASE_H__
#define __LINUX_LTFILEBASE_H__

#include <string>
#include "ltassert.h"
#include "ltfileoperations.h"
#include <sys/stat.h>

class CLinux_LTFileBase
{
protected:

	CLinux_LTFileBase();
	~CLinux_LTFileBase();

	// access to the underlying file handle
	FILE* GetFileHandle() const { return m_hFile; }

	// opens the file in the specified mode, and populates the internal file handle
	// member.  This will close a previously opened file prior to opening the new 
	// file. Returns false if an error occurs during the open operation.
	enum EOpenMode { eOpenRead, eOpenWrite, eOpenWriteAppend };
	bool OpenImpl(const char* pszFilename, EOpenMode eOpenMode);

	// test to see if this object has an open file.  Returns false if an error
	// occurs during the operation.
	bool GetIsOpenImpl(bool& bIsOpen);
	
	// seek to the specified position in the file relative to the start of the file.
	// Returns false if an error occurs during the seek.
	bool SeekImpl(uint64 nPos);

	// seek to the current end of file.  Returns false if an error occurs during the
	// seek operation.
	bool SeekToEndImpl();

	// return the current position based on the start of the file.  Returns false if 
	// an error occurs while retrieving the position.
	bool GetPosImpl(uint64& nPos);

	// test to see if the file is at EOF.  Returns false if an error occurs while
	// testing for EOF.
	bool GetIsEOFImpl(bool& bIsEOF);

	// return the size of the currently opened file.  Returns false if an error occurs
	// while determining the file size.
	bool GetFileSizeImpl(uint64& nSize);

	// close the file.  Returns false if an error occurs while closing the file.
	bool CloseImpl();

private:

	// helper for adjusting the path during the open operation
	void AdjustPathCaseForLinux(char* pszPath);

	// file handle
	FILE* m_hFile;

};

inline CLinux_LTFileBase::CLinux_LTFileBase()
	: m_hFile(NULL)
{
}

inline CLinux_LTFileBase::~CLinux_LTFileBase()
{
	// close the file if necessary
	CloseImpl();
}

inline bool CLinux_LTFileBase::OpenImpl(const char* pszFilename, EOpenMode eOpenMode)
{
	// close the file handle if it's already open
	CloseImpl();

	// build the arguments for the API call
	std::string strMode;

	switch(eOpenMode)
	{
		case eOpenRead:
		{
			strMode = "rb";
		}
		break;
		case eOpenWrite:
		{
			strMode = "wb";
		}
		break;
		case eOpenWriteAppend:
		{
			strMode = "ab";
		}
		break;
		default:
		{
			LTERROR("invalid open mode");
			return false;
		}
	}

	FILE* hFile = NULL;
	char pFileNameCopy[MAX_PATH];
	LTStrCpy(pFileNameCopy, pszFilename, MAX_PATH);
	
	LTFileOperations::AdjustPathSeparatorsForLinux(pFileNameCopy);
	
	// this is a case-insensitive open operation, so we need to check if
	// the file exists
	struct stat64 statInfo;
	if (::stat64(pszFilename, &statInfo) == -1)
	{
		// doesn't exist - try to correct it for the platform
		AdjustPathCaseForLinux(pFileNameCopy);
		hFile = ::fopen64(pFileNameCopy, strMode.c_str());
	}
	else
	{
		// since it exists we can open it without adjusting the path
		hFile = ::fopen64(pFileNameCopy, strMode.c_str());
	}
	
	// check return value
	if (hFile == NULL)
	{
		return false;
	}

	// set the file handle
	m_hFile = hFile;

	// success
	return true;
}

inline bool CLinux_LTFileBase::GetIsOpenImpl(bool& bIsOpen)
{
	bIsOpen = (m_hFile != NULL);
	
	return true;
}
		
inline bool CLinux_LTFileBase::SeekImpl(uint64 nPos)
{
	LTASSERT(m_hFile != NULL, "invalid file handle");

	// call fseeko64, specifying a seek from the start of the file
	return !::fseeko64(m_hFile, nPos, SEEK_SET);
}

inline bool CLinux_LTFileBase::SeekToEndImpl()
{
	LTASSERT(m_hFile != NULL, "invalid file handle");

	// call fseeko64, specifying a distance of zero from the end
	return !::fseeko64(m_hFile, 0, SEEK_END);
}

inline bool CLinux_LTFileBase::GetPosImpl(uint64& nPos)
{
	LTASSERT(m_hFile != NULL, "invalid file handle");

	// call ftello64 to get the current file position
	if ((nPos = ::ftello64(m_hFile)) == -1)
	{
		return false;
	}

	return true;
}

inline bool CLinux_LTFileBase::GetIsEOFImpl(bool& bIsEOF)
{
	LTASSERT(m_hFile != NULL, "invalid file handle");
	
	// retrieve the current position and file size
	uint64 nPos  = 0;
	uint64 nSize = 0;

	if (!GetPosImpl(nPos) || !GetFileSizeImpl(nSize))
	{
		return false;
	}

	// set outgoing parameter
	if (nPos == nSize)
	{
		bIsEOF = true;
	}
	else
	{
		bIsEOF = false;
	}

	// success
	return true;
}

inline bool CLinux_LTFileBase::GetFileSizeImpl(uint64& nSize)
{
	LTASSERT(m_hFile != NULL, "invalid file handle");

	struct stat64 statInfo;
	if (::fstat64(::fileno(m_hFile), &statInfo) == -1)
	{
		return false;
	}
	
	// set outgoing parameter
	nSize = statInfo.st_size;

	return true;
}

inline bool CLinux_LTFileBase::CloseImpl()
{
	bool bResult = true;

	// close if necessary
	if (m_hFile != NULL)
	{
		if (::fclose(m_hFile) != 0)
		{
			bResult = false;
		}
		m_hFile = NULL;
	}

	return bResult;
}

inline void CLinux_LTFileBase::AdjustPathCaseForLinux(char* pszPath)
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

#endif // __LINUX_LTFILEBASE_H__
