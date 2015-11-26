//-----------------------------------------------------------------------------------
// ILTResourceListener.h
//
// This is the interface that defines what events listeners of the resource manager
// system can respond to
//
//-----------------------------------------------------------------------------------

#ifndef __ILTRESOURCELISTENER_H__
#define __ILTRESOURCELISTENER_H__

//for the CLTReference definition
#ifndef __ILTREFCOUNT_H__
#	include "iltrefcount.h"
#endif

class ILTResourceListener
{
public:

	//allow derived classes to destruct
	virtual ~ILTResourceListener()	{}

	//reference counting support
	virtual void	AddRef() = 0;
	virtual void	Release() = 0;

	//called whenever a resource handle is requested for a resource that is not prefetched. This
	//is commonly used to perform prefetching of the asset if all the prefetching is meant to be
	//done ahead of time
	virtual void	OnObtainUnprefetchedResource(const char* pszResource) = 0;

	//called whenever an unprefetched resource is locked. It is very important to note that this will NOT
	//be called during final builds for performance. Typically this is used for updating of
	//asset lists dynamically.
	virtual void	OnUnprefetchedResourceLocked(const char* pszResource) = 0;

	//called whenever a resource is locked that is prefetched, but has not yet been loaded. This is
	//typically used to indicate that the prefetching needs to be setup in a more optimal manner. This
	//will NOT be called in final builds for performance reasons
	virtual void	OnUnloadedResourceLocked(const char* pszResource) = 0;
};

//provide an auto pointer type that can be used to help manage references to resource listeners
typedef CLTReference<ILTResourceListener>	TLTResourceListenerRef;


#endif
