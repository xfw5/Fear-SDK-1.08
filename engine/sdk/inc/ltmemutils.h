//------------------------------------------------------------------
// LTMemUtils.h
//
// Defines utilities for using the memory tracking libraries
//
//------------------------------------------------------------------

#ifndef __LTMEMUTILS_H__
#define __LTMEMUTILS_H__

#include "iltmemory.h"

// A default allocator for use in STL containers so the memory allocations will go
// into a specific category.  (Although the file/line info will still end up in
// this file.)
template <class T, int CATEGORY>
class LTAllocator : public std::allocator<T>
{
public:
	// Convenience typedefs
	typedef std::allocator<T> TParent;
	typedef LTAllocator<T, CATEGORY> TSelf;

	// Allocator typedefs
	typedef typename TParent::pointer pointer;
	typedef typename TParent::reference reference;
	typedef typename TParent::const_pointer const_pointer;
	typedef typename TParent::const_reference const_reference;

	typedef typename TParent::size_type size_type;
	typedef typename TParent::difference_type difference_type;

	// Rebind structure
	template<class _Other>
	struct rebind
	{
		typedef LTAllocator<_Other, CATEGORY> other;
	};

	// Re-define the allocation constructors and etc..
	LTAllocator() {}
	LTAllocator(const TSelf&) {}

	template<class _Other>
	LTAllocator(const LTAllocator<_Other, CATEGORY>&) {}

	template<class _Other>
	TSelf& operator=(const LTAllocator<_Other, CATEGORY>&) { return *this; }

	// Wrap the allocate function with the memory category
	pointer allocate(size_type _Count) {
		pointer result;
		LT_MEM_TRACK_ALLOC(result = TParent::allocate(_Count), CATEGORY);
		return result;
	}

	pointer allocate(size_type _Count, const void *) { return (allocate(_Count)); }
};

// Function for aligning a memory size to a minimum granularity.
// This is useful for determining the size of an allocation without breaking
// memory alignment constraints.
// Note: Memory alignment sizes are assumed to be a power of two by this function
// Usage example:
//		nMyMemorySize = LTMemSizeAlign(nMyMemorySize, sizeof(uint32));
inline uint32 LTMemSizeAlign(uint32 nMemorySize, uint32 nDataAlignment)
{
	LTASSERT((nDataAlignment & (nDataAlignment - 1)) == 0, "Non-power of two data alignment encountered");
	return (nMemorySize + (nDataAlignment - 1)) & ~(nDataAlignment - 1);
}

#endif
