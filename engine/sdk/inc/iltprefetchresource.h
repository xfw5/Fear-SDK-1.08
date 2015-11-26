//----------------------------------------------------------------------------------
// ILTPrefetchResource.h
//
// This interface exists to allow for prefetching of a variety of resource types. If
// a resource is to be able to be prefetched through the asset system, it must implement
// this interface and register it with the resource system through ILTResourceMgr
//
//----------------------------------------------------------------------------------

#ifndef __ILTPREFETCHRESOURCE_H__
#define __ILTPREFETCHRESOURCE_H__

class ILTPrefetchResource
{
public:

	//allow for derived classes to destruct
	virtual ~ILTPrefetchResource()	{}

	//this is called when a resource needs to be prefetched
	virtual bool	PrefetchResource(const char* pszResource) = 0;

	//this is called to determine if a resource is currently prefetched. If this cannot
	//be determined, this should simply return false. This functionality is used to minimize
	//reloading of assets
	virtual bool	IsResourcePrefetched(const char* pszResource) = 0;

	//this is called when a pretch is no longer needed and therefore should be discarded
	virtual void	DiscardResource(const char* pszResource) = 0;

	//this is called to determine if a resource is loaded, and if it is, determine the amount of memory
	//that the resource is using. This value does not need to be exact, but should be as close as possible
	//for statistics tracking. If the asset is not loaded, it should return false
	virtual bool	GetResourceMemoryUsage(const char* pszResource, uint32& nMemUsage) = 0;
};

#endif
