// *********************************************************************** //
//
// MODULE  : win32_ltsocketutils.cpp
//
// PURPOSE : Win32 implementation of LTSocketUtils.
//
// CREATED : 06/01/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <stdafx.h>
#include "ltsocketutils.h"

int LTSocketUtils::InitializeSockets()
{
	uint16  wVersion = MAKEWORD(1,1);
	WSADATA wsaData;

	return ::WSAStartup(wVersion, &wsaData);
}

void LTSocketUtils::ShutdownSockets()
{
	::WSACleanup();
}

int LTSocketUtils::GetLastError()
{
	return ::WSAGetLastError();
}

void LTSocketUtils::ClearLastError()
{
	::WSASetLastError(0);
}

int LTSocketUtils::SetNonBlockingMode(SOCKET socketFd, bool bNonBlocking)
{
	unsigned long value = 1;

	if (!bNonBlocking)
	{
		int value = 0;
	}

	return ::ioctlsocket(socketFd, FIONBIO, &value);
}

int LTSocketUtils::CloseSocket(SOCKET socketFd)
{
	return ::closesocket(socketFd);
}


