//----------------------------------------------------------------------------
// LTStlUtils.h
//
// Provides utilities for dealing with STL idiosyncrasies.  All contained 
// utilities are located in the "ltstd" namespace.
//
//----------------------------------------------------------------------------
#ifndef __LTSTLUTILS_H__
#define __LTSTLUTILS_H__

#include <vector>

namespace ltstd
{

/* 
	Functor object allowing deletion of the provided object.
	This is very useful for deleting objects in a container that were allocated
	using the new operator.

	Example:
		std::vector<CFoo*> aMyVector;
		...
		std::for_each(aMyVector.begin(), aMyVector.end(), ltstd::delete_object());
*/
struct delete_object
{
	template <class T>
	void operator()(const T *obj) const
	{
		delete obj;
	}
};

/*
	Function for freeing the memory associated with a vector.
	Note that this function is provided as a means of dealing with the differences 
	associated with various STL implementations in relation to the handling of
	vector::clear.  Some implementations will free the memory associated with the
	vector, while other implementations will leave underlying memory intact.  This
	implementation will be guaranted to free the associated memory.

	Example:
		std::vector<CFoo> aMyVector;
		...
		ltstd::free_vector(aMyVector);
		// Memory freed
*/
template <class T>
void free_vector(std::vector<T> &vec)
{
	vec.swap(std::vector<T>());
}

/*
	Function for resetting a vector without freeing the associated memory.
	Note that this function is provided as a means of dealing with the differences 
	associated with various STL implementations in relation to the handling of
	vector::clear.  Some implementations will free the memory associated with the
	vector, while other implementations will leave underlying memory intact.  This
	implementation will be guaranted NOT to free the associated memory.

	Example:
		std::vector<CFoo> aMyVector;
		...
		ltstd::reset_vector(aMyVector);
		// Memory not freed
*/
template <class T>
void reset_vector(std::vector<T> &vec)
{
	vec.resize(0);
}

} // namespace ltstd

#endif
