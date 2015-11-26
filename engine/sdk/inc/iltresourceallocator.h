//------------------------------------------------------------------------------------------------
// ILTResourceAllocator.h
// 
// This is a very simple interface that allocators for the resource system must implement. It
// contains standard allocation functionality as well as some basic statistical functionality
//
//------------------------------------------------------------------------------------------------

#ifndef __ILTRESOURCEALLOCATOR_H__
#define __ILTRESOURCEALLOCATOR_H__

//for the uint* definitions
#ifndef __LTINTEGER_H__
#	include "ltinteger.h"
#endif

//for the CLTReference definition
#ifndef __ILTREFCOUNT_H__
#	include "iltrefcount.h"
#endif


class ILTResourceAllocator
{
public:

	//overridable destructor
	virtual ~ILTResourceAllocator()	{}

	//reference counting support
	virtual void AddRef() = 0;
	virtual void Release() = 0;

	//this function is called to perform the actual allocation of memory. This will allocate at least
	//the specified number of bytes requested. This can fail however, and upon failure will return
	//NULL. 
	virtual void*		Allocate(uint32 nNumBytes) = 0;

	//this function is called to release the provided pointer. This pointer must have been obtained
	//through Allocate, and it must only be freed once. This operates on void* to allow
	//the taking of any pointer type without explicit conversion.
	virtual void		Free(void* pBuffer) = 0;

	//this function is called to perform allocation of a physically contiguous block of memory. This
	//is only supported through certain allocators on certain platforms, so it can fail. If it does
	//succeed, it guarantees that at least the specified number of bytes have been allocated in physical
	//address space. This must be matched with a FreeContiguous call, and must not be Free'd
	virtual void*		AllocateContiguous(uint32 nNumBytes) = 0;

	//this function is called to release the provided pointer. This pointer must have been obtained
	//through AllocateContiguous, and it must only be freed once. This operates on void* to allow
	//the taking of any pointer type without explicit conversion.
	virtual void		FreeContiguous(void* pBuffer) = 0;

	//this function is called to determine how much memory has been allocated by this allocator. This
	//value should only be used for debugging purposes as this value can change unexpectedly due to 
	//allocations that may be done on a separate thread
	virtual uint32		GetAllocatedMemSize() = 0;
};

//provide an auto pointer type that can be used to help manage references to resource allocators
typedef CLTReference<ILTResourceAllocator>	TLTResourceAllocatorRef;


#endif
