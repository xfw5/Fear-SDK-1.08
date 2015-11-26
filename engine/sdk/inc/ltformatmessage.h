// ----------------------------------------------------------------------- //
//
// MODULE  : ltformatmessage.h
//
// PURPOSE : Declares the CLTFormatMessageT class.  
//
// CREATED : 08/27/04
//
// (c) 1999-2004 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __LTFORMATMESSAGE_H__
#define __LTFORMATMESSAGE_H__

/*
 *	FORMAT MESSAGE REFERENCE
 *
 *  For most purposes use the default LTFormatMessage function.  The default 
 *  function supports a maximum of 16 fields in the format string.  If there 
 *  are more than 16 then this command will fail.
 *
 *  If you know exactly the amount of fields in the format string then you can 
 *  specify the count using the template function LTFormatMessageT.
 *
 *  Example:
 *
 *  LTFormatMessage( "Test String %1!s!, Test Number: %2!d!, More String: %1!s!", "wow", 10 );
 *
 *  Field Count	= 3
 *
 *  LTFormatMessageT<3>( "Test String %1!s!, Test Number: %2!d!, More String: %1!s!", "wow", 10 );
 *
 */

/* \Engine\sdk\inc\ */
#include "ltinteger.h"
#include "ltstrutils.h"
#include "sys/win/mpstrconv.h"

#if defined(PLATFORM_LINUX)
#include <wctype.h>
#endif

// you can override this default field count by setting this define prior 
// to including this header
#ifndef LTFORMATMESSAGE_FIELD_COUNT
#	define LTFORMATMESSAGE_FIELD_COUNT 16
#endif

template<typename T, uint32 t_nFieldCount = LTFORMATMESSAGE_FIELD_COUNT>
class CLTFormatMessageT
{
private:
	enum fmtype
	{
		fmtype_unknown,
		fmtype_long_int,
		fmtype_unsigned_long_int,
		fmtype_wchar_t,
		fmtype_wchar_t_ptr,
		fmtype_short_int,
		fmtype_unsigned_short_int,
		fmtype_char,
		fmtype_char_ptr,
		fmtype_int64,
		fmtype_int32,
		fmtype_int,
		fmtype_double,
		fmtype_int_ptr,
		fmtype_void_ptr
	};

	struct FormatMessageArgument
	{
		// offset from the begining of the format string to this field (the '%' char)
		uint16	m_iFieldOffset;
		// offset from the begining of the format string to the first exclamation mark
		uint16	m_iExclamationOffset;
		// size of the format string in between the two exclamation marks
		uint16	m_iFormatLength;

		// the index of the argument in the source va_list
		uint16	m_idxArg;

		// union of all supported types
		union{
			long int			m_long_int;
			unsigned long int	m_unsigned_long_int;
			wchar_t				m_wchar_t;
			wchar_t*			m_wchar_t_ptr;
			short int			m_short_int;
			unsigned short int	m_unsigned_short_int;
			char				m_char;
			char*				m_char_ptr;
			int64				m_int64;
			int32				m_int32;
			int					m_int;
			double				m_double;
			int*				m_int_ptr;
			void*				m_void_ptr;
		};

		// the type of the argument
		fmtype  m_Type;
	};

	// returns the type of a field, some fields differ depending on 
	// whether the string is a char or wchar_t pointer
	inline fmtype	FieldTypeT( const T *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nFieldOffset );
	inline fmtype	FieldType( const wchar_t *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nFieldOffset );
	inline fmtype	FieldType( const char *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nFieldOffset );

	// sets the ANSI error string in the buffer
	inline void		SetErrorMessage( char* pDest, size_t nBufferChars, const char* szError );
	inline void		SetErrorMessage( wchar_t* pDest, size_t nBufferChars, const char* szError );

	// overloaded string-to-int function
	inline int		StringToInt( const char* sz );
	inline int		StringToInt( const wchar_t* wsz );

	// returns the index of the field
	inline int32	FieldIndex( const T *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nExclamationOffset );

public:
	// public format message calls
	inline bool		FormatMessage( T *pDest, size_t nBufferChars, const T *pFormat, va_list argptr );
	inline bool		FormatMessage( T *pDest, size_t nBufferChars, const T *pFormat, ... );
};

/*
 *	INLINES
 */

