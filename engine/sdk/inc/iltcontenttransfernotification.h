// *********************************************************************** //
//
// MODULE  : iltcontenttransfernotification.h
//
// PURPOSE : Interface class for notifications about the content transfer.
//
// CREATED : 07/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTCONTENTTRANSFERNOTIFICATION_H__
#define __ILTCONTENTTRANSFERNOTIFICATION_H__

// options for notification callbacks
enum EContentTransferNotification
{
	k_nNotifyNone = 0,
	k_nNotifyFileSendProgress,
	k_nNotifyFileSendCompleted,
	k_nNotifyFileReceiveProgress,
	k_nNotifyFileReceiveCompleted,
	k_nNotifyContentTransferStarting,
	k_nNotifyContentTransferCompleted,
	k_nNotifyContentTransferError,
	k_nNotifyAll = k_nNotifyFileSendProgress | k_nNotifyFileSendCompleted | k_nNotifyFileReceiveProgress | k_nNotifyFileReceiveCompleted | k_nNotifyContentTransferStarting | k_nNotifyContentTransferCompleted | k_nNotifyContentTransferError
};

// error codes
enum EContentTransferError
{
	eContentTransferErrorServerDownloadRefused,
	eContentTransferErrorServerDownloadTooLarge,
	eContentTransferErrorServerFileNotFound,
	eContentTransferErrorClientDownloadNotAllowed,
	eContentTransferErrorClientDownloadTooLarge,
	eContentTransferErrorClientMissingNonDownloadableArchive,
	eContentTransferErrorServerAtMaximumDownloads,
	eContentTransferErrorFileTransfer,
	eContentTransferErrorUnknown
};

class ILTContentTransferNotification
{
public:
	
	ILTContentTransferNotification() {}
	virtual ~ILTContentTransferNotification() {}

	// notifications for a file receiver
	virtual void FileReceiveProgressNotification(const std::string& strFilename,
												 uint32				nFileBytesReceived,
												 uint32				nFileBytesTotal,
												 uint32				nTransferBytesTotal,
												 float				fTransferRate) {}

	virtual void FileReceiveCompletedNotification(const std::string& strFilename,
												  uint32			 nFileBytesTotal,
												  uint32			 nTransferBytesTotal,
												  float				 fTransferRate) {}

	// notifications for a file sender
	virtual void FileSendProgressNotification(HCLIENT			 hClient, 
											  const std::string& strFilename, 
											  uint32			 nBytesSent,
											  uint32			 nBytesTotal,
											  float				 fTransferRate) {}

	virtual void FileSendCompletedNotification(HCLIENT			  hClient,
											   const std::string& strFilename,
											   uint32			  nBytesTotal,
											   float			  fTransferRate) {}

	// notifications for the content transfer client
    virtual void ClientContentTransferStartingNotification(uint32 nTotalBytes, uint32 nTotalFiles) {}
	virtual void ClientContentTransferCompletedNotification() {}
	virtual void ClientContentTransferErrorNotification(EContentTransferError eError, const char* pszFilename, const wchar_t* pwszMessage) {}

	// notifications for the content transfer server
	virtual void ServerContentTransferStartingNotification(HCLIENT hClient) {}
	virtual void ServerContentTransferCompletedNotification(HCLIENT hClient) {}
	virtual void ServerContentTransferErrorNotification(HCLIENT hClient, EContentTransferError eError) {}

};

#endif // __ILTCONTENTTRANSFERNOTIFICATION_H__
