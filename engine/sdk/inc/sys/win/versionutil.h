// ----------------------------------------------------------------------- //
//
// MODULE  : versionutil.h
//
// PURPOSE : Contains version utility functions
//
// CREATED : 11/23/05
//
// (c) 2005 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#pragma once
#ifndef __VERSIONUTIL_H__
#define __VERSIONUTIL_H__

#ifndef __PLATFORM_H__
#	include "platform.h"
#endif

// this class is only supported under Win32
#ifdef PLATFORM_WIN32

#include <windef.h>
#include "ltbasedefs.h"
#include "ltassert.h"

#pragma comment( lib, "version.lib" )

#define CODEPAGE_UNICODE	"040904b0"
#define CODEPAGE_LATIN1		"040904e4"

// a class that wraps version information
class CVersionInfo
{
private:
	uint8*							m_pBuffer;
	uint32							m_dwSize;

	// loads file version information
	void LoadFileVersion( LPCTSTR szFilename )
	{
		DWORD dwHandle;
		m_dwSize = (uint32)GetFileVersionInfoSize( szFilename, (LPDWORD)&dwHandle );
		if( m_dwSize != 0 )
		{
			m_pBuffer = new uint8[m_dwSize];
			if( m_pBuffer )
			{
				if( !GetFileVersionInfo(szFilename, dwHandle, m_dwSize, m_pBuffer) )
				{
					delete[] m_pBuffer;
					m_pBuffer = NULL;
				}
			}
		}
	}

public:
	CVersionInfo( LPCTSTR szFilename ) : 
		m_pBuffer(NULL),
		m_dwSize(0)
	{
		LoadFileVersion( szFilename );
	}
	CVersionInfo( HMODULE hModule ) : 
		m_pBuffer(NULL),
		m_dwSize(0)
	{
		TCHAR szModule[256];
		GetModuleFileName( hModule, szModule, LTARRAYSIZE(szModule) );
		LoadFileVersion( szModule );
	}
	CVersionInfo( const CVersionInfo& rhs ) :
		m_pBuffer(NULL),
		m_dwSize(0)
	{
		operator=(rhs);
	}
	~CVersionInfo()
	{
		if( m_pBuffer )
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
	}
	CVersionInfo& operator=( const CVersionInfo& rhs )
	{
		if( &rhs == this )
			return *this;

		// delete our old buffer
		if( m_pBuffer )
			delete[] m_pBuffer;

		// reset our members
		m_pBuffer	= NULL;
		m_dwSize	= 0;

		// check to see if there is a new buffer we need to copy
		if( rhs.m_pBuffer )
		{
			m_dwSize	= rhs.m_dwSize;
			m_pBuffer	= new uint8[m_dwSize];
			memcpy( m_pBuffer, rhs.m_pBuffer, m_dwSize );
		}

		return *this;
	}

	const uint8*	GetVersionInfoBuffer() const	{ return m_pBuffer; }
	uint32			GetVersionInfoSize() const		{ return m_dwSize; }
};

typedef uint32 (*PFNGETMODULEVERSION)();

class CVersionUtil
{
private:

	static void DisplayVersionMismatchError( HMODULE hModule, const uint32 nVersion1, const uint32 nVersion2 )
	{
		TCHAR szFile1[MAX_PATH];
		GetModuleFileName( GetModuleHandle(NULL), szFile1, LTARRAYSIZE(szFile1) );

		TCHAR szFile2[MAX_PATH];
		GetModuleFileName( hModule, szFile2, LTARRAYSIZE(szFile2) );

		TCHAR szError[512];
		_sntprintf( szError, LTARRAYSIZE(szError), _T("There is a version mismatch between the following two modules. Please synchronize to source control to get the latest files.\n\nFile:   \t%s\nVersion:\t%d\n\nFile:   \t%s\nVersion:\t%d"), szFile1, nVersion1, szFile2, nVersion2 );

		::MessageBox( NULL, szError, _T("DLL Version Mismatch"), MB_OK|MB_ICONERROR );
	}

public:
	// compares the versions of two modules to determine if they are the same
	static bool DoesModuleVersionMatchApplication( const CVersionInfo& verModule )
	{
		return DoesModuleVersionMatch(verModule, CVersionInfo(::GetModuleHandle(NULL)));
	}