template<typename T, uint32 t_nFieldCount>
typename CLTFormatMessageT<T, t_nFieldCount>::fmtype CLTFormatMessageT<T, t_nFieldCount>::FieldTypeT( const T *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nFieldOffset )
{
	nFieldOffset = nOffset;
	switch( pFormat[nOffset] )
	{
	case 'l':
		if( nOffset + 1 >= nFormatLength )
			break;
		nFieldOffset = nOffset + 1;
		switch( pFormat[nOffset + 1] )
		{
		case 'd':
		case 'i':
		case 'o':
		case 'x':
		case 'X':
			return fmtype_long_int;
		
		case 'u':
			return fmtype_unsigned_long_int;

		case 'C':
		case 'c':
			return fmtype_wchar_t;

		case 'S':
		case 's':
			return fmtype_wchar_t_ptr;
		}
		break;

	case 'h':
		if( nOffset + 1 >= nFormatLength )
			break;
		nFieldOffset = nOffset + 1;
		switch( pFormat[nOffset + 1] )
		{
		case 'd':
		case 'i':
		case 'o':
		case 'x':
		case 'X':
			return fmtype_short_int;

		case 'u':
			return fmtype_unsigned_short_int;

		case 'C':
		case 'c':
			return fmtype_char;

		case 'S':
		case 's':
			return fmtype_char_ptr;
		}
		break;

	case 'I':
		{
			// I64 or I32
			if( nOffset + 3 >= nFormatLength )
				break;

			fmtype size;
			if( (pFormat[nOffset + 1] == '6') && (pFormat[nOffset + 2] == '4') )
				size = fmtype_int64;
			else if( (pFormat[nOffset + 1] == '3') && (pFormat[nOffset + 2] == '2') )
				size = fmtype_int32;
			else
				break;

			nFieldOffset = nOffset + 3;
			switch( pFormat[nOffset + 3] )
			{
			case 'd':
			case 'i':
			case 'o':
			case 'u':
			case 'x':
			case 'X':
				return size;
			}
		}
		break;

	case 'd':
	case 'i':
	case 'o':
	case 'x':
	case 'X':
	case 'u':
		return fmtype_int;

	case 'e':
	case 'E':
	case 'f':
	case 'g':
	case 'G':
		return fmtype_double;

	case 'n':
		return fmtype_int_ptr;

	case 'p':
		return fmtype_void_ptr;
	}

	return fmtype_unknown;
}

template<typename T, uint32 t_nFieldCount>
typename CLTFormatMessageT<T, t_nFieldCount>::fmtype CLTFormatMessageT<T, t_nFieldCount>::FieldType( const wchar_t *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nFieldOffset )
{
	// find the next alpha character
	// this should skip all of the flag fields and the exclamation mark
	while( !iswalpha(pFormat[nOffset]) )
	{
		if( nOffset + 1 >= nFormatLength )
			return fmtype_unknown;

		++nOffset;
	}

	nFieldOffset = nOffset;
	switch( pFormat[nOffset] )
	{
	case 'C':
		return fmtype_char;

	case 'c':
		return fmtype_wchar_t;

	case 'S':
		return fmtype_char_ptr;

	case 's':
		return fmtype_wchar_t_ptr;
	}

	return FieldTypeT(pFormat, nOffset, nFormatLength, nFieldOffset);
}

template<typename T, uint32 t_nFieldCount>
typename CLTFormatMessageT<T, t_nFieldCount>::fmtype CLTFormatMessageT<T, t_nFieldCount>::FieldType( const char *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nFieldOffset )
{
	// find the next alpha character
	// this should skip all of the flag fields and the exclamation mark
	while( !isalpha(pFormat[nOffset]) )
	{
		if( nOffset + 1 >= nFormatLength )
			return fmtype_unknown;

		++nOffset;
	}

	nFieldOffset = nOffset;
	switch( pFormat[nOffset] )
	{
	case 'c':
		return fmtype_char;

	case 'C':
		return fmtype_wchar_t;

	case 's':
		return fmtype_char_ptr;

	case 'S':
		return fmtype_wchar_t_ptr;
	}

	return FieldTypeT(pFormat, nOffset, nFormatLength, nFieldOffset);
}

// sets the error message
template<typename T, uint32 t_nFieldCount>
void CLTFormatMessageT<T, t_nFieldCount>::SetErrorMessage( char* pDest, size_t nBufferChars, const char* szError ){
	LTStrCpy( pDest, szError, (uint32)nBufferChars );
}

template<typename T, uint32 t_nFieldCount>
void CLTFormatMessageT<T, t_nFieldCount>::SetErrorMessage( wchar_t* pDest, size_t nBufferChars, const char* szError ){
	LTStrCpy( pDest, MPA2W(szError).c_str(), (uint32)nBufferChars );
}

