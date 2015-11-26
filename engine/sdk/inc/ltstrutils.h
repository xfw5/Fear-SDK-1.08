//----------------------------------------------------------------------------
// LTStrUtils.h
//
// Provides safe versions of common string operations for normal and wide
// character sets
//
//----------------------------------------------------------------------------
#ifndef __LTSTRUTILS_H__
#define __LTSTRUTILS_H__

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

#ifndef __LTASSERT_H__
#include "ltassert.h"
#endif 

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

#ifndef __STDARG_H__
#include <stdarg.h>
#define __STDARG_H__
#endif

#ifndef __STDIO_H__
#include <stdio.h>	// needed for _vsnprintf
#define __STDIO_H__
#endif

#ifndef __STRING_H__
#include <string.h>
#define __STRING_H__
#endif

#ifndef __STDLIB_H__
#include <stdlib.h>
#endif

/*! @author Jeff Cotton
 *  @brief this is an internal macro to create a wchar_t string litteral.
 *
 *  @warning DON'T USE DIRECTLY instead use LT_WCHAR_T( StringLiteral )
 */
#define __LT_WCHAR_T( x ) L ## x

/*! @author Jeff Cotton
 *  @brief this is a macro to create a wchar_t string litteral.
 * 
 *  #define THIS_IS_A_SYMBOL "This is a string litteral"
 *  
 *  const wchar_t * thisIsAVariable1 = __LT_WCHAR_T( "This is a string litteral" ) // fine, but you should really do 
 *  const wchar_t * thisIsAVariable2 = LT_WCHAR_T( "This is a string litteral" ) // this
 *  const wchar_t * thisIsAVariable3 = __LT_WCHAR_T( THIS_IS_A_SYMBOL ) // won't work L is and undefined identifier
 *  const wchar_t * thisIsAVariable4 = LT_WCHAR_T( THIS_IS_A_SYMBOL ) // correct everything is fine
 */
#define LT_WCHAR_T( x )    __LT_WCHAR_T( x )
#define LT_WCHAR_TEXT( x ) __LT_WCHAR_T( x )

/*!
Safe string length
*/
inline uint32 LTStrLen(const char* pszStr1)
{
	if(pszStr1)
		return (uint32)strlen(pszStr1);
	return 0;
}

inline uint32 LTStrLen(const wchar_t* pszStr1)
{
	if(pszStr1)
		return (uint32)wcslen(pszStr1);
	return 0;
}

/*!
Called to determine if the provided string is empty. This will assume that NULL strings are 
empty, and that the provided string, if not null, points to at least one character
*/
inline bool LTStrEmpty(const char* pszStr)
{
	return !pszStr || (pszStr[0] == '\0');
}

inline bool LTStrEmpty(const wchar_t* pszStr)
{
	return !pszStr || (pszStr[0] == (wchar_t)'\0');
}

/*!
Safe string duplication. This insures that all strings are allocated in the same manner
and that it handles all cases, of which some aren't handled by standard strdup. The caller
is responsible for freeing this memory using the array delete operator.
*/
inline char* LTStrDup(const char* pszString)
{
	//handle a null input
	if(!pszString)
		return NULL;

	//allocate the string
	char* pRV = new char[LTStrLen(pszString) + 1];

	//handle out of memory conditions
	if(pRV)
	{
		strcpy(pRV, pszString);
	}
	else
	{
		LTERROR("LTStrDup: Failed to allocate memory for string. Possible non-terminating string");
	}

	return pRV;
}

inline wchar_t* LTStrDup(const wchar_t* pszString)
{
	//handle a null input
	if(!pszString)
		return NULL;

	//allocate the string
	wchar_t* pRV = new wchar_t[LTStrLen(pszString) + 1];

	//handle out of memory conditions
	if(pRV)
	{
		wcscpy(pRV, pszString);
	}
	else
	{
		LTERROR("LTStrDup: Failed to allocate memory for string. Possible non-terminating string");
	}

	return pRV;
}


