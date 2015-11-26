//------------------------------------------------------------------
//
//  FILE      : DynArray.h
//
//  PURPOSE   : Caching dynamic arrays used everywhere.
//
//  CREATED   : 5/1/96
//
//  COPYRIGHT : Microsoft 1996 All Rights Reserved
//
//------------------------------------------------------------------


#ifndef __DYNARRAY_H__
#define __DYNARRAY_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __STDLITHDEFS_H__
#include "stdlithdefs.h"
#endif

// Predefined types of arrays.
#define CMoDWordArray   CMoArray<uint32>
#define CMoByteArray    CMoArray<uint8>

// This can be used if you don't want the extra 4 bytes of caching info in the array.
class NoCache
{
public:
    uint32  GetCacheSize() const        { return 0; }
    void    SetCacheSize(uint32 size)   { LTUNREFERENCED_PARAMETER(size); }
    uint32  GetWantedCache() const      { return 0; }
    void    SetWantedCache(uint32 size) { LTUNREFERENCED_PARAMETER(size); }
};

class DefaultCache
{
public:
    uint32  GetCacheSize() const		{ return m_CacheSize; }
    void    SetCacheSize(uint32 val)    { m_CacheSize = val; }
    uint32  GetWantedCache() const		{ return m_WantedCache; }
    void    SetWantedCache(uint32 val)	{ m_WantedCache = val; }
 
private:        
    uint32  m_CacheSize;
    uint32  m_WantedCache;
};

template<class T, class C=DefaultCache>
class CMoArray
{
public:

    // Constructors                 
    CMoArray(CMoArray<T, C> &copyFrom, const T &toAdd)
    {
        Clear();
        Init();
        CopyArray(copyFrom);
        Append(toAdd);
    }

    CMoArray()
    { 
        Clear();
        Init(); 
    }
    
    CMoArray(uint32 cacheSize)
    {
        Clear();
        Init(0, cacheSize); 
    }
                
    // Destructor
    ~CMoArray() { Term(); }
    
    // Member functions
    bool			Init(uint32 size = 0, uint32 cacheSize = 0);
    void			Term() { SetSize(0); }

    // Assignment
    
    // You should use CopyArray whenever possible.
    CMoArray<T, C>& operator=(const CMoArray<T, C> &other);
    CMoArray<T, C>  operator+(const T &toAdd);
    
    
    bool			CopyArray(const CMoArray<T, C> &other);

    bool			AppendArray(const CMoArray<T, C> &other);

    // Find and compare with T::operator==.
    uint32			FindElement(const T x) const;
    
    bool			Append(const T &toAdd) { return Insert(m_nElements, toAdd); }
    bool			Insert(uint32 index, const T &toInsert);
    void			Remove(uint32 index);

    // You can use it like a stack with these...
    bool			Push(const T &toAdd)		{ return Append(toAdd); }
    void			Pop()						{ LTASSERT(m_nElements > 0, "Invalid Array Access"); Remove(m_nElements-1); }

    // Helpers for if you want to wrap around...
    T&              Last()      const   { LTASSERT(m_nElements > 0, "Invalid Array Access"); return m_pArray[ m_nElements-1 ]; }
    uint32          LastI()     const   { LTASSERT(m_nElements > 0, "Invalid Array Access"); return m_nElements-1; }
    
    T&              Next(uint32 index) const { return m_pArray[ NextI(index) ]; }
    T&              Prev(uint32 index) const { return m_pArray[ PrevI(index) ]; }

    uint32          NextI(uint32 i) const
    {
        LTASSERT(m_nElements > 0, "Invalid Array Access");
        if (i < (m_nElements-1))
            return i+1;
        else
            return 0;
    }

    uint32          PrevI(uint32 i) const
    {
        LTASSERT(m_nElements > 0, "Invalid Array Access");
        if (i == 0)
            return m_nElements - 1;
        else
            return i-1;
    }

    // Number of elements
    operator uint32() const { return (uint32)m_nElements;}

    // Array-like access.
    T&              operator[](const uint32 index) const { return Get(index); }
    
    // Returns false if there isn't enough memory.
    bool		  SetSize(uint32 newSize);

    bool          SetSizeInit(uint32 newSize, const T &val);

    // Same as SetSize but preserves the old contents (ie: sizing from 8 to 4 preserves
    // the first 4 and sizing from 4 to 8 preserves the first 4).
    bool          NiceSetSize(uint32 newSize)						{return InternalNiceSetSize(newSize, false);}
    
    // Same as NiceSetSize, but uses memcpy instead of operator=.
    bool          Fast_NiceSetSize(uint32 newSize)                  {return InternalNiceSetSize(newSize, true);}
	
    uint32        GetSize() const { return m_nElements; }

    // Sets the cache size
    void  SetCacheSize(uint32 size)
    {
        m_Cache.SetWantedCache(size);
    }

