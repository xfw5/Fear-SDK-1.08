//------------------------------------------------------------------------------------------------
// ILTResourceLoader.h
//
// This interface defines what functionality all resource loaders must support. Each resource
// loader serves the role of taking an input stream, and converting the stream into a usable
// resource.
//
// Resource construction is done in two parts. The first part is the resource loading and can
// be done potentially on a background thread. This phase involves taking the provided stream
// and converting it into a usable form. The second phase is always done in the main thread
// and is called finalization. This finalization phase allows for attaching resource data to
// actual resource handles, and also performing any operations that must be performed on the
// main thread (such as rendering resources, sound resources, etc.)
//
// Note that resource loaders really should not perform any memory allocation except through the
// memory allocator that is provided.
//
// Resource loaders must also be able to free the specified resources, either before finalization
// or after.
//
//------------------------------------------------------------------------------------------------

#ifndef __ILTRESOURCELOADER_H__
#define __ILTRESOURCELOADER_H__

//for the CLTReference definition
#ifndef __ILTREFCOUNT_H__
#	include "iltrefcount.h"
#endif

//forward declarations
class ILTResourceAllocator;
class ILTInStream;

class ILTResourceLoader
{
public:

	//allow overriding of the destructor
	virtual ~ILTResourceLoader()	{}

	//reference counting support
	virtual void AddRef() = 0;
	virtual void Release() = 0;

	//given an input stream, this will handle loading the input stream and converting it to a
	//resource that can be used for finalization. In addition, an allocator is provided to
	//handle any memory allocation needs that this resource should need. Upon success, this should
	//return a pointer to the object that should be loaded. Upon failure NULL should be returned.
	//Both input parameters are guaranteed to exist and be valid, therefore they are passed by reference.
	//It is very important to note that this can be called from a thread other than the main thread.
	virtual void*	LoadResource(ILTInStream& InStream, ILTResourceAllocator& Allocator) = 0;

	//when a resource is ready to be finalized, this function will be called from the main thread. This
	//provides the pointer that was returned from LoadResource as long as it was not NULL. In addition
	//it will provide the same allocator that was given to the LoadResource function. In response to this
	//call, the loader should perform any steps necessary to get the resource into a usable state that
	//requires the main thread. If this cannot be done, NULL should be returned indicating failure, otherwise
	//the finalized resource should be returned. If this fails, this function is responsible for freeing
	//the provided loaded resource.
	virtual void*	FinalizeResource(void* pLoadedResource, ILTResourceAllocator& Allocator, const char* pszFilename) = 0;

	//this function is called to destroy a resource that has not yet reached the finalization state. This
	//takes in the pointer that was returned from LoadResource as well as the allocator that was provided.
	virtual void	FreeUnfinalizedResource(void* pLoadedResource, ILTResourceAllocator& Allocator) = 0;

	//this function is called to destory a resource that has undergone the finalization process. This
	//takes in the pointer that was returned from the FinalizeResource function as well as the allocator
	//provided to that function
	virtual void	FreeFinalizedResource(void* pFinalizedResource, ILTResourceAllocator& Allocator) = 0;

	//this function is called to get an approximated memory usage from the resource. This does not need
	//to be completely exact, but should be as close as possible and if anything error on the size of
	//being too large. This will only ever be called on the main thread.
	virtual uint32	GetResourceMemUsage(void* pFinalizedResource) = 0;
};

//provide an auto pointer type that can be used to help manage references to resource loaders
typedef CLTReference<ILTResourceLoader>	TLTResourceLoaderRef;

#endif
