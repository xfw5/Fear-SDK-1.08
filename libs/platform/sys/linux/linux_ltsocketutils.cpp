// *********************************************************************** //
//
// MODULE  : linux_ltsocketutils.cpp
//
// PURPOSE : Linux implementation of LTSocketUtils.
//
// CREATED : 06/01/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltsocketutils.h"
#include <fcntl.h>
#include <errno.h>

int LTSocketUtils::InitializeSockets()
{
	// no initialization necessary on Linux
	return 0;
}

void LTSocketUtils::ShutdownSockets()
{
	// no shutdown necessary on Linux
}

int LTSocketUtils::GetLastError()
{
	return errno;
}

void LTSocketUtils::ClearLastError()
{
	errno = 0;
}

int LTSocketUtils::SetNonBlockingMode(SOCKET socketFd, bool bNonBlocking)
{
	// get the current value from fcntl
	int currentValue = 0;
	currentValue = ::fcntl(socketFd, F_GETFL, 0);

	// now mask in or mask out the flag as appropriate
	if (bNonBlocking)
	{
		currentValue |= O_NONBLOCK;
	}
	else
	{
		currentValue &= ~O_NONBLOCK;
	}

	// set the new value
	return ::fcntl(socketFd, F_SETFL, currentValue);
}

int LTSocketUtils::CloseSocket(SOCKET socketFd)
{
	return close(socketFd);
}

