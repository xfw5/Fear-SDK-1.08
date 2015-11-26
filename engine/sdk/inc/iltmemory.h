//------------------------------------------------------------------
// ILTMemory.h
//
// Defines the interface between binaries and the memory DLL which
// is responsible for handling memory debugging, tracking, and
// reporting. All allocations should go through this interface if
// tracking is enabled.
//
//------------------------------------------------------------------

#ifndef __ILTMEMORY_H__
#define __ILTMEMORY_H__

//By changing the following lines you can enable memory tracking in final builds, or
//remove memory tracking from debug or release builds. Also you can setup a project to
//have the value DISABLE_LT_MEM_TRACKING to prevent memory tracking in that project.
//Note that memory tracking is always disabled in Linux builds.
#if defined(_FINAL) || defined(PLATFORM_LINUX)
#define DISABLE_MEMORY_TRACKING
#endif

//standard symbols
#ifndef __LTBASETYPES_H__
#	include "ltbasetypes.h"
#endif

//bring in all of the memory category IDs
#ifndef __LTMEMORYCATEGORIES_H__
#	include "ltmemorycategories.h"
#endif

class ILTMemory
{
public:

	//overridable destructor
	virtual ~ILTMemory()	{}

	//function prototype for a display function that can be set
	typedef void (*TDisplayFn)(const char* pszText);

	//allows for specifying a display function which is necessary for any output to any non-file
	//sources. Setting this will override any existing display functions.
	virtual bool	SetDisplayFunction(TDisplayFn DisplayFn)=0;

	//tells the memory manager to update any watches so that any tracked categories can be
	//displayed
	virtual bool	UpdateFrameDisplays()=0;

	//runs a command string. See the associated documentation for what can be in this command
	//string
	virtual bool	RunCommandString(uint32 nArgC, const char** ppArgV)=0;

	//this returns the number of categories that are tracked
	virtual uint32	GetNumCategories() = 0;

	//this returns the ID of the specified system by index, or -1 if an invalid index is provided
	virtual uint32	GetCategoryID(uint32 nIndex) = 0;

	//given a category ID, this will return the name, or NULL if the ID is invalid. This returned
	//pointer is valid until the memory system is unbound
	virtual const char*	GetCategoryName(uint32 nMemCategory) = 0;

	//given a category ID this will fill in the stats for the memory category. NULL may be used
	//to specify not to get that information. If false is returned, the category could not be found
	//and the values will be unmodified
	virtual bool	GetCategorySelfStats(uint32 nMemCategory, uint32* pAllocs, uint32* pMemory, uint32* pTotalAllocs, uint32* pTotalMem) = 0;

	//given a category ID this will fill in the stats for the memory category's children. NULL may be used
	//to specify not to get that information. If false is returned, the category could not be found
	//and the values will be unmodified
	virtual bool	GetCategoryChildStats(uint32 nMemCategory, uint32* pAllocs, uint32* pMemory, uint32* pTotalAllocs, uint32* pTotalMem) = 0;

	//this function will set the current context of the allocation. Only the
	//first context will be associated with the memory. In order to pop the context
	//once set, the corresponding function should be used.
	virtual bool	PushMemoryContext(const char* pszFile, uint32 nLine, uint32 nMemCategory)=0;

	//this function will pop the context stack if anything is placed on it. This should
	//be called once for every time the memory context is pushed.
	virtual bool	PopMemoryContext()=0;

	//this function will allocate a block of memory and handle all tracking of it. It must
	//still be deleted by the corresponding deallocation function.
	virtual void*	Allocate(uint32 nSize)=0;

	//handles freeing of memory
	virtual void	Free(void* pMemory)=0;

	//this will allow the tracking of a memory size through the memory manager. Note that this
	//is not a real allocation, but will act like an allocation to the category list for reports
	//and tracking. This is to provide a means for tracking allocations that are done out of
	//process such as driver allocations
	virtual void	TrackExternalAllocation(uint32 nMemCategory, uint32 nMemorySize, void *pMemory)=0;

