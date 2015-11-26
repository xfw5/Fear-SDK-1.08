// *********************************************************************** //
//
// MODULE  : ltassert.h
//
// PURPOSE : Defines the assertion interface
//
// CREATED : 10/15/03
//
// (c) 2002 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTASSERT_H__
#define __LTASSERT_H__

//this define is used to control whether or not performance monitoring should be used. By
//setting this define in the client system, all performance monitoring calls will attempt
//to be replaced with as simple of functionality as possible so as not to influence performance,
//memory, or code size
#ifdef _FINAL
#define DISABLE_ASSERTS
#endif

#ifndef DISABLE_ASSERTS

//for the abort function
#include <stdlib.h>

#ifndef __PLATFORM_H__
#	include "platform.h"
#endif

//define debug break which can be used to trigger a debug breakpoint. This is defined
//as a macro so that the debugger will break into the actual location instead of inside
//another function
#if defined( PLATFORM_XENON )
#	include <xtl.h>
#	define LTDEBUGBREAK() DebugBreak()
#elif defined(PLATFORM_LINUX)
#	define LTDEBUGBREAK() asm("int3")
#else
#	define LTDEBUGBREAK() __asm { int 3 }
#endif

#define LT_ASSERT_TEXT_SIZE 1024

// conditionally set the display state of the asserts
#ifdef _DEBUG
#	define LT_DISPLAY_ASSERT		true
#else
#	define LT_DISPLAY_ASSERT		false
#endif

// *********************************************************************** //
// Assert methods

//remap VERIFY to go through our library
#undef VERIFY
#define VERIFY( exp )				LTASSERT( exp, "" )

//remap the standard asserts to our LTASSERT
#undef assert
#define assert( exp )				LTASSERT( exp, "" )

#undef ASSERT
#define ASSERT( exp )				LTASSERT( exp, "" )


// helper macro for handling assert result
#define HANDLE_LTASSERT_RESULT(result) \
	switch (result) \
	{ \
	case LT_ASSERT_RESULT_DEBUG:		LTDEBUGBREAK(); break; \
	case LT_ASSERT_RESULT_ABORT:		abort(); break; \
	case LT_ASSERT_RESULT_IGNORE:		bIgnoreAlways = true; break; \
	case LT_ASSERT_RESULT_CONTINUE:		break; \
	default: break; \
	} 

#define STRING_LTERROR " [Forced Assert]"

// using LTASSERT(0, msg) will cause warning C4127: conditional expression is constant
#define LTERROR( msg ) \
{ \
	static bool bIgnoreAlways = false; \
    if( !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( STRING_LTERROR, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg )); } \
} 

#define LTERROR_PARAM1( msg, p1 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( STRING_LTERROR, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1 )); } \
} 

#define LTERROR_PARAM2( msg, p1, p2 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( STRING_LTERROR, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2 )); } \
} 

#define LTERROR_PARAM3( msg, p1, p2, p3 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( STRING_LTERROR, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2, p3 )); } \
} 

#define LTERROR_PARAM4( msg, p1, p2, p3, p4 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( STRING_LTERROR, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2, p3, p4 )); } \
} 

#define LTERROR_PARAM5( msg, p1, p2, p3, p4, p5 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( STRING_LTERROR, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2, p3, p4, p5)); } \
} 

#define LTASSERT( exp, msg ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !( exp ) && !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( #exp, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg )); } \
}

#define LTASSERT_PARAM1( exp, msg, p1 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !( exp ) && !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( #exp, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1 )); } \
} 

#define LTASSERT_PARAM2( exp, msg, p1, p2 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !( exp ) && !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( #exp, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2 )); } \
} 

#define LTASSERT_PARAM3( exp, msg, p1, p2, p3 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !( exp ) && !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( #exp, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2, p3 )); } \
} 

#define LTASSERT_PARAM4( exp, msg, p1, p2, p3, p4 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !( exp ) && !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( #exp, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2, p3, p4 )); } \
} 

