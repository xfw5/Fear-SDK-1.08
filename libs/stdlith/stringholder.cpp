//------------------------------------------------------------------
//
//	FILE	  : StringHolder.cpp
//
//	PURPOSE	  : Implementation for the CStringHolder class.
//
//	CREATED	  : 5/1/96
//
//	COPYRIGHT : Monolith 1996 All Rights Reserved
//
//------------------------------------------------------------------

// Includes....
#include "stringholder.h"
#include <string.h>
#include "ltassert.h"
#include "ltmath.h"

#define DEFAULT_ALLOC_SIZE	150


CStringHolder::CStringHolder()
{
	m_AllocSize = DEFAULT_ALLOC_SIZE;
}


CStringHolder::CStringHolder( uint32 allocSize )
{
	m_AllocSize = allocSize;
}

CStringHolder::~CStringHolder()
{
	ClearStrings();
}

void CStringHolder::SetAllocSize( uint32 size )
{
	ASSERT( m_Strings.GetSize() == 0 );
	m_AllocSize = size;
}


char *CStringHolder::AddString( const char *pString, bool bFindFirst )
{
	uint32	len = strlen(pString);
	return AddString( pString, bFindFirst, len );
}


char *CStringHolder::AddString( const char *pString, bool bFindFirst, uint32 len )
{
	ASSERT( len <= m_AllocSize );

	// See if we can find it in an array first.
	if( bFindFirst )
	{
		char *pRetVal = FindString( pString );
		if( pRetVal )
			return pRetVal;
	}

	// See if it'll fit in any of the arrays.
	bool bFoundOne = false;
	SBank *pBank = NULL;
	uint32 i;
	for( i=0; i < m_Strings; i++ )
	{
		SBank *pBank = &m_Strings[i];

		if((len+pBank->m_StringSize+1) < pBank->m_AllocSize)
		{
			bFoundOne = true;
			break;
		}
	}

	if( !bFoundOne )
	{
		uint32 allocSize = len + 1;
		if(m_AllocSize > allocSize)
			allocSize = m_AllocSize;
		
		SBank theString;
		theString.m_pString = new char[allocSize];
		if( !theString.m_pString )
			return NULL;
		
		theString.m_AllocSize = allocSize;
		theString.m_StringSize = 0;
		if( !m_Strings.Append(theString) )
		{
			theString.m_pString;
			return NULL;
		}
		
		i = (uint16)(m_Strings - 1);
	}

	pBank = &m_Strings[i];
	char *pRetVal = pBank->m_pString + pBank->m_StringSize;

	memcpy( pRetVal, pString, len );
	pRetVal[len] = 0;

	uint32 nNewSize = pBank->m_StringSize + len + 1;
	ASSERT(nNewSize <= pBank->m_AllocSize);
	pBank->m_StringSize = (uint16)LTMIN(nNewSize, 65535);

	return pRetVal;
}


char *CStringHolder::FindString( const char *pString )
{
	for(uint32 a=0; a < m_Strings; a++ )
	{
		SBank *pBank = &m_Strings[a];
		char *pBaseString = pBank->m_pString;
		
		uint32 i=0;
		while( i < pBank->m_StringSize )
		{
			uint32 len = strlen( &pBaseString[i] );
			if( strcmp(pString, &pBaseString[i]) == 0 )
				return &pBaseString[i];
	
			i += len + 1;
		}	
	}

	return NULL;
}



void CStringHolder::ClearStrings()
{
	for(uint32 i=0; i < m_Strings; i++ )
	{
		delete m_Strings[i].m_pString;
	}

	m_Strings.SetSize(0);
}