/*!
Safe string copy (strncpy doesn't always null terminate, but this does).
*/
inline void LTStrCpy(char *pDest, const char *pSrc, uint32 nBufferChars) 
{
    if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTStrCpy: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTStrCpy: Invalid source string provided");
		pSrc = "";
	}

	LTASSERT(nBufferChars > LTStrLen(pSrc), "LTStrCpy : Copy source is truncated");

    strncpy(pDest, pSrc, nBufferChars - 1);
    pDest[nBufferChars - 1] = '\0';
}

inline void LTStrCpy(wchar_t *pDest, const wchar_t *pSrc, uint32 nBufferChars) 
{
    if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTStrCpy: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTStrCpy: Invalid source string provided");
		pSrc = L"";
	}

	LTASSERT(nBufferChars > LTStrLen(pSrc), "LTStrCpy : Copy source is truncated");

    wcsncpy(pDest, pSrc, nBufferChars - 1);
    pDest[nBufferChars - 1] = '\0';
}

/*!
Safe substring copy (strncpy doesn't always null terminate, but this does).
*/
inline void LTSubStrCpy(char* pDest, const char* pSrc, uint32 nBufferChars, uint32 nSrcLen)
{
	if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTSubStrCpy: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTSubStrCpy: Invalid source string provided");
		pSrc = "";
	}

	uint32 nCopyLen = (nBufferChars <= nSrcLen) ? (nBufferChars-1) : nSrcLen;
	LTASSERT(nCopyLen == nSrcLen, "LTSubStrCpy : Copy source is truncated");

	strncpy( pDest, pSrc, nCopyLen );
	pDest[nCopyLen] = '\0';
}

inline void LTSubStrCpy(wchar_t* pDest, const wchar_t* pSrc, uint32 nBufferChars, uint32 nSrcLen)
{
	if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTSubStrCpy: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTSubStrCpy: Invalid source string provided");
		pSrc = L"";
	}

	uint32 nCopyLen = (nBufferChars <= nSrcLen) ? (nBufferChars-1) : nSrcLen;
	LTASSERT(nCopyLen == nSrcLen, "LTSubStrCpy : Copy source is truncated");

	wcsncpy( pDest, pSrc, nCopyLen );
	pDest[nCopyLen] = '\0';
}

/*!
Safe string concatenation (strncat doesn't always null terminate, but this does).
*/
inline void LTStrCat(char *pDest, const char *pSrc, uint32 nBufferChars) 
{
    if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTStrCat: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTStrCat: Invalid source string provided");
		pSrc = "";
	}

	uint32 destLen = LTStrLen(pDest);
	uint32 catLen = nBufferChars - destLen - 1;

	LTASSERT(catLen >= LTStrLen(pSrc), "LTStrCat : Copy source is truncated");

    strncat(pDest, pSrc, catLen);
    pDest[destLen + catLen] = '\0';
}

inline void LTStrCat(wchar_t *pDest, const wchar_t *pSrc, uint32 nBufferChars) 
{
    if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTStrCat: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTStrCat: Invalid source string provided");
		pSrc = L"";
	}

	uint32 destLen = LTStrLen(pDest);
	uint32 catLen = nBufferChars - destLen - 1;

	LTASSERT(catLen >= LTStrLen(pSrc), "LTStrCat : Copy source is truncated");

    wcsncat(pDest, pSrc, catLen);
    pDest[destLen + catLen] = '\0';
}

/*!
Safe substring concatenation (strncat doesn't always terminate, but this does).
*/
inline void LTSubStrCat(char* pDest, const char* pSrc, uint32 nBufferChars, uint32 srcLen)
{
	if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTSubStrCat: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTSubStrCat: Invalid source string provided");
		pSrc = "";
	}

	uint32 destLen = LTStrLen(pDest);
	uint32 catLen = ((nBufferChars - destLen) <= srcLen) ? (nBufferChars-destLen-1) : srcLen;

	LTASSERT(catLen == srcLen, "LTSubStrCat : Copy source is truncated");

	strncat( pDest, pSrc, catLen );
	pDest[destLen + catLen] = '\0';
}

