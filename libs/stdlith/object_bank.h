
// These work just like StructBanks but are for C++ objects that need their
// constructors and destructors called.

#ifndef __OBJECT_BANK_H__
#define __OBJECT_BANK_H__

#ifndef __STRUCT_BANK_H__
#include "struct_bank.h"
#endif

#ifndef __ILTMEMORY_H__
#include "iltmemory.h"
#endif

#define DEFAULT_OBJECT_CACHE_SIZE 32

class NullCS {
public:
    void Enter() {}
    void Leave() {}
};


// This is so you can give someone a BaseObjectBank pointer and assume 
// it will allocate the appropriate type of object.
class BaseObjectBank {
public:
    virtual ~BaseObjectBank() {}

    virtual void *AllocVoid()=0;
    virtual void FreeVoid(void *ptr)=0;
    virtual void Term()=0;
};


template<class T, int CATEGORY, class CS=NullCS>
class ObjectBank : public BaseObjectBank
{
public:

    ObjectBank();
    ObjectBank(uint32 cacheSize, uint32 preAllocate=0);
    virtual ~ObjectBank();

    void Init(uint32 cacheSize, uint32 preAllocate=0);

    // Set the cache size (in numbers of objects).  Default is DEFAULT_OBJECT_CACHE_SIZE.
    void SetCacheSize(uint32 size);

    T *Allocate();
    void Free(T *pObj);

    // Returns TRUE if the object is currently allocated.
    // Only returns FALSE in debug mode..
    LTBOOL IsObjectAllocated(T *pObj);


// Overrides.
public:

    virtual void *AllocVoid() {
        return Allocate();
    }

    virtual void FreeVoid(void *ptr) {
        Free((T*)ptr);
    }

    virtual void Term();


public:
    StructBank  m_Bank;
    CS          m_CS;
};


template<class T, int CATEGORY, class CS>
inline ObjectBank<T, CATEGORY, CS>::ObjectBank() {
	LT_MEM_TRACK_ALLOC(sb_Init(&m_Bank, sizeof(T), DEFAULT_OBJECT_CACHE_SIZE), CATEGORY);
}

template<class T, int CATEGORY, class CS>
inline ObjectBank<T, CATEGORY, CS>::ObjectBank(uint32 cacheSize, uint32 preAllocate) {
    LT_MEM_TRACK_ALLOC(sb_Init2(&m_Bank, sizeof(T), cacheSize, preAllocate), CATEGORY);
}

template<class T, int CATEGORY, class CS>
inline ObjectBank<T, CATEGORY, CS>::~ObjectBank() {
    sb_Term(&m_Bank);
}

template<class T, int CATEGORY, class CS>
inline void ObjectBank<T, CATEGORY, CS>::Init(uint32 cacheSize, uint32 preAllocate) {
    m_CS.Enter();
        sb_Term(&m_Bank);
        LT_MEM_TRACK_ALLOC(sb_Init2(&m_Bank, sizeof(T), cacheSize, preAllocate), CATEGORY);
    m_CS.Leave();
}

template<class T, int CATEGORY, class CS>
inline void ObjectBank<T, CATEGORY, CS>::Term() {
    m_CS.Enter();
        sb_Term(&m_Bank);
        LT_MEM_TRACK_ALLOC(sb_Init(&m_Bank, sizeof(T), DEFAULT_OBJECT_CACHE_SIZE), CATEGORY);
    m_CS.Leave();
}

template<class T, int CATEGORY, class CS>
inline void ObjectBank<T, CATEGORY, CS>::SetCacheSize(uint32 size) {
    m_CS.Enter();
        m_Bank.m_CacheSize = size;
    m_CS.Leave();
}

template<class T, int CATEGORY, class CS>
inline T* ObjectBank<T, CATEGORY, CS>::Allocate() {
    m_CS.Enter();
		T *pRet;
        LT_MEM_TRACK_ALLOC(pRet = (T*)sb_Allocate(&m_Bank), CATEGORY);
        if (pRet)
        {
            ::new (pRet) T;
        }
    m_CS.Leave();

    return pRet;
}

template<class T, int CATEGORY, class CS>
inline void ObjectBank<T, CATEGORY, CS>::Free(T *pObj) {
    m_CS.Enter();
        pObj->~T();
        sb_Free(&m_Bank, pObj);
    m_CS.Leave();
}


#endif  // __OBJECT_BANK_H__




