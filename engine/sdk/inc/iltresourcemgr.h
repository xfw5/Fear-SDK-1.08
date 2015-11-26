//-------------------------------------------------------------------------------------------
// ILTResourceMgr.h
//
// This interface provides game code control over how the resource manager operates, and
// allows the game code to setup, monitor, and provide hints to the resource manager
// in order to allow for optimal usage of resources.
//
//-------------------------------------------------------------------------------------------
#ifndef __ILTRESOURCEMGR_H__
#define __ILTRESOURCEMGR_H__

//for IBase and the other interface declaration macros
#ifndef __LTMODULE_H__
#	include "ltmodule.h"
#endif

//forward declarations
class ILTResourceAllocator;
class ILTResourceListener;

//Types used by the resource manager

//a handle to a streaming region, which contains an asset list, and also a memory limit on how large the
//assets within that list should be
class CStreamingRegion;
typedef const CStreamingRegion*		HSTREAMINGREGION;

//a handle to a list of assets that can be used for prefetching and discarding
class CStreamingAssetList;
typedef const CStreamingAssetList*	HREGIONASSETLIST;

//a handle to a resource bundle which is a collection of files stored as a single resource that can
//be loaded
class CResourceBundle;
typedef CResourceBundle*			HRESOURCEBUNDLE;

//a user asset list. These can be loaded from files and then prefetched and discarded to prefetch/discard
//all of their contents
class CUserAssetList;
typedef CUserAssetList*				HUSERASSETLIST;