// try to get the index number for this field
template<typename T, uint32 t_nFieldCount>
int32 CLTFormatMessageT<T, t_nFieldCount>::FieldIndex( const T *pFormat, uint32 nOffset, uint32 nFormatLength, uint32& nExclamationOffset )
{
	// scratch memory
	T szScratch[16];

	uint32 nStart = nOffset;

	// find all numeric characters
	while( (pFormat[nOffset] >= '0') && (pFormat[nOffset] <= '9') )
	{
		if( nOffset + 1 >= nFormatLength )
			break;

		++nOffset;
	}

	// make sure the digit is valid
	if( nOffset - nStart == 0 )
		return -1;

	// make sure the index is followed by an exclamation
	if( pFormat[nOffset] != '!' )
		return -1;

	nExclamationOffset = nOffset;

	LTSubStrCpy( szScratch, pFormat + nStart, LTARRAYSIZE(szScratch), nOffset - nStart );

	return LTStrToLong( szScratch );
}

template<typename T, uint32 t_nFieldCount>
bool CLTFormatMessageT<T, t_nFieldCount>::FormatMessage( T *pDest, size_t nBufferChars, const T *pFormat, va_list argptr )
{
	LTASSERT( pDest, "CLTFormatMessageT::FormatMessage" );
	LTASSERT( pFormat, "CLTFormatMessageT::FormatMessage" );
	if( (pDest == NULL) || (pFormat == NULL) )
		return false;

	uint32 nArgCount = 0;

	uint32 nFormatLength = LTStrLen( pFormat );

	FormatMessageArgument pArgs[t_nFieldCount];

	// locate all fields
	uint32 nSrc;
	for(nSrc=0;nSrc<nFormatLength;++nSrc)
	{
		switch( pFormat[nSrc] )
		{
		case '%':
			if( nSrc + 1 >= nFormatLength )
			{
				LTERROR( "Failed to extract field index." );
				SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Failed to extract field index]" );
				return false;
			}
	
			switch( pFormat[nSrc+1] )
			{
			case '%':
				// add a printf with no arguments
				// this is the easiest way to do this w/o excessive special cases

				// check if we can add more arguments
				if( nArgCount + 1 >= t_nFieldCount )
				{
					// reached the limit of field argument
					LTASSERT_PARAM1( nArgCount + 1 < t_nFieldCount, "Exceeded field count limit of '%d'!", t_nFieldCount );
					SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Exceeded field count limit]" );
					return false;
				}

				// save the type
				pArgs[nArgCount].m_iFieldOffset			= (uint16)nSrc;
				pArgs[nArgCount].m_iExclamationOffset	= (uint16)nSrc;
				pArgs[nArgCount].m_iFormatLength		= 1;
				pArgs[nArgCount].m_idxArg				= 0;
				pArgs[nArgCount].m_Type					= fmtype_unknown;
				++nArgCount;
				++nSrc;
				break;

			case '0':
				goto PARSECOMPLETE;
				break;

			default:
				{
					uint32 nExclamationOffset;
					int32 idxArg = FieldIndex( pFormat, nSrc + 1, nFormatLength, nExclamationOffset );
					if( idxArg == -1 )
					{
						LTERROR( "Failed to extract field index." );
						SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Failed to extract field index]" );
						return false;
					}

					if( nExclamationOffset + 1 >= nFormatLength )
					{
						LTERROR( "Failed to locate open exclamation mark." );
						SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Failed to locate open exclamation mark]" );
						return false;
					}

					uint32 nFieldOffset;
					fmtype type = FieldType( pFormat, nExclamationOffset + 1, nFormatLength, nFieldOffset );
					if( type == fmtype_unknown )
					{
						// invalid field specification
						LTERROR( "Failed to compute type of field" );
						SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Invalid field type]" );
						return false;
					}

					if( (nFieldOffset + 1 >= nFormatLength) || (pFormat[nFieldOffset + 1] != '!') )
					{
						// invalid specification
						LTERROR( "Failed to locate close exclamation mark" );
						SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Failed to locate close exclamation mark]" );
						return false;
					}

					// check if we can add more arguments
					if( nArgCount + 1 >= t_nFieldCount )
					{
						// reached the limit of field argument
						LTASSERT_PARAM1( nArgCount + 1 < t_nFieldCount, "Exceeded field count limit of '%d'!", t_nFieldCount );
						SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Exceeded field count limit]" );
						return false;
					}

					// save the type
					pArgs[nArgCount].m_iFieldOffset			= (uint16)nSrc;
					pArgs[nArgCount].m_iExclamationOffset	= (uint16)nExclamationOffset + 1;
					pArgs[nArgCount].m_iFormatLength		= (uint16)(nFieldOffset - nExclamationOffset);
					pArgs[nArgCount].m_idxArg				= (uint16)idxArg;
					pArgs[nArgCount].m_Type					= type;
					++nArgCount;

					nSrc = nFieldOffset + 1;
				}
			}
			break;
		}
	}