inline void LTSubStrCat(wchar_t* pDest, const wchar_t* pSrc, uint32 nBufferChars, uint32 srcLen)
{
	if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTSubStrCat: Invalid destination buffer provided");
        return;
	}

	if(!pSrc)
	{
		LTERROR("LTSubStrCat: Invalid source string provided");
		pSrc = L"";
	}

	uint32 destLen = LTStrLen(pDest);
	uint32 catLen = ((nBufferChars - destLen) <= srcLen) ? (nBufferChars-destLen-1) : srcLen;

	LTASSERT(catLen == srcLen, "LTSubStrCat : Copy source is truncated");

	wcsncat( pDest, pSrc, catLen );
	pDest[destLen + catLen] = '\0';
}

/*!
Safe string formatting (vsnprintf doesn't always null terminate, but this does).
*/
inline int32 LTVSNPrintF(char *pDest, size_t nBufferChars, const char *pFormat, va_list argptr)
{
	if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTVSNPrintF: Invalid destination buffer provided");
        return -1;
	}

	if(!pFormat)
	{
		LTERROR("LTVSNPrintF: Invalid source string provided");
		pFormat = "";
	}
	
#if defined(PLATFORM_LINUX)
	int32 iReturn = vsnprintf(pDest, nBufferChars, pFormat, argptr);
#else
	int32 iReturn = _vsnprintf(pDest, nBufferChars, pFormat, argptr);
#endif

	pDest[nBufferChars - 1] = '\0';

	return iReturn;
}

inline int32 LTVSNPrintF(wchar_t *pDest, size_t nBufferChars, const wchar_t *pFormat, va_list argptr)
{
	if (!pDest || (nBufferChars == 0))
	{
		LTERROR("LTVSNPrintF: Invalid destination buffer provided");
        return -1;
	}

	if(!pFormat)
	{
		LTERROR("LTVSNPrintF: Invalid source string provided");
		pFormat = L"";
	}

#if defined(PLATFORM_LINUX)
	int32 iReturn = vswprintf(pDest, nBufferChars, pFormat, argptr);
#else
	int32 iReturn = _vsnwprintf(pDest, nBufferChars, pFormat, argptr);
#endif
	
	pDest[nBufferChars - 1] = '\0';

	return iReturn;
}

/*!
Safe string formatting (snprintf doesn't always null terminate, but this does).
*/
inline int32 LTSNPrintF(char *buffer, size_t count, const char *format, ...)
{
	va_list marker;
	va_start(marker, format);
	int32 iReturn = LTVSNPrintF(buffer, count, format, marker);
	va_end(marker);

	return iReturn;
}

inline int32 LTSNPrintF(wchar_t *buffer, size_t count, const wchar_t *format, ...)
{
	va_list marker;
	va_start(marker, format);
	int32 iReturn = LTVSNPrintF(buffer, count, format, marker);
	va_end(marker);

	return iReturn;
}

/*!
Case sensitive string comparison
*/
inline int32 LTStrCmp(const char* pszStr1, const char* pszStr2)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTStrCmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

	return strcmp(pszStr1, pszStr2);
}

inline int32 LTStrCmp(const wchar_t* pszStr1, const wchar_t* pszStr2)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTStrCmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

	return wcscmp(pszStr1, pszStr2);
}

/*!
Case insensitive string comparison
*/
inline int32 LTStrICmp(const char* pszStr1, const char* pszStr2)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTStrICmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

#if defined(PLATFORM_LINUX)
	return strcasecmp(pszStr1, pszStr2);
#else
	return stricmp(pszStr1, pszStr2);
#endif
}

inline int32 LTStrICmp(const wchar_t* pszStr1, const wchar_t* pszStr2)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTStrICmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

#if defined(PLATFORM_LINUX)
	return wcscasecmp(pszStr1, pszStr2);
#else
	return wcsicmp(pszStr1, pszStr2);
#endif
}

/*!
Case sensitive substring comparison
*/
inline int32 LTSubStrCmp(const char* pszStr1, const char* pszStr2, uint32 nCharsToCmp)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTSubStrCmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

	return strncmp(pszStr1, pszStr2, nCharsToCmp);
}

inline int32 LTSubStrCmp(const wchar_t* pszStr1, const wchar_t* pszStr2, uint32 nCharsToCmp)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTSubStrCmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

	return wcsncmp(pszStr1, pszStr2, nCharsToCmp);
}

