// *********************************************************************** //
//
// MODULE  : ltsocketutils.h
//
// PURPOSE : Helpers for socket operations.
//
// CREATED : 06/01/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LTSOCKETUTILS_H__
#define __LTSOCKETUTILS_H__

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

class LTSocketUtils
{
public:

	// initialize sockets - returns zero if successful
	static int InitializeSockets();

	// shutdown threading
	static void ShutdownSockets();

	// get last error value for this thread
	static int GetLastError();

	// clear current error value for this thread
	static void ClearLastError();

	// Set a socket's non-blocking mode attribute (true = non-blocking, false = blocking)
	static int SetNonBlockingMode(SOCKET socketFd, bool bNonBlocking);
	
	// close a socket
	static int CloseSocket(SOCKET socketFd);
};


#endif // __LTSOCKETUTILS_H__
