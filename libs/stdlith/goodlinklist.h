#ifndef __GOODLINKLIST_H__
#define __GOODLINKLIST_H__

#ifndef __LTINTEGER_H__
#include "ltinteger.h"
#endif

#ifndef __STDLITHDEFS_H__
#include "stdlithdefs.h"
#endif


class CGLLNode
{
public:
    
    CGLLNode    *m_pGPrev, *m_pGNext;

};
typedef CGLLNode*   GPOS;

template<class T>
class CGLinkedList
{
public:

    CGLinkedList()
    {
        m_nElements = 0;
        m_pHead = NULL;
    }

    ~CGLinkedList()
    {
        RemoveAll();
    }

    void Term()			{RemoveAll();}

    operator uint32()	{return m_nElements;}
    operator GPOS()		{ return m_pHead; }
    
    uint32			GetSize()   const       { return m_nElements; }
    bool			IsEmpty()   const       { return !m_pHead; }

    inline T        GetHead()   const;
    inline T        GetTail()   const;

    inline T        RemoveHead();
    inline T        RemoveTail();

    inline GPOS     AddHead(T newHead);
    inline GPOS     AddTail(T newTail);

    inline void     RemoveAll();

    GPOS			GetHeadPosition()   const   { return m_pHead; }
    GPOS			GetTailPosition()   const   { return (m_pHead ? m_pHead->m_pGPrev : NULL); }

    inline T        GetNext(GPOS &pos)    const;
    inline T        GetPrev(GPOS &pos)    const;
    
    inline T        GetAt(GPOS pos) const;
    
    inline void     RemoveAt(GPOS pos);

    GPOS			InsertBefore(GPOS pos, T el);
    GPOS			InsertAfter(GPOS pos, T el);

    // Find and compare with T::operator==.
    GPOS			Find(T searchFor, uint32 *pIndex=NULL)        const;
    uint32			FindElement(T searchFor, GPOS *pPos=NULL)     const;

    GPOS			FindIndex(uint32 index)                       const;

protected:

    uint32          m_nElements;
    CGLLNode        *m_pHead;
};



template<class T>
inline T CGLinkedList<T>::GetHead() const
{
    LTASSERT(m_pHead, "Invalid head pointer encountered");
    return (T)m_pHead;
}


template<class T>
inline T CGLinkedList<T>::GetTail() const
{
    LTASSERT(m_pHead, "Invalid head pointer encountered");
    return (T)m_pHead->m_pGPrev;
}


template<class T>
inline T CGLinkedList<T>::GetNext(GPOS &pos) const
{
    T ret = (T)pos;
    
    if (pos->m_pGNext == m_pHead)
        pos = NULL;
    else
        pos = pos->m_pGNext;

    return ret;
}


template<class T>
inline T CGLinkedList<T>::GetPrev(GPOS &pos) const
{
    T ret = (T)pos;

    if (pos->m_pGPrev == m_pHead)
        pos = NULL;
    else
        pos = pos->m_pGPrev;

    return ret;
}


template<class T>
inline T CGLinkedList<T>::RemoveHead()
{
    LTASSERT(m_pHead, "Invalid head pointer encountered");
    T ret = (T)m_pHead;
    RemoveAt(m_pHead);
    
    return ret;
}


template<class T>
inline T CGLinkedList<T>::RemoveTail()
{
    LTASSERT(m_pHead, "Invalid head pointer encountered");
    T ret = (T)m_pHead->m_pGPrev;
    RemoveAt(m_pHead->m_pGPrev);
    
    return ret;
}


template<class T>
inline GPOS CGLinkedList<T>::AddHead(T newHead)
{
    if (m_pHead)
    {
        return InsertBefore(m_pHead, newHead);
    }
    else
    {
        CGLLNode *pNode = (CGLLNode*)newHead;

        pNode->m_pGNext = (CGLLNode*)newHead;
        pNode->m_pGPrev = (CGLLNode*)newHead;

        m_pHead = (CGLLNode*)newHead;
        ++m_nElements;

        return m_pHead;
    }
}


