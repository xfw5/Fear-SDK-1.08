// *********************************************************************** //
//
// MODULE  : win32_ltfilebuffer.cpp
//
// PURPOSE : File buffer and object bank for Win32 the CLTFile implementation.
//
// CREATED : 09/17/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include "win32_ltfilebuffer.h"
#include "ltautocriticalsection.h"

// object bank for allocating file buffers
static const uint32 knFileBufferStartSize  = 5;
static const uint32 knFileBufferGrowSize = 5;
static ObjectBank<CWin32_LTFileBuffer, LT_MEM_TYPE_MISC> g_FileBufferObjectBank(knFileBufferGrowSize, knFileBufferStartSize);

//critical section used to control access to the object bank
CLTCriticalSection	g_FileBufferObjectBankCS;

// creates a new buffer from the object bank.  Note that Release() must be called to
// return the buffer to the object bank.
CWin32_LTFileBuffer* CWin32_LTFileBuffer::Allocate()
{
	//restrict access to prevent race conditions from corrupting our bank
	CLTAutoCriticalSection AutoCS(g_FileBufferObjectBankCS);

	// allocate a stream from the bank
	CWin32_LTFileBuffer* pFileBuffer = g_FileBufferObjectBank.Allocate();

	if(!pFileBuffer)
	{
		return NULL;
	}

	return pFileBuffer;
}

// returns the buffer to the object bank when it is no longer needed
void CWin32_LTFileBuffer::Release()
{
	//restrict access to prevent race conditions from corrupting our bank
	CLTAutoCriticalSection AutoCS(g_FileBufferObjectBankCS);

	g_FileBufferObjectBank.Free(this);
}
