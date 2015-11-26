// *********************************************************************** //
//
// MODULE  : win32_ltfilebuffer.h
//
// PURPOSE : File buffer and object bank for Win32 the CLTFile implementation.
//
// CREATED : 09/17/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __WIN32_LTFILEBUFFER_H__
#define __WIN32_LTFILEBUFFER_H__

#include "object_bank.h"

// buffer allocation size
enum { k_nWin32FileBufferSize = 4096 };

class CWin32_LTFileBuffer
{
public:

	CWin32_LTFileBuffer() {}
	~CWin32_LTFileBuffer() {}

	// creates a new buffer from the object bank.  Note that Release() must be called to
	// return the buffer to the object bank.
	static CWin32_LTFileBuffer* Allocate();

	// accessor for the buffer
	char* GetBuffer() { return m_pBuffer; }
	
	// returns the buffer to the object bank when it is no longer needed
	void Release();

private:

	// buffer
	char m_pBuffer[k_nWin32FileBufferSize];

};

#endif
