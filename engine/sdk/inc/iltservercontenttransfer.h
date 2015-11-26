// *********************************************************************** //
//
// MODULE  : iltservercontenttransfer.h
//
// PURPOSE : Interface class for the server side of a content transfer.
//
// CREATED : 07/28/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __ILTSERVERCONTENTTRANSFER_H__
#define __ILTSERVERCONTENTTRANSFER_H__

#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

// forward declarations
class CPacket_Read;
class ILTContentTransferNotification;

// structure for server content transfer options
struct SServerContentTransferOptions
{
	SServerContentTransferOptions()
		:	m_nClientTimeoutMS(0),
			m_bAllowContentDownload(false),
			m_nMaxDownloadRatePerClient(0),
			m_nMaxDownloadRateAllClients(0),
			m_nMaxSimultaneousDownloads(0),
			m_nMaxDownloadSize(0),
			m_ppszRedirectURLs(NULL),
			m_nNumberOfRedirectURLs(0),
			m_pwszContentDownloadMessage(NULL),
			m_bOwnership(false) {}

	~SServerContentTransferOptions()
	{
		Deallocate();
	}

	void operator= (const SServerContentTransferOptions& RHS)
	{
		Deallocate();
			
		m_nClientTimeoutMS           = RHS.m_nClientTimeoutMS;
		m_bAllowContentDownload      = RHS.m_bAllowContentDownload;
		m_nMaxDownloadRatePerClient  = RHS.m_nMaxDownloadRatePerClient;
		m_nMaxDownloadRateAllClients = RHS.m_nMaxDownloadRateAllClients;
		m_nMaxSimultaneousDownloads  = RHS.m_nMaxSimultaneousDownloads;
		m_nMaxDownloadSize           = RHS.m_nMaxDownloadSize;
		m_nNumberOfRedirectURLs      = RHS.m_nNumberOfRedirectURLs;

		LT_MEM_TRACK_ALLOC(m_ppszRedirectURLs = new const char*[m_nNumberOfRedirectURLs], LT_MEM_TYPE_MISC);
		for (uint32 nIndex = 0; nIndex < m_nNumberOfRedirectURLs; ++nIndex)
		{
			LT_MEM_TRACK_ALLOC(m_ppszRedirectURLs[nIndex] = new char[LTStrLen(RHS.m_ppszRedirectURLs[nIndex]) + 1], LT_MEM_TYPE_MISC);
			LTStrCpy((char*)m_ppszRedirectURLs[nIndex], RHS.m_ppszRedirectURLs[nIndex], LTStrLen(RHS.m_ppszRedirectURLs[nIndex]) + 1);
		}

		LT_MEM_TRACK_ALLOC(m_pwszContentDownloadMessage = new wchar_t[LTStrLen(RHS.m_pwszContentDownloadMessage) + 1], LT_MEM_TYPE_MISC);
		LTStrCpy((wchar_t*)m_pwszContentDownloadMessage, RHS.m_pwszContentDownloadMessage, LTStrLen(RHS.m_pwszContentDownloadMessage) + 1);

		m_bOwnership = true;
	}

	// timeout period for activity on a client connection (including heartbeat)
	uint32	       m_nClientTimeoutMS;

	// indicates whether the server will send files to clients
	bool	       m_bAllowContentDownload;

	// maximum bandwidth to allocate to a single downloading client
	uint32	       m_nMaxDownloadRatePerClient;

	// maximum bandwidth to allocate to all downloading clients
	uint32	       m_nMaxDownloadRateAllClients;

	// maximum number of simultaneous downloads
	uint8	       m_nMaxSimultaneousDownloads;

	// maximum total size that the server will agree to send to the client (all files)
	uint32	       m_nMaxDownloadSize;

	// list of redirect URLs
	const char**   m_ppszRedirectURLs;

	// number of redirect URLs in the list
	uint32	       m_nNumberOfRedirectURLs;

	// message to send to the client if the download cannot be completed
	const wchar_t* m_pwszContentDownloadMessage;

private:

	void Deallocate()
	{
		if (m_bOwnership)
		{
			// free redirect URLs
			for (uint32 nIndex = 0; nIndex < m_nNumberOfRedirectURLs; ++nIndex)
			{
				delete [] m_ppszRedirectURLs[nIndex];
			}
			delete [] m_ppszRedirectURLs;
			m_ppszRedirectURLs = NULL;

			// free download message
			delete [] m_pwszContentDownloadMessage;
			m_pwszContentDownloadMessage = NULL;
		}
	}

	// indicates ownership of the allocated members
	bool		   m_bOwnership;

};

// ILTServerContentTransfer interface
class ILTServerContentTransfer : public IBase
{
public:

	interface_version(ILTServerContentTransfer, 0);
	declare_interface(ILTServerContentTransfer);

	ILTServerContentTransfer() {}
	virtual ~ILTServerContentTransfer() {}

	// sets the ILTContentTransferNotification-derived class to receive notifications, and a set of
	// options specifying which notifications should be sent
	virtual LTRESULT SetNotificationSink(ILTContentTransferNotification* pNotificationSink, uint32 nNotificationOptions);
    
	// set the options for the server content transfer
	virtual LTRESULT SetOptions(const SServerContentTransferOptions& cOptions);

	// builds and sends the content transfer message to the client
	virtual LTRESULT SendTransferInfoToClient(HCLIENT hClient);

	// message handler for messages received while in the content transfer state
	virtual LTRESULT OnMessage(HCLIENT hClient, CPacket_Read& cMsg);

	// update method for driving the content transfer implementation (called once for each 
	// connection that is in the content transfer state)
	virtual LTRESULT OnUpdate(HCLIENT hClient);

	// abort a content transfer with the specified client
	virtual LTRESULT AbortTransfer(HCLIENT hClient);

	// compresses server-side file archives
	virtual LTRESULT PrepareFileArchives(bool bCompressFileArchives);
		
};

#endif // __ILTSERVERCONTENTTRANSFER_H__