#define LTASSERT_PARAM5( exp, msg, p1, p2, p3, p4, p5 ) \
{ \
	static bool bIgnoreAlways = false; \
	if( !( exp ) && !bIgnoreAlways ) \
	{ HANDLE_LTASSERT_RESULT(LTAssert::RunAssert( #exp, __FILE__, __LINE__, __FUNCTION__, LT_DISPLAY_ASSERT, msg, p1, p2, p3, p4, p5)); } \
} 

// *********************************************************************** //
// Forward declarations

#define	LT_ASSERT_RESULT_DEBUG					1100
#define	LT_ASSERT_RESULT_ABORT					1101
#define	LT_ASSERT_RESULT_IGNORE					1102
#define	LT_ASSERT_RESULT_CONTINUE				1103

typedef void (*PFNASSERTCALLBACK)( const char* sAssert, const char* sFilename, int nLine, const char* sFunction, const char* sAssertText, const char* sStackDump, int iResult, void* pUserData );

class LTAssert
{
	public:
		enum eDisplay
		{
			// never display asserts, regardless of the build configuration
			eDisplay_Never,
			// always display asserts, regardless of the build configuration 
			eDisplay_Always,
			// use the current build configuration to determine the state
			// #ifdef _DEBUG then Always, else Never
			eDisplay_Default
		};


		// Assert execute routines
		static int RunAssert( const char* sAssert, const char* sFilename, int nLine, const char* sFunction, bool bShowAssert, const char* sFmt, ... );

		// sets if asserts are to be displayed
		static void					SetDisplayAssert( eDisplay display );
		static LTAssert::eDisplay	GetDisplayAssert();

		// set any global data
		static void SetGlobalData( void* pData );

		// registers an assert callback function
		static void RegisterCallback( PFNASSERTCALLBACK pfnCallback, void* pUserData );
		// unregisters an assert callback function
		static void UnregisterCallback( PFNASSERTCALLBACK pfnCallback, void* pUserData );

		// registers the custom crash dialog
		static void RegisterUnhandledExceptionHandler( unsigned int nDumpFlags, const char* pszEmail );
		// sets the default comment for a crash
		static void SetDefaultUnhandledExceptionComment( const char* pszComment );

	private:

		// Don't allow construction...
		LTAssert();
		LTAssert( const LTAssert& a );
};

enum LTMiniDumpFlag
{
	LTMiniDumpFlagNormal						= 0x0000, 
	LTMiniDumpFlagWithDataSegs					= 0x0001, 
	LTMiniDumpFlagWithFullMemory				= 0x0002, 
	LTMiniDumpFlagWithHandleData				= 0x0004, 
	LTMiniDumpFlagFilterMemory					= 0x0008, 
	LTMiniDumpFlagScanMemory					= 0x0010, 
	LTMiniDumpFlagWithUnloadedModules			= 0x0020, 
	LTMiniDumpFlagWithIndirectlyReferencedMemory = 0x0040, 
	LTMiniDumpFlagFilterModulePaths				= 0x0080, 
	LTMiniDumpFlagWithProcessThreadData			= 0x0100, 
	LTMiniDumpFlagWithPrivateReadWriteMemory	= 0x0200
};

#if defined( PLATFORM_WIN32 )

struct SRegisterUnhandledExceptionHandler
{
	SRegisterUnhandledExceptionHandler( unsigned int nDumpFlags, const char* pszEmail )
	{
		LTAssert::RegisterUnhandledExceptionHandler( nDumpFlags, pszEmail );
	}
};

#define REGISTER_UNHANDLED_EXCEPTION_HANDLER( nDumpFlags, email ) \
	static SRegisterUnhandledExceptionHandler g_RegisterUnhandledExceptionHandler(nDumpFlags, email)

#endif  // defined( PLATFORM_WIN32 )

// *********************************************************************** //

#else //DISABLE_ASSERTS

#undef VERIFY
#define VERIFY( exp )										exp

#undef assert
#define assert( exp )										(void)0

#undef ASSERT
#define ASSERT( exp )										(void)0

#define LTDEBUGBREAK()										(void)0

#define LTERROR( msg )										(void)0
#define LTASSERT( exp, msg )								(void)0

#define LTASSERT_PARAM1( exp, msg, p1 )						(void)0
#define LTASSERT_PARAM2( exp, msg, p1, p2 )					(void)0
#define LTASSERT_PARAM3( exp, msg, p1, p2, p3 )				(void)0
#define LTASSERT_PARAM4( exp, msg, p1, p2, p3, p4 )			(void)0
#define LTASSERT_PARAM5( exp, msg, p1, p2, p3, p4, p5 )		(void)0

#define LTERROR_PARAM1( msg, p1 )						(void)0
#define LTERROR_PARAM2( msg, p1, p2 )					(void)0
#define LTERROR_PARAM3( msg, p1, p2, p3 )				(void)0
#define LTERROR_PARAM4( msg, p1, p2, p3, p4 )			(void)0
#define LTERROR_PARAM5( msg, p1, p2, p3, p4, p5 )		(void)0

#define REGISTER_UNHANDLED_EXCEPTION_HANDLER( nDumpFlags )

#endif //DISABLE_ASSERTS

// *********************************************************************** //

#endif//__LTASSERT_H__