template<class T>
inline GPOS CGLinkedList<T>::AddTail(T newTail)
{
    if (m_pHead)
        return InsertAfter(m_pHead->m_pGPrev, newTail);
    else
        return AddHead(newTail);
}


template<class T>
inline void CGLinkedList<T>::RemoveAll()
{
    m_pHead = NULL;
    m_nElements = 0;
}


template<class T>
inline T CGLinkedList<T>::GetAt(GPOS pos) const
{
    return (T)pos;
}


template<class T>
void CGLinkedList<T>::RemoveAt(GPOS pos)
{
    LTASSERT(m_nElements > 0, "Removing element from empty list");
    
    pos->m_pGPrev->m_pGNext = pos->m_pGNext;
    pos->m_pGNext->m_pGPrev = pos->m_pGPrev;
    
    if (pos == m_pHead)
        m_pHead = m_pHead->m_pGNext;
    
    --m_nElements;
    if (m_nElements == 0)
        m_pHead = NULL;
}


template<class T>
GPOS CGLinkedList<T>::InsertBefore(GPOS pos, T el)
{
    pos->m_pGPrev->m_pGNext = (CGLLNode*)el;
    ((CGLLNode*)el)->m_pGPrev = pos->m_pGPrev;
    ((CGLLNode*)el)->m_pGNext = pos;
    pos->m_pGPrev = (CGLLNode*)el;
    
    ++m_nElements;
    if (((CGLLNode*)el)->m_pGNext == m_pHead)
        m_pHead = (CGLLNode*)el;

    return (CGLLNode*)el;
}


template<class T>
GPOS CGLinkedList<T>::InsertAfter(GPOS pos, T el)
{
    pos->m_pGNext->m_pGPrev = (CGLLNode*)el;
    ((CGLLNode*)el)->m_pGNext = pos->m_pGNext;
    ((CGLLNode*)el)->m_pGPrev = pos;
    pos->m_pGNext = (CGLLNode*)el;
    
    ++m_nElements;
    return (CGLLNode*)el;
}


template<class T>
GPOS CGLinkedList<T>::Find(T searchFor, uint32 *pIndex)   const
{
    
    if (m_pHead)
    {
        T pCur = (T)m_pHead;
		uint32 index = 0;

        do
        {
            
            if (pCur == searchFor)
            {
                if (pIndex)
                    *pIndex = index;

                return pCur;
            }

            pCur = (T)pCur->m_pGNext;
            ++index;

        } while (pCur != (T)m_pHead);
    }

    if (pIndex)
        *pIndex = BAD_INDEX;

    return NULL;
}


template<class T>
uint32 CGLinkedList<T>::FindElement(T searchFor, GPOS *pPos) const
{
    uint32  index;   
    GPOS pos = Find(searchFor, &index);

    if (pPos)
        *pPos = pos;

    return index;
}


template<class T>
GPOS CGLinkedList<T>::FindIndex(uint32 index) const
{
    if (index >= m_nElements)
        return NULL;

    T pCur = (T)m_pHead;
    do
    {
        
        if (index == 0)
            return pCur;

        pCur = (T)pCur->m_pGNext;
        --index;

    } while (pCur != (T)m_pHead);

    LTERROR("Index not found in list");    // Shouldn't ever get here.
    return NULL;
}

// Useful template function if your linked list is made up of pointers.
template<class T>
void GDeleteAndRemoveElements(CGLinkedList<T> &theList)
{
    for (GPOS pos=theList.GetHeadPosition(); pos != NULL;)
        delete (T)(theList.GetNext(pos));

    theList.RemoveAll();
}


// Useful template function if your linked list is made up of pointers.
template<class T, class OB>
void GDeleteAndRemoveElementsOB(CGLinkedList<T> &theList, OB &objectBank)
{
    for (GPOS pos=theList.GetHeadPosition(); pos != NULL;)
    {
        objectBank.Free((T)(theList.GetNext(pos)));
    }

    theList.RemoveAll();
}


#endif // __GOODLINKLIST_H__
    