PARSECOMPLETE:
	// the end of the source string
	uint32 nSourceEnd = nSrc;
	
	// create a map
	uint16 pIndices[t_nFieldCount];
	memset( pIndices, -1, sizeof(pIndices) );

	uint16 nLargestIndex = 0;
	for(uint16 nArg=0;nArg<nArgCount;++nArg)
	{
		if( pArgs[nArg].m_idxArg == 0 )
			continue;

		nLargestIndex = LTMAX(nLargestIndex, pArgs[nArg].m_idxArg);
		pIndices[pArgs[nArg].m_idxArg-1] = nArg;
	}

	// extract the arguments
	for(uint16 nArg=0;nArg<nLargestIndex;++nArg)
	{
		// make sure that it is valid
		if( pIndices[nArg] == (uint16)-1 )
		{
			// unknown size of argument
			LTASSERT_PARAM1( pIndices[nArg] != (uint16)-1, "Unknown size of argument '%d'.  All fields must be used in the string.", nArg );
			SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Unknown size of argument]" );
			return false;
		}

		switch( pArgs[pIndices[nArg]].m_Type )
		{
		case fmtype_unknown:
			// printf with no arguments
			break;
		case fmtype_long_int:
			pArgs[pIndices[nArg]].m_long_int = va_arg( argptr, long int );
			break;
		case fmtype_unsigned_long_int:
			pArgs[pIndices[nArg]].m_unsigned_long_int = va_arg( argptr, unsigned long int );
			break;
		case fmtype_wchar_t:
#if defined(PLATFORM_LINUX)
			pArgs[pIndices[nArg]].m_wchar_t = va_arg( argptr, int);
#else
			pArgs[pIndices[nArg]].m_wchar_t = va_arg( argptr, wchar_t );
#endif
			break;
		case fmtype_wchar_t_ptr:
			pArgs[pIndices[nArg]].m_wchar_t_ptr = va_arg( argptr, wchar_t* );
			break;
		case fmtype_short_int:
#if defined(PLATFORM_LINUX)
			pArgs[pIndices[nArg]].m_short_int = va_arg( argptr, int);
#else
			pArgs[pIndices[nArg]].m_short_int = va_arg( argptr, short int );
#endif
			break;
		case fmtype_unsigned_short_int:
#if defined(PLATFORM_LINUX)
			pArgs[pIndices[nArg]].m_unsigned_short_int = va_arg( argptr, int );
#else
			pArgs[pIndices[nArg]].m_unsigned_short_int = va_arg( argptr, unsigned short int );
#endif
			break;
		case fmtype_char:
#if defined(PLATFORM_LINUX)
			pArgs[pIndices[nArg]].m_char = va_arg( argptr, int );
#else
			pArgs[pIndices[nArg]].m_char = va_arg( argptr, char );
#endif
			break;
		case fmtype_char_ptr:
			pArgs[pIndices[nArg]].m_char_ptr = va_arg( argptr, char* );
			break;
		case fmtype_int64:
			pArgs[pIndices[nArg]].m_int64 = va_arg( argptr, int64 );
			break;
		case fmtype_int32:
			pArgs[pIndices[nArg]].m_int32 = va_arg( argptr, int32 );
			break;
		case fmtype_int:
			pArgs[pIndices[nArg]].m_int = va_arg( argptr, int );
			break;
		case fmtype_double:
			pArgs[pIndices[nArg]].m_double = va_arg( argptr, double );
			break;
		case fmtype_int_ptr:
			pArgs[pIndices[nArg]].m_int_ptr = va_arg( argptr, int* );
			break;
		case fmtype_void_ptr:
			pArgs[pIndices[nArg]].m_void_ptr = va_arg( argptr, void* );
			break;

		default:
			// this should never hit
			LTERROR( "[LTFormatMessageT Error - Unknown argument type]" );
			SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Unknown argument type]" );
			return false;
		}
	}

	// scratch memory
	T szScratch[16];
	szScratch[0] = '%';

	// create the final string
	uint32 nDest = 0;
	nSrc = 0;
	for(uint32 nArg=0;nArg<nArgCount;++nArg)
	{
		int32 nLength = pArgs[nArg].m_iFieldOffset - nSrc;
		if( nLength < 0 )
			break;

		// copy the portion before the field
		LTSubStrCpy( pDest + nDest, pFormat + nSrc, (uint32)nBufferChars - nDest, nLength );

		nSrc += nLength;
		nDest += nLength;

		if( nDest >= nBufferChars )
			break;

		// create the new format string
		LTSubStrCpy( szScratch + 1, pFormat + pArgs[nArg].m_iExclamationOffset, LTARRAYSIZE(szScratch) - 1, pArgs[nArg].m_iFormatLength );

		int32 nPrintFLength = -1;
		switch( pArgs[nArg].m_Type )
		{
		case fmtype_unknown:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch );
			break;
		case fmtype_long_int:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_long_int );
			break;
		case fmtype_unsigned_long_int:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_unsigned_long_int );
			break;
		case fmtype_wchar_t:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_wchar_t );
			break;
		case fmtype_wchar_t_ptr:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_wchar_t_ptr );
			break;
		case fmtype_short_int:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_short_int );
			break;
		case fmtype_unsigned_short_int:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_unsigned_short_int );
			break;
		case fmtype_char:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_char );
			break;
		case fmtype_char_ptr:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_char_ptr );
			break;
		case fmtype_int64:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_int64 );
			break;
		case fmtype_int32:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_int32 );
			break;
		case fmtype_int:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_int );
			break;
		case fmtype_double:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_double );
			break;
		case fmtype_int_ptr:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_int_ptr );
			break;
		case fmtype_void_ptr:
			nPrintFLength = LTSNPrintF( pDest + nDest, nBufferChars - nDest, szScratch, pArgs[nArg].m_void_ptr );
			break;
		}

		// check for error
		if( nPrintFLength == -1 )
		{
			LTERROR( "LTSNPrintF failed - Invalid format specification or buffer too small." );
			SetErrorMessage( pDest, nBufferChars, "[LTFormatMessageT Error - Invalid format specification or buffer too small.]" );
			return false;
		}

		nDest += nPrintFLength;
		if( nDest >= nBufferChars )
			break;

		nSrc = pArgs[nArg].m_iExclamationOffset + pArgs[nArg].m_iFormatLength + 1;
	}

	if( nSourceEnd > nSrc )
	{
		// copy the end of the string
		LTSubStrCpy( pDest + nDest, pFormat + nSrc, (uint32)nBufferChars - nDest, nSourceEnd - nSrc );
	}
	else
	{
		// make sure it's null terminated
		pDest[LTMIN((uint32)nBufferChars - 1, nDest)] = '\0';
	}

	return true;
}

