// *********************************************************************** //
//
// MODULE  : linux_stlcompat.h
//
// PURPOSE : Linux STL compatibility file.
//
// CREATED : 06/05/04
//
// (c) 2004 Monolith Productions, Inc.  All Rights Reserved
//
// *********************************************************************** //

#ifndef __LINUX_STLCOMPAT_H__
#define __LINUX_STLCOMPAT_H__

#include <ext/hash_map>

namespace std
{

// hash_compare class for compatibility with MS hash table
template<class _Kty, class _Pr = std::less<_Kty> >
class hash_compare
{	
public:
	enum 
	{
		bucket_size = 4,	// 0 < bucket_size
		min_buckets = 8		// min_buckets = 2 ^^ N, 0 < N
	};
	
	hash_compare() : comp() {}
	hash_compare(_Pr _Pred) : comp(_Pred) {}

	size_t operator()(const _Kty& _Keyval) const
	{
		// hash _Keyval to size_t value
		return ((size_t)_Keyval);
	}

	bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
	{	
		// test if _Keyval1 ordered before _Keyval2
		return (comp(_Keyval1, _Keyval2));
	}

private:

	// the comparator object
	_Pr comp;	
};

// unsigned char template specialization of char_traits
 template<>
    struct char_traits<unsigned char>
    {
      typedef unsigned char     char_type;
      typedef unsigned int      int_type;
      typedef streamoff         off_type;
      typedef streampos         pos_type;
      typedef mbstate_t         state_type;
       
      static void
      assign(char_type& __c1, const char_type& __c2)
      { __c1 = __c2; }
 
      static bool
      eq(const char_type& __c1, const char_type& __c2)
      { return __c1 == __c2; }
 
      static bool
      lt(const char_type& __c1, const char_type& __c2)
      { return __c1 < __c2; }
 
      static int
      compare(const char_type* __s1, const char_type* __s2, size_t __n)
      { return memcmp(__s1, __s2, __n); }
 
      static size_t
      length(const char_type* __s)
      { return strlen((const char*)__s); }
 
      static const char_type*
      find(const char_type* __s, size_t __n, const char_type& __a)
      { return static_cast<const char_type*>(memchr(__s, __a, __n)); }
 
      static char_type*
      move(char_type* __s1, const char_type* __s2, int_type __n)
      { return static_cast<char_type*>(memmove(__s1, __s2, __n)); }
 
      static char_type*
      copy(char_type* __s1, const char_type* __s2, size_t __n)
      { return static_cast<char_type*>(memcpy(__s1, __s2, __n)); }
                                                                                  
      static char_type*
      assign(char_type* __s, size_t __n, char_type __a)
      { return static_cast<char_type*>(memset(__s, __a, __n)); }
                                                                                  
      static char_type
      to_char_type(const int_type& __c) { return char_type(__c); }
                                                                                  
      static int_type
      to_int_type(const char_type& __c) { return int_type(__c); }
                                                                                  
      static bool
      eq_int_type(const int_type& __c1, const int_type& __c2)
      { return __c1 == __c2; }
                                                                                  
      static int_type
      eof() { return static_cast<int_type>(EOF); }
                                                                                  
      static int_type
      not_eof(const int_type& __c)
      { return eq_int_type(__c, eof()) ? 0 : __c; }
  };

} // namespace std

namespace stdext
{
	// bring hash_compare into the stdext namespace
	using std::hash_compare;
	
	// compatibility class for hash_map - remaps the template arguments to the
	// Linux version of the class.
	template<class TKey,
			 class TValue,
			 class THashCompare,
			 class TAlloc = std::allocator<TValue> >
	class hash_map : public __gnu_cxx::hash_map<TKey, TValue, THashCompare, THashCompare, TAlloc>
	{
	};
}

#endif // __LINUX_STLCOMPAT_H__
