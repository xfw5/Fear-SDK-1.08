#include "ltprofilercontrol.h"
#include "ltbasedefs.h"
#include "ltlibraryloader.h"

//a class to handle binding to our module
class CVTuneLibrary
{
public:

	//external function prototypes
	typedef void (*TPauseProfilerFn)();
	typedef void (*TResumeProfilerFn)();

	CVTuneLibrary() :
      	m_bFailedLoad(false),
		m_hModule(NULL),
		m_pfnPauseProfiler(NULL),
		m_pfnResumeProfiler(NULL)
	{
	}

	~CVTuneLibrary()
	{
		LTLibraryLoader::CloseLibrary(m_hModule);
	}

	//called to bind the module. This will do nothing if it is already bound
	bool BindVTuneModule()
	{
		//bail if already bound
		if(m_hModule)
			return true;

		//see if we have already failed to load
		if(m_bFailedLoad)
			return false;

		//now we need to load up this module
		m_hModule = LTLibraryLoader::OpenLibrary("vtuneapi.dll");
		if(!m_hModule)
		{
			m_bFailedLoad = true;
			return false;
		}

		//bind the functions
		m_pfnPauseProfiler	= (TPauseProfilerFn)LTLibraryLoader::GetProcAddress(m_hModule, "VTPause");
		m_pfnResumeProfiler = (TResumeProfilerFn)LTLibraryLoader::GetProcAddress(m_hModule, "VTResume");

		//check the functions
		if(!m_pfnPauseProfiler || !m_pfnResumeProfiler)
		{
			//free our functions
			m_pfnPauseProfiler  = NULL;
			m_pfnResumeProfiler = NULL;

			//and our library
			LTLibraryLoader::CloseLibrary(m_hModule);
			m_hModule = NULL;

			m_bFailedLoad = true;
			return false;
		}

		//success
		return true;
	}

	TPauseProfilerFn	m_pfnPauseProfiler;
	TResumeProfilerFn	m_pfnResumeProfiler;
	
private:

	bool				m_bFailedLoad;
	HLTMODULE			m_hModule;
};

//the global manager for the module
CVTuneLibrary	g_VTuneModule;


//called to pause the sampling of the profiler
void LTPauseProfiler()
{
	if(g_VTuneModule.BindVTuneModule())
	{
		g_VTuneModule.m_pfnPauseProfiler();
	}
}

//called to resume sampling of the profiler
void LTResumeProfiler()
{
	if(g_VTuneModule.BindVTuneModule())
	{
		g_VTuneModule.m_pfnResumeProfiler();
	}
}