template<typename T, uint32 t_nFieldCount>
bool CLTFormatMessageT<T, t_nFieldCount>::FormatMessage( T *pDest, size_t nBufferChars, const T *pFormat, ... )
{
	// extract the source argument list
	va_list marker;
	va_start( marker, pFormat );

	bool bResult = FormatMessage( pDest, nBufferChars, pFormat, marker );

	va_end( marker );

	return bResult;
}

template<uint32 t_nFieldCount>
inline bool LTFormatMessageT( char *pDest, size_t nBufferChars, const char *pFormat, ... )
{
	CLTFormatMessageT<char, t_nFieldCount> fmt;

	// extract the source argument list
	va_list marker;
	va_start( marker, pFormat );

	bool bResult = fmt.FormatMessage( pDest, nBufferChars, pFormat, marker );

	va_end( marker );

	return bResult;
}

template<uint32 t_nFieldCount>
inline bool LTFormatMessageT( wchar_t *pDest, size_t nBufferChars, const wchar_t *pFormat, ... )
{
	CLTFormatMessageT<wchar_t, t_nFieldCount> fmt;

	// extract the source argument list
	va_list marker;
	va_start( marker, pFormat );

	bool bResult = fmt.FormatMessage( pDest, nBufferChars, pFormat, marker );

	va_end( marker );

	return bResult;
}

template<uint32 t_nFieldCount>
inline bool LTFormatMessageT( char *pDest, size_t nBufferChars, const char *pFormat, va_list argptr )
{
	CLTFormatMessageT<char, t_nFieldCount> fmt;
	return fmt.FormatMessage( pDest, nBufferChars, pFormat, argptr );
}

template<uint32 t_nFieldCount>
inline bool LTFormatMessageT( wchar_t *pDest, size_t nBufferChars, const wchar_t *pFormat, va_list argptr )
{
	CLTFormatMessageT<wchar_t, t_nFieldCount> fmt;
	return fmt.FormatMessage( pDest, nBufferChars, pFormat, argptr );
}

#define LTFormatMessage LTFormatMessageT<LTFORMATMESSAGE_FIELD_COUNT>

#endif  // __LTFORMATMESSAGE_H__
