// ----------------------------------------------------------------------- //
//
// MODULE  : mpstrconv.h
//
// PURPOSE : This header defines helper classes used to convert ANSI
//           strings to Unicode strings.  These classes are based on
//           the classes defined in ATL 7.0, atlconv.h.
//
// CREATED : 12/04/03
//
// (c) 1999-2003 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __MPSTRCONV_H__
#define __MPSTRCONV_H__

/*
 *	CONVERSION REFERENCE
 *
 *  All string conversion classes follow the following scheme:
 *
 *  MP<from>2[C]<to>[EX]
 *
 *  Where 
 *
 *  from	either A (ANSI) or W (Wide Char/Unicode) 
 *          or T (Condition compile, uses W #ifdef UNICODE #else A )
 *  C		const, if using conditional compile a const version 
 *          will just copy the string pointer and not allocate 
 *          stack memory for a new string
 *  to      either A (ANSI) or W (Wide Char/Unicode) 
 *          or T (Condition compile, uses W #ifdef UNICODE #else A )
 *  EX      allows you to specify the size of the stack buffer via 
 *          the first template parameter. if EX is not specified 
 *          STRCONV_BUFFER_LENGTH will be used instead.
 *
 */

/* \Engine\sdk\inc\ */
#include "ltinteger.h"
#include "ltstrutils.h"
#include "ltassert.h"
#include "iltmemory.h"

// you can override this default stack size by setting this define prior 
// to including this header
#ifndef STRCONV_BUFFER_LENGTH
#	define STRCONV_BUFFER_LENGTH 128
#endif

#ifndef CP_ACP
#	define CP_ACP 0
#endif

// you can override the codepage by setting this define prior to 
// including this header
#ifndef MP_CODEPAGE
#	define MP_CODEPAGE CP_ACP
#endif


/*
 *	Wide Character To Wide Character
 */

template< uint32 t_nBufferLength = STRCONV_BUFFER_LENGTH >
class MPW2WEX
{
private:
	wchar_t*						m_pwsz;
	wchar_t							m_wszBuffer[t_nBufferLength];

	void Init( const wchar_t* wsz )
	{
		if( wsz == NULL )
		{
			m_pwsz = NULL;
			return;
		}

		uint32 nLength = LTStrLen( wsz ) + 1;
		if( nLength > t_nBufferLength )
		{
			LT_MEM_TRACK_ALLOC( m_pwsz = new wchar_t[nLength], LT_MEM_TYPE_MISC ); 
			if( m_pwsz == NULL )
			{
				LTASSERT( m_pwsz, "[MPW2WEX::Init] : Out of Memory" );
				return;
			}
		}

		LTStrCpy( m_pwsz, wsz, nLength );
	}

public:
	MPW2WEX( const wchar_t* wsz ) : m_pwsz( m_wszBuffer ) {
		Init( wsz );
	}
	MPW2WEX( const wchar_t* wsz, uint32 nCodePage ) : m_pwsz( m_wszBuffer ) {
		(void)nCodePage;  // code page doesn't matter
		Init( wsz );
	}
	~MPW2WEX() {
		if( m_pwsz != m_wszBuffer )
			delete[] m_pwsz;
	}

	operator wchar_t*() const {
		return m_pwsz;
	}
	operator const wchar_t*() const {
		return (const wchar_t*)m_pwsz;
	}

	const wchar_t* c_str() {
		return m_pwsz;
	}

private:
	MPW2WEX( const MPW2WEX& );
	MPW2WEX& operator=( const MPW2WEX& );
};
typedef MPW2WEX<> MPW2W;


/*
 *	Ansi To Ansi
 */

template< uint32 t_nBufferLength = STRCONV_BUFFER_LENGTH >
class MPA2AEX
{
private:
	char*							m_psz;
	char							m_szBuffer[t_nBufferLength];

