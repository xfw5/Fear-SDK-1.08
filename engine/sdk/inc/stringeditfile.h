// ----------------------------------------------------------------------- //
//
// MODULE  : stringeditfile.h
//
// PURPOSE : Contains the file format chunk declarations
//
// CREATED : 08/05/04
//
// (c) 1999-2004 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __STRINGEDITFILE_H__
#define __STRINGEDITFILE_H__

// the four CC that is at the beginning of each string keeper file to ensure that the file
// is of the appropriate type
#define	STRINGEDIT_FILE_FOURCC		LTMakeFourCC('S', 'K', 'D', 'B')

// the current working file version that the string keeper files should be in to be properly
// handled
#define STRINGEDIT_FILE_VERSION		2

// the header of the string keeper file
struct StringEditFileHeader
{
	uint32							m_dwFourCC;
	uint32							m_dwVersion;

	uint32							m_nIDCount;
	uint32							m_nStringCount;
	uint32							m_nVoiceCount;

	uint32							m_nStringTableSize;
};

struct StringEditIndex
{
	uint32							m_idxValue;
	uint32							m_idxVoice;
};

#endif  // __STRINGEDITFILE_H__
