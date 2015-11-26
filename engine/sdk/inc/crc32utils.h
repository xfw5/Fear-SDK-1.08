//---------------------------------------------------------------------------------
// CRC32Utils.h
//
// CRC utility functions.
//---------------------------------------------------------------------------------

#ifndef __CRC32UTILS_H__
#define __CRC32UTILS_H__

namespace CRC32Utils
{
	uint32 CalcFileCRC(const char *pszFilename);
	uint32 CalcFileCRC(const char *pszFilename, char const* pszRealName);
	uint32 CalcDataCRC(const void *pData, uint32 nDataSize);
	uint32 CalcArchiveFileCRC(ILTInStream* pFileStream, const char* pszFilename);
};

#endif // __CRC32UTILS_H__
