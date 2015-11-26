// *********************************************************************** //
//
// MODULE  : ltguid.h
//
// PURPOSE : GUID related types, macros, and helper functions.
//
// CREATED : 09/12/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTGUID_H__
#define __LTGUID_H__

#include "ltstrutils.h"

struct _LTGUID
{
	uint32	a;
	uint16	b;
	uint16	c;
	uint8   d[8];
};

struct _LTGUID_64
{
	uint64	a;
	uint64  b;
};

union LTGUID
{
	_LTGUID guid;
	_LTGUID_64 guid_64;	   // use this for fast & easy compares

	inline
		bool operator<(const LTGUID& _X) const
	{ return (guid_64.a == _X.guid_64.a) ? (guid_64.b < _X.guid_64.b) : (guid_64.a < _X.guid_64.a); }

	inline
		bool operator==(const LTGUID& _X) const
	{ return (guid_64.a == _X.guid_64.a) && (guid_64.b == _X.guid_64.b); }
};

// macros for defining LTGUIDs a la windows...
// Define INITLTGUID where the LTGUIDs are defined;
// Undefine INITLTGUID where LTGUIDs are declared but not defined.
#ifndef INITLTGUID
#define DEFINE_LTGUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	extern "C" const LTGUID name
#else

#define DEFINE_LTGUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
	extern "C" const LTGUID name \
	= { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
#endif // INITLTGUID

// size of a GUID in string form
#define LTGUID_STRING_SIZE 32

// convert an LTGUID to a string
inline void LTGUIDToString(char* szBuffer, const uint32 nBufferSize, const LTGUID& cGUID)
{
	LTSNPrintF(szBuffer, nBufferSize, "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
		cGUID.guid.a,
		cGUID.guid.b,
		cGUID.guid.c,
		cGUID.guid.d[0],
		cGUID.guid.d[1],
		cGUID.guid.d[2],
		cGUID.guid.d[3],
		cGUID.guid.d[4],
		cGUID.guid.d[5],
		cGUID.guid.d[6],
		cGUID.guid.d[7]);
}

// convert a string to an LTGUID
inline void StringToLTGUID(const char* pszString, LTGUID& cGUID)
{
	::sscanf(pszString, "%8x%4x%4x%2x%2x%2x%2x%2x%2x%2x%2x", 
			 &cGUID.guid.a,
			 &cGUID.guid.b,
			 &cGUID.guid.c,
			 &cGUID.guid.d[0],
			 &cGUID.guid.d[1],
			 &cGUID.guid.d[2],
			 &cGUID.guid.d[3],
			 &cGUID.guid.d[4],
			 &cGUID.guid.d[5],
			 &cGUID.guid.d[6],
			 &cGUID.guid.d[7]);
}

#endif
