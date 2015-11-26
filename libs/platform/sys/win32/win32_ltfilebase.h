// *********************************************************************** //
//
// MODULE  : win32_ltfilebase.h
//
// PURPOSE : Base Win32 implementation class for ILTFileRead and ILTFileWrite.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __WIN32_LTFILEBASE_H__
#define __WIN32_LTFILEBASE_H__

#ifndef __LTASSERT_H__
#include "ltassert.h"
#endif

class CWin32_LTFileBase
{
protected:

	CWin32_LTFileBase();
	~CWin32_LTFileBase();

	// access to the underlying file handle
	HANDLE GetFileHandle() const { return m_hFile; }

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

	// Win32 file handle
	HANDLE m_hFile;

};

inline CWin32_LTFileBase::CWin32_LTFileBase()
	: m_hFile(INVALID_HANDLE_VALUE)
{
}

inline CWin32_LTFileBase::~CWin32_LTFileBase()
{
	// close the file if necessary
	CloseImpl();
}

inline bool CWin32_LTFileBase::OpenImpl(const char* pszFilename, EOpenMode eOpenMode)
{
	// close the file handle if it's already open
	CloseImpl();

	// build the arguments for the API call
	DWORD dwAccess		= 0;
	DWORD dwShare		= 0;
	DWORD dwDisposition = 0;

	switch(eOpenMode)
	{
		case eOpenRead:
		{
			dwAccess	  = GENERIC_READ;
			dwShare		  = FILE_SHARE_READ;
			dwDisposition = OPEN_EXISTING;
		}
		break;
		case eOpenWrite:
		{
			dwAccess      = GENERIC_WRITE;
			dwShare		  = FILE_SHARE_READ;
			dwDisposition = CREATE_ALWAYS;
		}
		break;
		case eOpenWriteAppend:
		{
			dwAccess      = GENERIC_WRITE;
			dwShare		  = FILE_SHARE_READ;
			dwDisposition = OPEN_ALWAYS;
		}
		break;
		default:
		{
			LTERROR("invalid open mode");
			return false;
		}
	}

	// call the Win32 CreateFile API to open the file (exclusive)
	HANDLE hFile = ::CreateFileA(pszFilename, dwAccess, dwShare, NULL, dwDisposition, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	
	// check return value
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	// set the file handle
	m_hFile = hFile;

	// success
	return true;
}

inline bool CWin32_LTFileBase::GetIsOpenImpl(bool& bIsOpen)
{
	bIsOpen = (m_hFile != INVALID_HANDLE_VALUE);

	return true;
}

inline bool CWin32_LTFileBase::SeekImpl(uint64 nPos)
{
	LTASSERT(m_hFile != INVALID_HANDLE_VALUE, "invalid file handle");

	// build arguments for the API call
	LARGE_INTEGER liPos;
	liPos.QuadPart = nPos;

	// call SetFilePointerEx, specifying a seek from the start of the file
	return !!::SetFilePointerEx(m_hFile, liPos, NULL, FILE_BEGIN);
}

inline bool CWin32_LTFileBase::SeekToEndImpl()
{
	LTASSERT(m_hFile != INVALID_HANDLE_VALUE, "invalid file handle");

	// build arguments for the API call
	LARGE_INTEGER liZero;
	liZero.QuadPart = 0;

	// call SetFilePointerEx, specifying a distance of zero from the end
	return !!::SetFilePointerEx(m_hFile, liZero, NULL, FILE_END);
}

inline bool CWin32_LTFileBase::GetPosImpl(uint64& nPos)
{
	LTASSERT(m_hFile != INVALID_HANDLE_VALUE, "invalid file handle");

	// build arguments for the API call
	LARGE_INTEGER liZero;
	liZero.QuadPart = 0;
	LARGE_INTEGER liCurrentPos;
	liCurrentPos.QuadPart = 0;

	// call SetFilePointerEx, asking for the current file position
	if (::SetFilePointerEx(m_hFile, liZero, &liCurrentPos, FILE_CURRENT) == FALSE)
	{
		return false;
	}

	// set outgoing parameter
	nPos = liCurrentPos.QuadPart;

	return true;
}

inline bool CWin32_LTFileBase::GetIsEOFImpl(bool& bIsEOF)
{
	LTASSERT(m_hFile != INVALID_HANDLE_VALUE, "invalid file handle");
	
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

inline bool CWin32_LTFileBase::GetFileSizeImpl(uint64& nSize)
{
	LTASSERT(m_hFile != INVALID_HANDLE_VALUE, "invalid file handle");

	// build arguments for the API call
	LARGE_INTEGER liSize;
	liSize.QuadPart = 0;

	// call GetFileSizeEx
	if (::GetFileSizeEx(m_hFile, &liSize) == FALSE)
	{
		return false;
	}

	// set outgoing parameter
	nSize = liSize.QuadPart;

	return true;
}

inline bool CWin32_LTFileBase::CloseImpl()
{
	bool bResult = true;

	// close if necessary
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		bResult = !!::CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	return bResult;
}

#endif // __WIN32_LTFILEBASE_H__