	void Init( const char* sz )
	{
		if( sz == NULL )
		{
			m_psz = NULL;
			return;
		}

		uint32 nLength = LTStrLen( sz ) + 1;
		if( nLength > t_nBufferLength )
		{
			LT_MEM_TRACK_ALLOC( m_psz = new char[nLength], LT_MEM_TYPE_MISC ); 
			if( m_psz == NULL )
			{
				LTASSERT( m_psz, "[MPA2AEX::Init] : Out of Memory" );
				return;
			}
		}

		LTStrCpy( m_psz, sz, nLength );
	}

public:
	MPA2AEX( const char* sz ) : m_psz( m_szBuffer ) {
		Init( sz );
	}
	MPA2AEX( const char* sz, uint32 nCodePage ) : m_psz( m_szBuffer ) {
		(void)nCodePage;  // code page doesn't matter
		Init( sz );
	}
	~MPA2AEX() {
		if( m_psz != m_szBuffer )
			delete[] m_psz;
	}

	operator char*() const {
		return m_psz;
	}
	operator const char*() const {
		return (const char*)m_psz;
	}

	const char* c_str() {
		return m_psz;
	}

private:
	MPA2AEX( const MPA2AEX& );
	MPA2AEX& operator=( const MPA2AEX& );
};
typedef MPA2AEX<> MPA2A;


/*
 *	Ansi To Wide Character
 */

template< uint32 t_nBufferLength = STRCONV_BUFFER_LENGTH >
class MPA2WEX
{
private:
	wchar_t*						m_pwsz;
	wchar_t							m_wszBuffer[t_nBufferLength];

	void Init( const char* sz, uint32 nCodePage )
	{
		if( sz == NULL )
		{
			m_pwsz = NULL;
			return;
		}

		uint32 nLength = LTStrLen( sz ) + 1;
		if( nLength > t_nBufferLength )
		{
			LT_MEM_TRACK_ALLOC( m_pwsz = new wchar_t[nLength], LT_MEM_TYPE_MISC ); 
			if( m_pwsz == NULL )
			{
				LTASSERT( m_pwsz, "[MPA2WEX::Init] : Out of Memory" );
				return;
			}
		}

#if defined(PLATFORM_LINUX)
		if( ::mbstowcs(m_pwsz, sz, nLength) == -1)
		{
			LTERROR( "[MPA2WEX::Init] : mbstowcs Failed" );
		}
#else
		if( ::MultiByteToWideChar( nCodePage, 0, sz, nLength, m_pwsz, nLength ) == 0 )
		{
			LTERROR( "[MPA2WEX::Init] : MultiByteToWideChar Failed" );
		}
#endif
	}

public:
	MPA2WEX( const char* sz ) : m_pwsz( m_wszBuffer ) {
		Init( sz, MP_CODEPAGE );
	}
	MPA2WEX( const char* sz, uint32 nCodePage ) : m_pwsz( m_wszBuffer ) {
		Init( sz, nCodePage );
	}
	~MPA2WEX() {
		if( m_pwsz != m_wszBuffer )
			delete[] m_pwsz;
	}

	operator wchar_t*() const {
		return m_pwsz;
	}
	operator const wchar_t*() const {
		return (const wchar_t*)m_pwsz;
	}

	const wchar_t* c_str() {
		return m_pwsz;
	}

private:
	MPA2WEX( const MPA2WEX& );
	MPA2WEX& operator=( const MPA2WEX& );
};
typedef MPA2WEX<> MPA2W;


/*
 *	Wide Character To Ansi
 */

template< uint32 t_nBufferLength = STRCONV_BUFFER_LENGTH >
class MPW2AEX
{
private:
	char*							m_psz;
	char							m_szBuffer[t_nBufferLength];

	void Init( const wchar_t* wsz, uint32 nCodePage )
	{
		if( wsz == NULL )
		{
			m_psz = NULL;
			return;
		}

		uint32 nLength = LTStrLen( wsz ) + 1;
		if( nLength > t_nBufferLength )
		{
			LT_MEM_TRACK_ALLOC( m_psz = new char[nLength], LT_MEM_TYPE_MISC ); 
			if( m_psz == NULL )
			{
				LTASSERT( m_psz, "[MPW2AEX::Init] : Out of Memory" );
				return;
			}
		}

#if defined(PLATFORM_LINUX)
		if (::wcstombs(m_psz, wsz, nLength) == -1)
		{
			LTERROR( "[MPW2AEX::Init] : wcstombs Failed" );
		}
#else
		if( ::WideCharToMultiByte( nCodePage, 0, wsz, nLength, m_psz, nLength, NULL, NULL ) == 0 )
		{
			LTERROR( "[MPW2AEX::Init] : WideCharToMultiByte Failed" );
		}
#endif
	}

public:
	MPW2AEX( const wchar_t* wsz ) : m_psz( m_szBuffer ) {
		Init( wsz, MP_CODEPAGE );
	}
	MPW2AEX( const wchar_t* wsz, uint32 nCodePage ) : m_psz( m_szBuffer ) {
		Init( wsz, nCodePage );
	}
	~MPW2AEX() {
		if( m_psz != m_szBuffer )
			delete[] m_psz;
	}