	//this will free an external allocation and remove its counts from the category specified.
	//This assumes that an external allocation of equal size had been made to this category
	//previously
	virtual void	FreeExternalAllocation(uint32 nMemCategory, uint32 nMemorySize, void *pMemory)=0;

	//disables stack tracking temporarily, if supported by the implementation
	virtual void	DisableStackTracking() {}

	//re-enable stack tracking, if supported by the implementation
	virtual void	EnableStackTracking() {}

	//notification that the system is shutting down
	virtual void	OnExit() {}
};

#ifndef DISABLE_MEMORY_TRACKING

	// function prototypes for memory tracking functions that are called from the macros. These are
	// defined in the memory hook source file, and are never intended to be called directly.
	// Instead use the macros so they will be compiled out if memory tracking is disabled
	void		LTMemPushMemoryContext(const char* sFileName, uint32 nLineNum, uint32 nCategory);
	void		LTMemPopMemoryContext();
	void		LTMemExternalAlloc(uint32 nMemCategory, uint32 nSize, void *pMemory);
	void		LTMemExternalFree(uint32 nMemCategory, uint32 nSize, void *pMemory);
	void		LTMemDisableStackTracking();
	void		LTMemEnableStackTracking();
	void		LTMemOnExit();

	//This will provide access to the ILTMemory interface. This can return NULL, so the value
	//should always be checked
	ILTMemory*	LTMemGetILTMemory();

	// Class for tracking categories by scope
	class CLTMem_ScopeTrack
	{
	public:
		CLTMem_ScopeTrack(const char *pFile, uint32 nLine, uint32 nCategory) { LTMemPushMemoryContext(pFile, nLine, nCategory); }
		~CLTMem_ScopeTrack() { LTMemPopMemoryContext(); }
	};

	// macros to do memory tracking
	#define	LT_MEM_BEGIN_TRACKING(nCategory)			{ LTMemPushMemoryContext(__FILE__, __LINE__, nCategory); }
	#define LT_MEM_END_TRACKING()						{ LTMemPopMemoryContext(); }
	#define LT_MEM_TRACK_ALLOC(ltStatement, nCategory)	{ LT_MEM_BEGIN_TRACKING(nCategory); ltStatement; LT_MEM_END_TRACKING(); }
	#define LT_MEM_EXTERNAL_ALLOC(nCategory, nSize, pMemory)	{ LTMemExternalAlloc(nCategory, nSize, pMemory); }
	#define LT_MEM_EXTERNAL_FREE(nCategory, nSize, pMemory)		{ LTMemExternalFree(nCategory, nSize, pMemory); }
	#define LT_MEM_TRACK_SCOPE(nCategory)				CLTMem_ScopeTrack LT_MEM_CUR_SCOPE(__FILE__, __LINE__, nCategory);
	#define LT_MEM_DISABLESTACK()						{ LTMemDisableStackTracking(); }
	#define LT_MEM_ENABLESTACK()						{ LTMemEnableStackTracking(); }
	#define LT_MEM_ONEXIT()								{ LTMemOnExit(); }
	
#else

	//if tracking is not enabled, the memory interface will always be NULL
	inline ILTMemory*	LTMemGetILTMemory()		{ return NULL; }
	
	// macros that do nothing but the basic allocation and free's when mem tracking is off
	#define	LT_MEM_BEGIN_TRACKING(nCategory)			
	#define LT_MEM_END_TRACKING()						
	#define LT_MEM_TRACK_ALLOC(ltStatement, ltAllocType) ltStatement
	#define LT_MEM_EXTERNAL_ALLOC(nCategory, nSize, pMemory)
	#define LT_MEM_EXTERNAL_FREE(nCategory, nSize, pMemory)
	#define LT_MEM_TRACK_SCOPE(nCategory)				
	#define LT_MEM_DISABLESTACK()						
	#define LT_MEM_ENABLESTACK()						
	#define LT_MEM_ONEXIT()								

#endif

#endif
