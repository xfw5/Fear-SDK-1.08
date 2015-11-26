// *********************************************************************** //
//
// MODULE  : linux_ltlibraryloader.cpp
//
// PURPOSE : Linux implementation of LTLibraryLoader.
//
// CREATED : 05/21/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#include <dlfcn.h>
#include <linux/stddef.h>
#include <stdio.h>
#include <link.h>
#include <stdafx.h>
#include "ltfileoperations.h"
#include "ltbasetypes.h"
#include "ltstrutils.h"
#include "ltlibraryloader.h"

HLTMODULE LTLibraryLoader::OpenLibrary(const char* pFileName)
{
	HLTMODULE handle = ::dlopen(pFileName, RTLD_GLOBAL | RTLD_NOW);
	
	if (handle == (HLTMODULE)NULL)
	{
		 printf("Failed loading module '%s': %s\n", pFileName, ::dlerror());
	}
	
	return handle;
}

void LTLibraryLoader::CloseLibrary(const HLTMODULE hModule)
{
	::dlclose((void*)hModule);
}

bool LTLibraryLoader::IsLibraryLoaded(const char* pFileName)
{
	char szTargetFileName[MAX_PATH];
	char szTargetSuffix[MAX_PATH];
	LTFileOperations::SplitPath(pFileName, NULL, szTargetFileName, szTargetSuffix);
	
	struct link_map* pLinkEntry = (struct link_map*)::dlopen(0, RTLD_LAZY);
	while(pLinkEntry)
	{
		char szLinkEntryFileName[MAX_PATH];
		char szLinkEntrySuffix[MAX_PATH];
		LTFileOperations::SplitPath(pLinkEntry->l_name, NULL, szLinkEntryFileName, szLinkEntrySuffix);

		if ((LTStrCmp(szTargetFileName, szLinkEntryFileName) == 0)
			&& (LTStrCmp(szTargetSuffix, szLinkEntrySuffix) == 0))
		{
			// found a match
			return true;
		}
		
		pLinkEntry = pLinkEntry->l_next;
	}
	
	// not found
	return false;
}

HLTMODULE LTLibraryLoader::GetLibraryHandle(const char* pFileName)
{
	HLTMODULE hModule = NULL;
	
	// first determine if the library is loaded
	if (IsLibraryLoaded(pFileName))
	{
		// call dlopen() to get its handle, which will increment its reference count
		hModule = ::dlopen(pFileName, RTLD_LAZY);
		
		// call dlclose() to decrement the reference count back to where it should be
		::dlclose(hModule);
	}
	
	return hModule;
}

HLTMODULE LTLibraryLoader::GetMainHandle()
{
	return ::dlopen(NULL, 0);
}

HLTPROC LTLibraryLoader::GetProcAddress(HLTMODULE hModule, const char* pProcName)
{
	return ::dlsym(hModule, pProcName);
}