	operator char*() const {
		return m_psz;
	}
	operator const char*() const {
		return (const char*)m_psz;
	}

	const char* c_str() {
		return m_psz;
	}

private:
	MPW2AEX( const MPW2AEX& );
	MPW2AEX& operator=( const MPW2AEX& );
};
typedef MPW2AEX<> MPW2A;

/*
 *	Const Wide Character To Const Wide Character
 */

template< uint32 t_nBufferLength = STRCONV_BUFFER_LENGTH >
class MPW2CWEX
{
private:
	const wchar_t*					m_pwsz;

public:
	MPW2CWEX( const wchar_t* pwsz ) : m_pwsz( pwsz ) {}
	MPW2CWEX( const wchar_t* pwsz, uint32 nCodePage ) : m_pwsz( pwsz ) {
		(void)nCodePage;
	}
	~MPW2CWEX(){}

	operator const wchar_t*() const {
		return m_pwsz;
	}

	const wchar_t* c_str() {
		return m_pwsz;
	}

private:
	MPW2CWEX( const MPW2CWEX& );
	MPW2CWEX& operator=( const MPW2CWEX& );
};
typedef MPW2CWEX<> MPW2CW;


/*
 *	Const Ansi To Const Ansi
 */

template< uint32 t_nBufferLength = STRCONV_BUFFER_LENGTH >
class MPA2CAEX
{
private:
	const char*						m_psz;

public:
	MPA2CAEX( const char* psz ) : m_psz( psz ) {}
	MPA2CAEX( const char* psz, uint32 nCodePage ) : m_psz( psz ) {
		(void)nCodePage;
	}
	~MPA2CAEX(){}

	operator const char*() const {
		return m_psz;
	}

	const char* c_str() {
		return m_psz;
	}

private:
	MPA2CAEX( const MPA2CAEX& );
	MPA2CAEX& operator=( const MPA2CAEX& );
};
typedef MPA2CAEX<> MPA2CA;


#ifdef UNICODE

	#define MPW2T		MPW2W
	#define MPW2TEX		MPW2WEX
	#define MPW2CT		MPW2CW
	#define MPW2CTEX	MPW2CWEX
	#define MPT2W		MPW2W
	#define MPT2WEX		MPW2WEX
	#define MPT2CW		MPW2CW
	#define MPT2CWEX	MPW2CWEX

	#define MPA2T		MPA2W
	#define MPA2TEX		MPA2WEX
	#define MPA2CT		MPA2W
	#define MPA2CTEX	MPA2WEX
	#define MPT2A		MPW2A
	#define MPT2AEX		MPW2AEX
	#define MPT2CA		MPW2A
	#define MPT2CAEX	MPW2AEX

#else  // !UNICODE

	#define MPW2T		MPW2A
	#define MPW2TEX		MPW2AEX
	#define MPW2CT		MPW2A
	#define MPW2CTEX	MPW2AEX
	#define MPT2W		MPA2W
	#define MPT2WEX		MPA2WEX
	#define MPT2CW		MPA2W
	#define MPT2CWEX	MPA2WEX

	#define MPA2T		MPA2A
	#define MPA2TEX		MPA2AEX
	#define MPA2CT		MPA2CA
	#define MPA2CTEX	MPA2CAEX
	#define MPT2A		MPA2A
	#define MPT2AEX		MPA2AEX
	#define MPT2CA		MPA2CA
	#define MPT2CAEX	MPA2CAEX

#endif  // UNICODE

#endif  // __MPSTRCONV_H__
