
#ifndef __STRUCT_BANK_H__
#define __STRUCT_BANK_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __MEMORY_H__
#include <memory.h>
#define __MEMORY_H__
#endif

typedef struct StructLink_t {
    struct StructLink_t *m_pSLNext;
} StructLink;

typedef struct StructBankPage_t {
    struct StructBankPage_t *m_pNext;
    uint32			m_nObjects; // How many objects are in this page?
    uint32          m_Data[1];
} StructBankPage;

typedef struct StructBank_t {
    // Struct size.
    uint32 m_StructSize;
    
    // The actual size we allocate with (aligned to 4 bytes and with 4 bytes
    // extra for the StructLink).
    uint32 m_AlignedStructSize;
    
    // How many structs per page.
    uint32 m_CacheSize;

    // How many pages have we allocated?
    uint32 m_nPages;
    
    // Total number of objects this StructBank has allocated.
    uint32 m_nTotalObjects;

    // The first page.
    StructBankPage *m_PageHead;

    // The free list.
    StructLink *m_FreeListHead;
} StructBank;


// You can use this to declare StructBanks.. it'll make it safe to
// call sb_Term even if you haven't called sb_Init yet.
#define DECLARE_STRUCTBANK(name) \
    StructBank name = {0, 0, 0, 0, 0};

// Internal.. don't call this.
bool sb_AllocateNewStructPage(StructBank *pBank, uint32 nAllocations);

// Initialize.. pass in the size of your structure and how much to cache.
void sb_Init(StructBank *pBank, uint32 structSize, uint32 cacheSize);
	
// Initialize and preallocate some (to avoid allocation the first time).
// Returns 1 if successful, 0 otherwise.  Even if it returns 0, the StructBank
// will be initialized and you might be able to allocate thru it.
bool sb_Init2(StructBank *pBank, uint32 structSize, uint32 cacheSize, uint32 nPreallocations);

// This puts everything that has been allocated into the free list.  It doesn't 
// actually free any memory.  It can be useful if you want easy access to a bunch
// of StructBank'd structures and don't want to worry about freeing them all individually.
void sb_FreeAll(StructBank *pBank);
	
// Shutdown.
void sb_Term(StructBank *pBank);

//called to determine the number of free objects in this struct bank
uint32 sb_GetFreeObjectCount(const StructBank* pBank);

//called to determine the total number of objects in this bank
uint32 sb_GetTotalObjectCount(const StructBank* pBank);

// Allocate an instance.
inline void* sb_Allocate(StructBank *pBank) 
{
    StructLink *pRet = pBank->m_FreeListHead;
    if (!pRet) 
	{
        sb_AllocateNewStructPage(pBank, pBank->m_CacheSize);
        pRet = pBank->m_FreeListHead;
        if (!pRet)
            return NULL;
    }

    pBank->m_FreeListHead = pRet->m_pSLNext;
    return pRet;
}

// Allocate a zero-initted instance.    
inline void* sb_Allocate_z(StructBank *pBank) 
{
    StructLink *pRet = pBank->m_FreeListHead;
    if (!pRet) 
	{
        sb_AllocateNewStructPage(pBank, pBank->m_CacheSize);
        pRet = pBank->m_FreeListHead;
        if (!pRet)
            return NULL;
    }

    pBank->m_FreeListHead = pRet->m_pSLNext;
    memset(pRet, 0, pBank->m_StructSize);
    return pRet;
}

inline void sb_Free(StructBank *pBank, void *pObj) 
{
    StructLink *pLink = (StructLink*)pObj;

    #ifndef _FINAL
		memset(pObj, 0xEA, pBank->m_StructSize);
    #endif

    pLink->m_pSLNext = pBank->m_FreeListHead;
    pBank->m_FreeListHead = pLink;
}

#endif