	// compares version numbers
	static bool DoesModuleVersionMatch( HMODULE hModule, const uint32 nVersion, const bool bSilent = false )
	{
		PFNGETMODULEVERSION pfnGetModuleVersion = (PFNGETMODULEVERSION)::GetProcAddress( hModule, "GetModuleVersion" );
		if( pfnGetModuleVersion == NULL )
		{
			if( !bSilent )
				DisplayVersionMismatchError( hModule, nVersion, (uint32)-1 );
			return false;
		}

		const uint32 nVersionDLL = pfnGetModuleVersion();
		if( nVersionDLL != nVersion )
		{
			if( !bSilent )
				DisplayVersionMismatchError( hModule, nVersion, nVersionDLL );
			return false;
		}

		return true;
	}

	// compares the versions of two modules to determine if they are the same
	static bool DoesModuleVersionMatch( const CVersionInfo& verModule1, const CVersionInfo& verModule2 )
	{
		TCHAR szVersionInfo1[128];
		if( !ExtractVersionInformation(verModule1, _T("FileVersion"), szVersionInfo1, LTARRAYSIZE(szVersionInfo1)) )
			return false;

		uint32 nProduct1, nSKU1, nBuild1;
		if( !ExtractVersionTokens(szVersionInfo1, &nProduct1, &nSKU1, &nBuild1) )
			return false;

		TCHAR szVersionInfo2[128];
		if( !ExtractVersionInformation(verModule2, _T("FileVersion"), szVersionInfo2, LTARRAYSIZE(szVersionInfo2)) )
			return false;

		uint32 nProduct2, nSKU2, nBuild2;
		if( !ExtractVersionTokens(szVersionInfo2, &nProduct2, &nSKU2, &nBuild2) )
			return false;

		// if all tokens match then so do the versions
		if( (nProduct1 == nProduct2) &&	(nSKU1 == nSKU2) &&	(nBuild1 == nBuild2) )
			return true;

		return false;
	}

	// extracts a version string from the resource of a module
	static bool ExtractVersionInformation( const CVersionInfo& verModule, LPCTSTR szVersionInfo, LPTSTR szBuffer, uint32 nBufferSize )
	{
		LTASSERT( szVersionInfo, "Invalid version information" );
		LTASSERT( szBuffer, "Invalid buffer" );
		if( !szVersionInfo || !szBuffer )
			return false;

		const LPVOID pVersionInfoBuffer = (const LPVOID)verModule.GetVersionInfoBuffer();
		LTASSERT( pVersionInfoBuffer, "Invalid version buffer" );
		if( !pVersionInfoBuffer )
			return false;

		TCHAR szSubBlock[128];
		LTSNPrintF( szSubBlock, LTARRAYSIZE(szSubBlock), _T("\\StringFileInfo\\") _T(CODEPAGE_UNICODE) _T("\\%s"), szVersionInfo );

		TCHAR* pszBlockBuffer = NULL;
		UINT uiSize = (UINT)verModule.GetVersionInfoSize();
		if( VerQueryValue(pVersionInfoBuffer, szSubBlock, (void**)&pszBlockBuffer, &uiSize) == FALSE )
		{
			LTSNPrintF( szSubBlock, LTARRAYSIZE(szSubBlock), _T("\\StringFileInfo\\") _T(CODEPAGE_LATIN1) _T("\\%s"), szVersionInfo );
			uiSize = (UINT)verModule.GetVersionInfoSize();
			VerQueryValue(pVersionInfoBuffer, szSubBlock, (void**)&pszBlockBuffer, &uiSize);
		}

		// if we found a valid string pointer
		if( pszBlockBuffer )
		{
			LTStrCpy( szBuffer, pszBlockBuffer, nBufferSize );
			return true;
		}

		LTERROR( "Version info not found in module!" );

		return false;
	}

	// extracts version tokens from a version string
	static bool ExtractVersionTokens( LPCTSTR szFileVersion, uint32* pnProduct = NULL, uint32* pnSKU = NULL, uint32* pnBuild = NULL, uint32* pnBugFix = NULL )
	{
		LTASSERT( szFileVersion, "Invalid version string" );
		if( !szFileVersion )
			return false;

		int nProduct, nSKU, nBuild, nBugFix;
		_stscanf( szFileVersion, _T("%d.%d.%d.%d"), &nProduct, &nSKU, &nBuild, &nBugFix );

		if( pnProduct )
			*pnProduct = (uint32)nProduct;
		if( pnSKU )
			*pnSKU = (uint32)nSKU;
		if( pnBuild )
			*pnBuild = (uint32)nBuild;
		if( pnBugFix )
			*pnBugFix = (uint32)nBugFix;

		return true;
	}
};

// return the version of the DLL
#define LTEXPORTMODULEVERSION(n) \
	extern "C" \
	{ \
		__declspec(dllexport) uint32 GetModuleVersion() \
		{ \
			return (n); \
		} \
	}

#endif  // PLATFORM_WIN32

#endif  // __VERSIONUTIL_H__
