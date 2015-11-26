#include "platform.h"
#include "iperformancemonitor.h"

#if !defined(DISABLE_PERFORMANCE_MONITORING)

	#if defined(PLATFORM_SEM)

		//for the SEM module, all we need to do is call into our performance monitor function
		extern "C"
		{
			IPerformanceMonitor *GetIPerformanceMonitor();
		}

		IPerformanceMonitor* GetPerformanceMonitor()
		{
			//avoid the additional function call each time
			static IPerformanceMonitor *s_pIPerformanceMonitor = GetIPerformanceMonitor();
			return s_pIPerformanceMonitor;
		}

	#else //PLATFORM_SEM

		//we need this so we can load in the libraries
		#include "ltlibraryloader.h"

		//for the non SEM version, we actually need to load up the DLL and get the function
		typedef IPerformanceMonitor* (*TGetIPerformanceMonitorFn)();
		static IPerformanceMonitor* g_pIPerformanceMonitor;

		//this function will verify that the ILTMemory interface is hooked up correctly,
		//and will return true if it is. Otherwise it will return false and not try again
		static bool VerifyPerformanceMonitorInterface()
		{
			static bool s_bFailedInit = false;

			if(!g_pIPerformanceMonitor && !s_bFailedInit)
			{
				//get the calling module
				HLTMODULE hModule = LTLibraryLoader::OpenLibrary("performancemon.dll");
				if(hModule)
				{
					HLTPROC hProcedure = LTLibraryLoader::GetProcAddress(hModule, "GetIPerformanceMonitor");
					if(hProcedure)
					{
						TGetIPerformanceMonitorFn GetPerfMonFn = (TGetIPerformanceMonitorFn)hProcedure;
						g_pIPerformanceMonitor = GetPerfMonFn();
					}
				}

				//we should have an interface by this point
				if(!g_pIPerformanceMonitor)
				{
					s_bFailedInit = true;
					return false;
				}
			}

			return g_pIPerformanceMonitor != NULL;
		}

		//This will provide access to the ILTMemory interface.
		IPerformanceMonitor* GetPerformanceMonitor()
		{
			if(VerifyPerformanceMonitorInterface())
			{
				return g_pIPerformanceMonitor;
			}
			return NULL;
		}

	#endif //PLATFORM_SEM

#endif