    // Get and set
    T& Get(uint32 index) const
    {
        ASSERT(index < m_nElements);
        return m_pArray[index];
    }

    void Set(uint32 index, T &data)
    {
        ASSERT(index < m_nElements);
        m_pArray[index] = data;
    }

    // Returns a pointer to the internal array..
    T*              GetArray()			{ return m_pArray; }
	const T*		GetArray() const	{ return m_pArray; }

    void            RemoveAll()         { SetSize(0); }
    bool            Add(const T &toAdd) { return Insert(m_nElements, toAdd); }

private:

    void    _InitArray(uint32 wantedCache);
    void    _DeleteAndDestroyArray();
    T*      _AllocateTArray(uint32 nElements);

    bool	InternalNiceSetSize(uint32 newSize, bool bFast);
    uint32  GetNumAllocatedElements() const   {return m_nElements + m_Cache.GetCacheSize();}

    void    Clear()
    {
        m_pArray = 0;
        m_nElements = 0;
        m_Cache.SetCacheSize(0);
        m_Cache.SetWantedCache(0);
    }

    // Member variables
    
    T       *m_pArray;
    uint32  m_nElements;

    C       m_Cache;
};


template<class T, class C>
bool CMoArray<T, C>::Init(uint32 size, uint32 cacheSize)
{
    Term();
    _InitArray(cacheSize);
    
    return SetSize(size);
}

template<class T, class C>
CMoArray<T, C>& CMoArray<T, C>::operator=(const CMoArray<T, C> &other)
{
    CopyArray(other);
    return *this;
}


template<class T, class C>
CMoArray<T, C> CMoArray<T, C>::operator+(const T &toAdd)
{
    return CMoArray<T, C>(*this, toAdd);
}


template<class T, class C>
bool CMoArray<T, C>::CopyArray(const CMoArray<T,C> &other)
{
    if (m_pArray)
    {
        _DeleteAndDestroyArray();
    }

    m_nElements = other.m_nElements;
    m_Cache.SetCacheSize(other.m_Cache.GetCacheSize());
    m_Cache.SetWantedCache(other.m_Cache.GetWantedCache());

    if (m_nElements + m_Cache.GetCacheSize() > 0)
    {
        m_pArray = _AllocateTArray(m_nElements + m_Cache.GetCacheSize());
    }
    else
    {
        m_nElements = 0;
        m_Cache.SetCacheSize(0);
        m_pArray = NULL;
        return true;
    }

    // Could it allocate the array?
    if (!m_pArray)
    {
        m_nElements = 0;
        m_Cache.SetCacheSize(0);
        return false;
    }

    for (uint32 i=0; i < m_nElements; i++)
        m_pArray[i] = other.m_pArray[i];

    return true;
}

template<class T, class C>
bool CMoArray<T, C>::AppendArray(const CMoArray<T, C> &other)
{
    for (uint32 i=0; i < other; i++)
        if (!Append(other[i]))
            return false;

    return true;
}


template<class T, class C>
uint32 CMoArray<T, C>::FindElement(const T x) const
{
    uint32 ret = BAD_INDEX;

    for (uint32 i=0; i < m_nElements; i++)
    {
        if (m_pArray[i] == x)
        {
            ret = i;
            break;
        }
    }

    return ret;
}



template<class T, class C>
bool CMoArray<T, C>::Insert(uint32 index, const T &toInsert)
{
	ASSERT(index <= m_nElements);
	if (index > m_nElements)
		return false;

	// Create a new array (possibly).
	uint32 newSize = m_nElements + 1;

	//if(newSize >= (m_nElements+m_CacheSize) || m_nElements == 0)
	if (m_Cache.GetCacheSize() == 0)
	{
		T *pNewArray = _AllocateTArray(newSize + m_Cache.GetWantedCache());
		if (!pNewArray)
			return false;

		// Copy the old array into the new one, start inserting at index.
		uint32 i;
		for (i=0; i < index; i++)
			pNewArray[i] = m_pArray[i];

		for (i=index; i < m_nElements; i++)
			pNewArray[i+1] = m_pArray[i];

		// Insert the new item into the array
		pNewArray[index] = toInsert;

		// Free the old array and set our pointer to the new one
		if (m_pArray)
		{
			_DeleteAndDestroyArray();
		}

		m_Cache.SetCacheSize(m_Cache.GetWantedCache());
		m_pArray = pNewArray;
	}
	else
	{
		for (uint32 i=m_nElements; i > index; i--)
			m_pArray[i] = m_pArray[i-1];

		m_Cache.SetCacheSize(m_Cache.GetCacheSize() - 1);

		m_pArray[index] = toInsert;
	}

	++m_nElements;      

	return true;
}