class ILTResourceMgr :
	public IBase
{
public:

	//set the interface version number
	interface_version(ILTResourceMgr, 1);

	//allow derived classes to override the destructor
	virtual ~ILTResourceMgr()	{}

	//------------------------------------------
	// Initialization

	//this must be called within the game code's OnEngineInitialized function before any resources
	//are accessed. This is exposed to allow the game code to control whether or not background
	//resource loading should be used. No terminate needs to be called as the resource manager
	//will be shut down as part of normal engine shutdown.
	virtual LTRESULT	InitResourceSystem(bool bBackgroundLoading) = 0;

	//------------------------------------------
	// Allocators
	//
	// Provides interfaces for creating and managing the allocators that are supported by the
	// engine. Each one is implemented and accessed through the ILTResourceAllocator interface.
	
	//called to create a generic allocator. This will allocate until memory is exhausted
	virtual LTRESULT	CreateRandomAccessAllocator(ILTResourceAllocator*& pAllocator) = 0;

	//called to construct a block of memory allocator that contains two allocation heads, one that
	//grows up, another that grows down. These do not perform any memory freeing until all outstanding
	//allocations are performed, and this is  intended for use with resource bundles only. This can also
	//be allocated in a contiguous block and take an optional address, which can be set to zero to have the
	//address picked.
	virtual LTRESULT	CreateDualBlockAllocator(	uint32 nPoolSize, bool bContiguous, uint32 nStartAddress,
													ILTResourceAllocator*& pHeadAllocator, ILTResourceAllocator*& pTailAllocator) = 0;

	//------------------------------------------
	// Prefetching Systems
	//
	// The following functions are used in order to detect which resources are needed for
	// a given section of a level, and functionality is provided to allow for prefetching
	// of those assets so that they can be loaded into memory prior to when they are needed. It
	// is important to note that while these objects do not have reference counting associated
	// with them, they are only valid during the course of a single level, and therefore
	// must not be held across a level boundary.

	//support for iterating through streaming regions, this provides the total number of streaming
	//regions in the level
	virtual LTRESULT			GetNumStreamingRegions(uint32& nNumRegions) = 0;

	//support for iterating through streaming regions, this provides access to a streaming region
	//given an index in the range of [0..Num regions - 1], an invalid index will return NULL
	virtual HSTREAMINGREGION	GetStreamingRegion(uint32 nIndex) = 0;

	//given a point in space, this will return the streaming region that the point is contained
	//within, as well as the streaming region that is being linked to (which should start
	//prefetching). Note that both can be NULL if the point is outside all streaming regions,
	//and the linking region can be NULL if the point is in a streaming region but out of a linking
	//region
	virtual LTRESULT			GetPointStreamingRegions(	const LTVector& vPos, HSTREAMINGREGION& hRegion, 
															HSTREAMINGREGION& hLinked) = 0;

	//called to get the size that is specified in the level that should be used for the streaming region
	//memory pool size
	virtual LTRESULT			GetStreamingRegionPoolSize(uint32& nPoolSize) = 0;

	//called to get the size that is specified for the global assets memory pool
	virtual LTRESULT			GetGlobalPoolSize(uint32& nPoolSize) = 0;

	//called to obtain the list of global assets from a level. This can be NULL if no level is loaded
	virtual HREGIONASSETLIST	GetGlobalAssetList() = 0;

	//given a streaming region, this will obtain the memory limit that is associated with this streaming
	//region, which is useful for debugging
	virtual LTRESULT			GetStreamingRegionResourceLimit(HSTREAMINGREGION hRegion, uint32& nLimit) = 0;

	//given a streaming region, this will get the asset list associated with it. This can be NULL if
	//the provided region is invalid
	virtual HREGIONASSETLIST	GetStreamingRegionAssetList(HSTREAMINGREGION hRegion) = 0;

	//given a streaming region, this will copy the name into the provided buffer
	virtual LTRESULT			GetStreamingRegionName(HSTREAMINGREGION hRegion, char* pszBuffer, uint32 nBufferLen) = 0;

	//called to set the currently active streaming region that should have any assets added to it for
	//automatic asset list updating. Note that this will not do any prefetching for this region or anything
	//like that
	virtual LTRESULT			SetActiveRegion(HSTREAMINGREGION hRegion) = 0;

	//called to prefetch all of the files within an asset list. A corresponding discard must be 
	//performed to avoid a resource leak.
	virtual LTRESULT			PrefetchRegionAssetList(HREGIONASSETLIST hList) = 0;

	//called to discard all of the files within an asset list. This must not be called without a prior
	//prefetch call.
	virtual LTRESULT			DiscardRegionAssetList(HREGIONASSETLIST hList) = 0;

	//given a prefetch list, this will determine the total number of assets, the number that have
	//been loaded, and the amount of memory that these resources have consumed. This memory is
	//not exact, but should be quite close
	virtual LTRESULT			GetRegionAssetListStats(HREGIONASSETLIST hList, uint32& nNumLoaded, uint32& nTotalAssets, uint32& nMemUsage) = 0;
	
	//this function sets up the allocator that will be used for all prefetching and reloading requests.
	//If there has been no allocator set, or this is set to NULL, prefetching will not occur.
	virtual LTRESULT			SetPrefetchAllocator(ILTResourceAllocator* pAllocator) = 0;

	//called to enable automatic prefetching of assets when the resource handle is requested. This
	//means that all assets in a level will be loaded as early as possible, using more memory, but
	//less frequent hitches for assets that are not properly prefetched
	virtual LTRESULT			EnablePrefetchOnRequest(bool bEnable) = 0;

	//called to enable automatic prefetching of assets when the resource handle is locked. This
	//means that all assets in a level will be loaded only as needed. This causes more hitches in the
	//game, but is the only way to allow for streaming since prefetching on request would load all level
	//assets
	virtual LTRESULT			EnablePrefetchOnLock(bool bEnable) = 0;

	//called to flush any assets that were auto loaded. This will release the auto load handle, and
	//is useful when resources that were not properly prefetched through other means need to be discarded
	virtual LTRESULT			FlushAutoLoadedResources() = 0;

	//------------------------------------------
	// Streaming Status
	//
	// Various functions that are used to determine the status of the resource streaming system

	//this will return the number of files that are currently queued by the streaming system for
	//loading in the background. This is done on a background thread, so the actual number can
	//change at any time therefore this should only be used for monitoring purposes. This will
	//always be zero if background resource loading is not enabled.
	virtual LTRESULT			GetNumLoadingResources(uint32& nNumResources) = 0;


	//------------------------------------------
	// User Asset Lists
	//
	// This is a prefetching system that allows the game to load up asset lists from a file and handle
	// prefetching and discarding of the assets within that asset list

	//called to load a user asset list given a relative filename. This will return NULL on failure.
	virtual HUSERASSETLIST		LoadUserAssetList(const char* pszRelFilename) = 0;

	//called to prefetch the resources that are listed within a user asset list. This has no
	//effect if called multiple times without a matching discard
	virtual LTRESULT			PrefetchUserAssetList(HUSERASSETLIST hUserAssetList) = 0;

	//called to discard the resources that are listed within a user asset list. This should
	//be called to match the prefetch
	virtual LTRESULT			DiscardUserAssetList(HUSERASSETLIST hUserAssetList) = 0;

	//called to release the specified user asset list. The provided object must not be used
	//after this call.
	virtual LTRESULT			ReleaseUserAssetList(HUSERASSETLIST hUserAssetList) = 0;


	//------------------------------------------
	// Resource Bundles
	//
	// Resource bundles are collections of files stored in a bundle to allow for more efficient loading
	// and more predictable memory usage. Note that these resource bundles need to be loaded before
	// to the resources that they contain can be used.

	//given the name of a resource bundle, this will load it up into memory so that the resources
	//contained within can be used
	virtual HRESOURCEBUNDLE	LoadResourceBundle(const char* pszFilename, ILTResourceAllocator* pAllocator) = 0;

	//called to determine if the specified resource bundle is still loading
	virtual LTRESULT		IsResourceBundleLoading(HRESOURCEBUNDLE hBundle, bool& bLoading) = 0;

	//called to stop the loading of the resource bundle. Note that the effect of this may not
	//be instantanious
	virtual LTRESULT		StopResourceBundleLoading(HRESOURCEBUNDLE hBundle) = 0;

	//called to unload the specified resource bundle. This will not work if the resource bundle
	//is currently loading or has not been loaded, of if resources within the bundle are locked
	virtual LTRESULT		UnloadResourceBundle(HRESOURCEBUNDLE hBundle) = 0;

	//called to wait for the specified resoure bundle to finish loading. This will not return until
	//the provided resource bundle has finished loading (either through successful load, finishing
	//an aborted load, or through failing to load)
	virtual LTRESULT		WaitForResourceBundleToLoad(HRESOURCEBUNDLE hBundle) = 0;

	//------------------------------------------
	// Resource Listeners
	// 
	// Resource listeners implement the ILTResourceListener interface, and are given notifications
	// of certain events in the resource system. These events allow for the customization of how the
	// system operates

	//called to register a resource listener for events
	virtual LTRESULT		RegisterResourceListener(ILTResourceListener* pListener) = 0;

	//called to unregister a resource listener from the resource manager so it will stop receiving
	//events
	virtual LTRESULT		UnregisterResourceListener(ILTResourceListener* pListener) = 0;
};


#endif
