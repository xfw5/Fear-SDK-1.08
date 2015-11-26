// *********************************************************************** //
//
// MODULE  : win32_ltfileread.cpp
//
// PURPOSE : Win32 implementation for ILTFileRead.
//
// CREATED : 09/13/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include "stdafx.h"
#include "win32_ltfileread.h"

bool CWin32_LTFileRead::Read(void* const pBuffer, const uint32 nBytesToRead)
{
	LTASSERT(GetFileHandle() != INVALID_HANDLE_VALUE, "invalid file handle");

	// check to see if we have enough in the buffer to satisfy the request
	uint32 nRemainingBytesInBuffer = GetRemainingBytesInBuffer();
	if (nRemainingBytesInBuffer >= nBytesToRead)
	{
		// get the data from the buffer
		GetDataFromBuffer(pBuffer, nBytesToRead);
	}
	else
	{
		// we need more than what's currently available in the buffer to satisfy
		// this request.  First, copy the remaining data (if any) from the read buffer
		// to the user buffer
		if (nRemainingBytesInBuffer)
		{
			if (!GetDataFromBuffer(pBuffer, nRemainingBytesInBuffer))
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}
		}

		// calculate the amount left to read
		uint32 nAmountRemainingToRead = nBytesToRead - nRemainingBytesInBuffer;

		// if the amount we need to read is more than half of our buffer allocation size,
		// then read it directly into the user buffer
		if (nAmountRemainingToRead >= (k_nWin32FileBufferSize / 2))
		{
			uint32 nAmountReadFromFile = 0;
			if (!ReadFromFile((uint8*)pBuffer + (nBytesToRead - nAmountRemainingToRead), nAmountRemainingToRead, nAmountReadFromFile))
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}

			// check to see if we read the full amount
			if (nAmountReadFromFile != nAmountRemainingToRead)
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}
		}
		else
		{
			// perform a full read into the internal buffer
			if (!FillBufferFromFile())
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}

			// check to see if the file actually had enough data remaining to satisfy the request
			if (m_nBufferAmount < nAmountRemainingToRead)
			{
				// not enough left in the file
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}

			// get the remaining amount from the buffer
			if (!GetDataFromBuffer((uint8*)pBuffer + (nBytesToRead - nAmountRemainingToRead), nAmountRemainingToRead))
			{
				::memset(pBuffer, 0, nBytesToRead);
				return false;
			}
		}
	}

	// success
	return true;
}