template<class T, class C>
void CMoArray<T, C>::Remove(uint32 index)
{
	ASSERT(index < m_nElements && m_pArray);

	bool bSlideDown = true;
	if (m_Cache.GetCacheSize() >= (m_Cache.GetWantedCache()*2))
	{
		uint32 newSize = m_nElements - 1;
		uint32 newAllocSize = newSize + m_Cache.GetWantedCache();
		T *pNewArray = _AllocateTArray(newAllocSize);

		// Make sure it allocated the array .. if it didn't, just have
		// it slide all the elements down (this guarantees that Remove() 
		// won't fail.)
		if (pNewArray || newAllocSize == 0)
		{
			uint32 i;
			for (i=0; i < index; i++)
				pNewArray[i] = m_pArray[i];

			for (i=index; i < m_nElements-1; i++)
				pNewArray[i] = m_pArray[i+1];

			_DeleteAndDestroyArray();
			m_pArray = pNewArray;

			m_Cache.SetCacheSize(m_Cache.GetWantedCache());
			bSlideDown = false;
		}
	}


	if (bSlideDown)
	{
		// Slide them all down one.
		m_Cache.SetCacheSize(m_Cache.GetCacheSize() + 1);

		for (uint32 i=index; i < m_nElements-1; i++)
			m_pArray[i] = m_pArray[i+1];
	}

	--m_nElements;
}


template<class T, class C>
bool CMoArray<T, C>::SetSize(uint32 newSize)
{
	// If they request the current settings, there's no need to change
	if ((newSize == m_nElements) && (newSize != 0))
	{
		return true;
	}

	if (m_pArray)
	{
		_DeleteAndDestroyArray();
	}

	m_nElements = newSize;
	if (newSize > 0)
	{
		m_pArray = _AllocateTArray(newSize + m_Cache.GetWantedCache());
		if (!m_pArray)
		{
			m_nElements = 0;
			m_Cache.SetCacheSize(0);
			return false;
		}

		m_Cache.SetCacheSize(m_Cache.GetWantedCache());
	}

	return true;
}

template<class T, class C>
bool CMoArray<T, C>::SetSizeInit(uint32 newSize, const T &val)
{
	if (!SetSize(newSize))
		return false;

	for (uint32 i=0; i < GetSize(); i++)
	{
		m_pArray[i] = val;
	}

	return true;
}

template<class T, class C>
bool CMoArray<T, C>::InternalNiceSetSize(uint32 newSize, bool bFast)
{
    // Trivial reject..
    if (newSize < m_nElements)
    {
        m_Cache.SetCacheSize(m_Cache.GetCacheSize() + (m_nElements - newSize));
        m_nElements = newSize;
        return true;
    }
    else if (newSize > m_nElements && (m_nElements + m_Cache.GetCacheSize()) >= newSize)
    {
        m_Cache.SetCacheSize(m_Cache.GetCacheSize() - (newSize - m_nElements));
        m_nElements = newSize;
        return true;
    }
    else if (newSize == m_nElements)
    {
        // uhhh ok..
        return true;
    }

    T *pNewArray = _AllocateTArray(newSize + m_Cache.GetWantedCache());
    if (!pNewArray)
        return false;

    uint32 nToCopy = m_nElements;
    if (nToCopy > newSize)
        nToCopy = newSize;

    // Copy as many elements as we can.
    if (bFast)
    {
        memcpy(pNewArray, m_pArray, sizeof(T)*nToCopy);
    }
    else
    {
        for (uint32 i=0; i < nToCopy; i++)
        {
            pNewArray[i] = m_pArray[i];
        }
    }
    
    // Get rid of the old array and point at the new one.
    _DeleteAndDestroyArray();
    m_pArray = pNewArray;
    m_nElements = newSize;
    m_Cache.SetCacheSize(m_Cache.GetWantedCache());

    return true;
}


template<class T, class C>
void CMoArray<T, C>::_InitArray(uint32 wantedCache)
{
    m_pArray = NULL;
    m_nElements = 0;

    m_Cache.SetWantedCache(wantedCache);
    m_Cache.SetCacheSize(0);
}



template<class T, class C>
T *CMoArray<T, C>::_AllocateTArray(uint32 nElements)
{
    T *tPtr = new T [nElements];
    return tPtr;
}

template<class T, class C>
void CMoArray<T, C>::_DeleteAndDestroyArray()
{
    if (m_pArray)
    {
		delete [] m_pArray;
        m_pArray = NULL;
    }

    m_Cache.SetCacheSize(0);
}

template<class T, class V>
void SetArray(T &theArray, V val)
{
    for (uint32 i=0; i < theArray; i++)
        theArray[i] = val;
}

template<class T>
void DeleteAndClearArray(T &theArray)
{
	for (uint32 i=0; i < theArray.GetSize(); i++)
	{
		if (theArray[i])
		{
			delete theArray[i];
		}
	}

	theArray.SetSize(0);
}


#endif 




