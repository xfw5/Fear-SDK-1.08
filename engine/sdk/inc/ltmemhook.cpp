//////////////////////////////////////////////////////////////////////////////
// Debug new handler implementation

#include "platform.h"
#include "ltlibraryloader.h"
#include "iltmemory.h"

#ifndef DISABLE_MEMORY_TRACKING

#if defined(PLATFORM_WIN32)
	#include <windows.h>
#endif

	//the global ILTMemory interface for use	
#if defined(PLATFORM_SEM)
	extern "C"
	{
		ILTMemory *GetILTMemory();
	}
#else // PLATFORM_SEM
	typedef ILTMemory* (*TGetILTMemoryFn)();
#endif // !PLATFORM_SEM
	static ILTMemory* g_pILTMemory;

	//this function will verify that the ILTMemory interface is hooked up correctly,
	//and will return true if it is. Otherwise it will return false and not try again
	static bool VerifyMemoryInterface()
	{
		static bool s_bFailedMemInit = false;

		if(!g_pILTMemory && !s_bFailedMemInit)
		{
#if defined(PLATFORM_SEM)
			g_pILTMemory = GetILTMemory();
#else // !PLATFORM_SEM
			//get the calling module
			HLTMODULE hModule = LTLibraryLoader::OpenLibrary("LTMemory.dll");
			if(hModule)
			{
				HLTPROC hProcedure = LTLibraryLoader::GetProcAddress(hModule, "GetILTMemory");

				if(hProcedure)
				{
					TGetILTMemoryFn MemoryFn = (TGetILTMemoryFn)hProcedure;
					g_pILTMemory = MemoryFn();
				}
			}

			//we should have an interface by this point
			if(!g_pILTMemory)
			{
				s_bFailedMemInit = true;
				return false;
			}
#endif // !PLATFORM_SEM
		}

		return g_pILTMemory != NULL;
	}

	//function that allows for pushing the context data onto the stack
	void LTMemPushMemoryContext(const char* pszFile, uint32 nLine, uint32 nCategoryID)
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->PushMemoryContext(pszFile, nLine, nCategoryID);
		}
	}

	//function that allows for pushing the context data onto the stack
	void LTMemPopMemoryContext()
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->PopMemoryContext();
		}
	}

	void LTMemExternalAlloc(uint32 nMemCategory, uint32 nSize, void *pMemory)
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->TrackExternalAllocation(nMemCategory, nSize, pMemory);
		}
	}

	void LTMemExternalFree(uint32 nMemCategory, uint32 nSize, void *pMemory)
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->FreeExternalAllocation(nMemCategory, nSize, pMemory);
		}
	}

	void LTMemDisableStackTracking()
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->DisableStackTracking();
		}
	}

	void LTMemEnableStackTracking()
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->EnableStackTracking();
		}
	}

	void LTMemOnExit()
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->OnExit();
		}
	}

	//This will provide access to the ILTMemory interface.
	ILTMemory*	LTMemGetILTMemory()
	{
		if(VerifyMemoryInterface())
		{
			return g_pILTMemory;
		}
		return NULL;
	}

	//overload operator new
	void* operator new (size_t nSize)
	{
		if(VerifyMemoryInterface())
		{
			return g_pILTMemory->Allocate((uint32)nSize);
		}

		//no memory interface available, just allocate the data
		return malloc(nSize);
	}

	void* operator new[] (size_t nSize)
	{
		if(VerifyMemoryInterface())
		{
			return g_pILTMemory->Allocate((uint32)nSize);
		}

		//no memory interface available, just allocate the data
		return malloc(nSize);
	}

	//overload operator delete
	void operator delete(void* pData)
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->Free(pData);
			return;
		}

		//no memory interface available, just allocate the data
		free(pData);
	}

	//overload operator delete
	void operator delete[](void* pData)
	{
		if(VerifyMemoryInterface())
		{
			g_pILTMemory->Free(pData);
			return;
		}

		//no memory interface available, just allocate the data
		free(pData);
	}


#endif //LTMEMTRACK