/*!
Case insensitive substring comparison
*/
inline int32 LTSubStrICmp(const char* pszStr1, const char* pszStr2, uint32 nCharsToCmp)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTSubStrICmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

#if defined(PLATFORM_LINUX)
	return strncasecmp(pszStr1, pszStr2, nCharsToCmp);
#else
	return strnicmp(pszStr1, pszStr2, nCharsToCmp);
#endif
}

inline int32 LTSubStrICmp(const wchar_t* pszStr1, const wchar_t* pszStr2, uint32 nCharsToCmp)
{
	LTASSERT(pszStr1 && pszStr2, "Error: Invalid input to LTSubStrICmp");
	if(!pszStr1)
		return (pszStr2) ? -1 : 0;
	if(!pszStr2)
		return 1;

#if defined(PLATFORM_LINUX)
	return wcsncasecmp(pszStr1, pszStr2, nCharsToCmp);
#else
	return wcsnicmp(pszStr1, pszStr2, nCharsToCmp);
#endif
}

/*!
Case sensitive string equality comparison
*/
inline bool LTStrEquals(const char* pszStr1, const char* pszStr2)
{
	return (LTStrCmp(pszStr1, pszStr2) == 0);
}

inline bool LTStrEquals(const wchar_t* pszStr1, const wchar_t* pszStr2)
{
	return (LTStrCmp(pszStr1, pszStr2) == 0);
}

/*!
Case insensitive string equality comparison
*/
inline bool LTStrIEquals(const char* pszStr1, const char* pszStr2)
{
	return (LTStrICmp(pszStr1, pszStr2) == 0);
}

inline bool LTStrIEquals(const wchar_t* pszStr1, const wchar_t* pszStr2)
{
	return (LTStrICmp(pszStr1, pszStr2) == 0);
}

/*!
Case sensitive substring equality comparison
*/
inline bool LTSubStrEquals(const char* pszStr1, const char* pszStr2, uint32 nNumCharsToCmp)
{
	return (LTSubStrCmp(pszStr1, pszStr2, nNumCharsToCmp) == 0);
}

inline bool LTSubStrEquals(const wchar_t* pszStr1, const wchar_t* pszStr2, uint32 nNumCharsToCmp)
{
	return (LTSubStrCmp(pszStr1, pszStr2, nNumCharsToCmp) == 0);
}

/*!
Case insensitive substring equality comparison
*/
inline bool LTSubStrIEquals(const char* pszStr1, const char* pszStr2, uint32 nNumCharsToCmp)
{
	return (LTSubStrICmp(pszStr1, pszStr2, nNumCharsToCmp) == 0);
}

inline bool LTSubStrIEquals(const wchar_t* pszStr1, const wchar_t* pszStr2, uint32 nNumCharsToCmp)
{
	return (LTSubStrICmp(pszStr1, pszStr2, nNumCharsToCmp) == 0);
}

/*!
String to uppercase
*/
inline char* LTStrUpr(char* pszStr)
{
	char* pszStrIndex = pszStr;

	while (*pszStrIndex)
	{
		*pszStrIndex = (char)toupper(*pszStrIndex);
		++pszStrIndex;
	}
	
	return pszStr;
}

/*!
String clearing
*/

//this handles clearing the provided string in the most efficient manner possible. This assumes
//that the provided string is valid and is at least one character large
inline void LTStrClear(char* pszStr)
{
	pszStr[0] = '\0';
}

inline void LTStrClear(wchar_t* pszStr)
{
	pszStr[0] = (wchar_t)'\0';

}

/*!
String to long
*/

inline long LTStrToLong(const char* pszStr)
{
	return strtol(pszStr, NULL, 0);
}

inline long LTStrToLong(const wchar_t* pszStr)
{
	return wcstol(pszStr, NULL, 0);
}

inline double LTStrToDouble(const char* pszStr)
{
	return strtod(pszStr, NULL);
}

inline double LTStrToDouble(const wchar_t* pszStr)
{
	return wcstod(pszStr, NULL);
}

#endif
