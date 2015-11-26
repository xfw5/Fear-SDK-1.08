
#include "stdlith.h"
#include "struct_bank.h"


void sb_Init(StructBank *pBank, uint32 structSize, uint32 cacheSize)
{
	pBank->m_StructSize = structSize;
	
	pBank->m_AlignedStructSize = structSize;
	if(pBank->m_AlignedStructSize < sizeof(StructLink))
		pBank->m_AlignedStructSize = sizeof(StructLink);

	if(pBank->m_AlignedStructSize % 4 != 0)
	{
		pBank->m_AlignedStructSize += (4 - (pBank->m_AlignedStructSize % 4));
 	}
	
	pBank->m_nPages = 0;
	pBank->m_nTotalObjects = 0;
	pBank->m_CacheSize = cacheSize;
	pBank->m_PageHead = NULL;
	pBank->m_FreeListHead = NULL;
}


bool sb_Init2(StructBank *pBank, uint32 structSize, uint32 cacheSize, uint32 nPreallocations)
{
	sb_Init(pBank, structSize, cacheSize);
	return sb_AllocateNewStructPage(pBank, nPreallocations);
}


void sb_FreeAll(StructBank *pBank)
{
	pBank->m_FreeListHead = NULL;

	StructBankPage *pPage = pBank->m_PageHead;
	while(pPage)
	{
		#ifndef _FINAL
			memset(pPage->m_Data, 0xEA, pBank->m_AlignedStructSize * pPage->m_nObjects);
		#endif

		uint8 *pDataPos = (uint8*)pPage->m_Data;
		uint32 count = pPage->m_nObjects;
		while(count--)
		{
			StructLink *pStruct = (StructLink*)pDataPos;
			pStruct->m_pSLNext = pBank->m_FreeListHead;
			pBank->m_FreeListHead = pStruct;
			pDataPos += pBank->m_AlignedStructSize;
		}
		
		pPage = pPage->m_pNext;
	}
}


void sb_Term(StructBank *pBank)
{
	#ifndef _FINAL
		// Make sure everything is freed.
		uint32 freeListCount = sb_GetFreeObjectCount(pBank);
		LTASSERT(freeListCount == pBank->m_nTotalObjects, "Not all objects were properly freed from this struct or object bank");
	#endif

	StructBankPage *pPage = pBank->m_PageHead;
	while(pPage)
	{
		StructBankPage *pNext = pPage->m_pNext;
		delete [] (uint8*)pPage;
		pPage = pNext;
	}

	pBank->m_PageHead = NULL;
	pBank->m_FreeListHead = NULL;
	pBank->m_nPages = 0;
	pBank->m_nTotalObjects = 0;
}

//called to determine the number of free objects in this struct bank
uint32 sb_GetFreeObjectCount(const StructBank* pBank)
{
	uint32 nFreeListCount = 0;
	for(StructLink *pCur = pBank->m_FreeListHead; pCur; pCur = pCur->m_pSLNext)
	{
		++nFreeListCount;
	}

	return nFreeListCount;
}

//called to determine the total number of objects in this bank
uint32 sb_GetTotalObjectCount(const StructBank* pBank)
{
	return pBank->m_nTotalObjects;
}

bool sb_AllocateNewStructPage(StructBank *pBank, uint32 nAllocations)
{
	if(nAllocations == 0)
		return true;

	// Allocate a new page.
	uint32 nPageSize = (pBank->m_AlignedStructSize * nAllocations) + (sizeof(StructBankPage)-sizeof(uint32));
	StructBankPage *pPage = (StructBankPage*)(new uint8 [nPageSize]);
	if(!pPage)
		return false;

	pPage->m_pNext = pBank->m_PageHead;
	pPage->m_nObjects = nAllocations;

	pBank->m_PageHead = pPage;

	++pBank->m_nPages;
	pBank->m_nTotalObjects += nAllocations;

	// Put its contents into the free list.
	uint8 *pDataPos = (uint8*)pPage->m_Data;
	uint32 count = nAllocations;
	while(count--)
	{
		StructLink *pStruct = (StructLink*)pDataPos;
		pStruct->m_pSLNext = pBank->m_FreeListHead;
		pBank->m_FreeListHead = pStruct;
		pDataPos += pBank->m_AlignedStructSize;
	}

	return true;
}

