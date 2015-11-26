// *********************************************************************** //
//
// MODULE  : iltclientcontenttransfer.h
//
// PURPOSE : Interface class for the client side of a content transfer.
//
// CREATED : 07/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTCLIENTCONTENTTRANSFER_H__
#define __ILTCLIENTCONTENTTRANSFER_H__

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

// forward declarations
class CPacket_Read;
class ILTContentTransferNotification;

// structure for client content transfer options
struct SClientContentTransferOptions
{
	SClientContentTransferOptions()
		: m_bAllowContentDownload(false),
		  m_bAllowContentDownloadRedirect(false),
		  m_nMaxDownloadSize(0) {}

	void operator= (const SClientContentTransferOptions& RHS)
	{
		LTStrCpy(m_szCustomContentDirectory, RHS.m_szCustomContentDirectory, LTARRAYSIZE(m_szCustomContentDirectory));

		m_bAllowContentDownload         = RHS.m_bAllowContentDownload;
		m_bAllowContentDownloadRedirect = RHS.m_bAllowContentDownloadRedirect;
		m_nMaxDownloadSize              = RHS.m_nMaxDownloadSize;
	}

	// path to custom content directory
	char   m_szCustomContentDirectory[MAX_PATH];

	// indicates whether the client will download custom content
	bool   m_bAllowContentDownload;

	// indicates whether the client will use HTTP redirect servers
	bool   m_bAllowContentDownloadRedirect;

	// maximum total transfer size the client will accept
	uint32 m_nMaxDownloadSize;
};

// structure for archive file information
struct SClientContentTransferArchiveFileInfo
{
	SClientContentTransferArchiveFileInfo() : m_nSize(0) { ::memset(&m_cGUID, 0, sizeof(LTGUID)); }
	
	// GUID of the archive file
	LTGUID m_cGUID;

	// size of the archive file
	uint32 m_nSize;
};

// ILTClientContentTransfer interface
class ILTClientContentTransfer : public IBase
{
public:

	interface_version(ILTClientContentTransfer, 0);
	declare_interface(ILTClientContentTransfer);

	ILTClientContentTransfer() {}
	virtual ~ILTClientContentTransfer() {}

	// sets the ILTContentTransferNotification-derived class to receive notifications, and the options 
	// specifying which notifications should be sent
	virtual LTRESULT SetNotificationSink(ILTContentTransferNotification* pNotificationSink, uint32 nNotificationOptions);

	// set the options for the client content transfer
	virtual LTRESULT SetOptions(const SClientContentTransferOptions& cOptions);

	// message handler for client messages received while in the content transfer state
	virtual LTRESULT OnMessage(CPacket_Read& cPacket);

	// update method called while the client is in the content transfer state
	virtual LTRESULT OnUpdate();

	// aborts a content download in progress
	virtual LTRESULT AbortContentDownload();

	// cleans up and unloads resources when the client disconnects from a server
	virtual LTRESULT OnDisconnect();

	// checks for the presence of the specified archive files on the client, and returns
	// the total size of all missing files, if any
	virtual LTRESULT CheckForMissingArchiveFiles(SClientContentTransferArchiveFileInfo* ppArchiveFileList,
												 uint32  nNumberOfArchiveFiles,
												 bool&   bIsMissingFiles,
												 uint32& nTotalMissingFilesSize);

};

#endif // __ILTCLIENTCONTENTTRANSFER_H__
