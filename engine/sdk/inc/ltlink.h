/*!  This is for simple but very convenient doubly linked lists.  You
need an \b LTLink for the list head, and this header merely defines common
routines to insert, remove, and tie off.  */

#ifndef __LTLINK_H__
#define __LTLINK_H__

//forward declaration of LTLink for the odd LTHeadLink/LTLink relationship
template <class T>
class LTLink;

//forward declarations of the list types in order to allow friendship between the head links
//and the lists
template <class T>
class LTList;

template <class T>
class LTCountedList;


#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

#ifndef __LTASSERT_H__
#include "ltassert.h"
#endif

//---------------------------------------------------------------------------------------------
// LTHeadLink
//
// This link class provides simply pointers for a doubly linked list along with operations that
// can be used to modify them. This holds onto no actual data, and is intended only to serve
// as a base class for links that do hold onto data and also for heads of lists
//---------------------------------------------------------------------------------------------

template <class T> 
class LTHeadLink 
{
public:

	//called to remove this link from the list. This will also tie off the link making it an empty
	//list
    void Remove()
	{
		m_pPrev->m_pNext = m_pNext;
		m_pNext->m_pPrev = m_pPrev;
		TieOff();
	}

	//called to initialize the link to be an empty list
    void TieOff()
	{
		m_pPrev = m_pNext = (LTLink<T>*)this;
	}

	//called to add the specified link into the list after this link
    void AddAfter(LTHeadLink<T> *pLink)
	{
	    pLink->m_pPrev = (LTLink<T>*)this;
	    pLink->m_pNext = m_pNext;
	    pLink->m_pPrev->m_pNext = pLink->m_pNext->m_pPrev = (LTLink<T>*)pLink;
	}

	//called to add the specified link into the list before this link
    void AddBefore(LTHeadLink<T> *pLink)
	{
	    pLink->m_pPrev = m_pPrev;
	    pLink->m_pNext = (LTLink<T>*)this;
	    pLink->m_pPrev->m_pNext = pLink->m_pNext->m_pPrev = (LTLink<T>*)pLink;
	}

	//determines if this link is a single element list
    bool IsTiedOff() const
	{
	    return (GetPrev() == this) && (GetNext() == this);
	}

	//called to access the next and previous pointers
	LTLink<T>*	GetNext()					{ return m_pNext; }
	LTLink<T>*	GetPrev()					{ return m_pPrev; }

	void SetNext(LTLink<T>* pNext)			{ m_pNext = pNext; }
	void SetPrev(LTLink<T>* pPrev)			{ m_pPrev = pPrev; }

	const LTLink<T>*	GetNext() const		{ return m_pNext; }
	const LTLink<T>*	GetPrev() const		{ return m_pPrev; }

protected:

	friend class LTList<T>;
	friend class LTCountedList<T>;

	//lifetime operators. This will initialize the link to be an empty link when
	//it is constructed. These are declared protected because a LTHeadLink should only
	//be created as a base class for a link, or as a head node in a list (hence the reason
	//for friendship with the list)
	LTHeadLink()		{ TieOff(); }
	~LTHeadLink()
	{
		//we should always be tied off, otherwise we will have dangling pointers
		LTASSERT(IsTiedOff(), "Error: Destroying link that was not properly tied off");
	}

private:

	//we cannot support copying since if we ever did copy this link, it would corrupt the list
	LTHeadLink(const LTHeadLink&);
	LTHeadLink& operator=(const LTHeadLink&);

	//the links forwards and backwards in the cyclic list
    LTLink<T> *m_pPrev;
	LTLink<T> *m_pNext;
};

//---------------------------------------------------------------------------------------------
// LTLink
//
// This link class has doubly linked list support and also a typed data member so that it can
// hold onto any form of data appropriate.
//---------------------------------------------------------------------------------------------
template <class T>
class LTLink : 
	public LTHeadLink<T>
{
public:

	LTLink()							{}
	LTLink(const T& Def) : m_Data(Def)	{}
    
	//accessors for the provided data
	void		SetData(const T& Data)		{ m_Data = Data; }
	T&			GetData()					{ return m_Data; }
	const T&	GetData() const				{ return m_Data; }

private:

	//we cannot copy this link since copying of this link would corrupt the list
	LTLink(const LTLink&);
	LTLink& operator=(const LTLink&);

    T	m_Data;
};

//---------------------------------------------------------------------------------------------
// LTListIter
//
// This class provides forward iteration support through a listing of links and should
// be used with the list classes below's Begin/End functionality.
//---------------------------------------------------------------------------------------------
template <class T>
class LTListIter
{
public:

	//creation 
	LTListIter() : m_pPos(NULL)									{}
	LTListIter(const LTListIter& rhs) : m_pPos(rhs.m_pPos)		{}
	LTListIter(LTLink<T>* pPos)	: m_pPos(pPos)					{}

	//assignment
	LTListIter& operator=(const LTListIter& rhs)				{ m_pPos = rhs.m_pPos; return *this; }
	LTListIter& operator=(LTLink<T>* pPos)						{ m_pPos = pPos; return *this; }

	//comparison
	bool operator==(const LTListIter& rhs) const				{ return m_pPos == rhs.m_pPos; }
	bool operator==(LTLink<T>* pPos) const						{ return m_pPos == pPos; }
	bool operator!=(const LTListIter& rhs) const				{ return m_pPos != rhs.m_pPos; }
	bool operator!=(LTLink<T>* pPos) const						{ return m_pPos != pPos; }

	//iteration
	LTListIter operator++()										{ LTListIter Temp(*this); m_pPos = m_pPos->GetNext(); return Temp; }
	LTListIter& operator++(int nPrefix)							{ m_pPos = m_pPos->GetNext(); return *this; }
	LTListIter GetNext()										{ return LTListIter(m_pPos->GetNext()); }

	//provide access to the data
	T& operator->()												{ return m_pPos->GetData(); }
	T& operator*()												{ return m_pPos->GetData(); }
	
	//provide access to the link
	LTLink<T>*	GetLink()										{ return m_pPos; }

private:

	//the current link at our position in the list
	LTLink<T>*	m_pPos;
};

//---------------------------------------------------------------------------------------------
// LTRListIter
//
// This class provides reverse iteration support through a listing of links and should
// be used with the list classes below's RBegin/REnd functionality.
//---------------------------------------------------------------------------------------------
template <class T>
class LTRListIter
{
public:

	//creation 
	LTRListIter() : m_pPos(NULL)								{}
	LTRListIter(const LTRListIter& rhs) : m_pPos(rhs.m_pPos)	{}
	LTRListIter(LTLink<T>* pPos)	: m_pPos(pPos)				{}

	//assignment
	LTRListIter& operator=(const LTRListIter& rhs)				{ m_pPos = rhs.m_pPos; return *this; }
	LTRListIter& operator=(LTLink<T>* pPos)						{ m_pPos = pPos; return *this; }

	//comparison
	bool operator==(const LTRListIter& rhs) const				{ return m_pPos == rhs.m_pPos; }
	bool operator==(LTLink<T>* pPos) const						{ return m_pPos == pPos; }
	bool operator!=(const LTRListIter& rhs) const				{ return m_pPos != rhs.m_pPos; }
	bool operator!=(LTLink<T>* pPos) const						{ return m_pPos != pPos; }

	//iteration
	LTRListIter operator++()									{ LTRListIter Temp(*this); m_pPos = m_pPos->GetPrev(); return Temp; }
	LTRListIter& operator++(int nPrefix)						{ m_pPos = m_pPos->GetPrev(); return *this; }
	LTRListIter GetNext()										{ return LTRListIter(m_pPos->GetPrev()); }

	//provide access to the data
	T& operator->()												{ return m_pPos->GetData(); }
	T& operator*()												{ return m_pPos->GetData(); }
	
	//provide access to the link
	LTLink<T>*	GetLink()										{ return m_pPos; }

private:

	//the current link at our position in the list
	LTLink<T>*	m_pPos;
};


//---------------------------------------------------------------------------------------------
// LTList
//
// Provides a list implementation that uses the above link types. This list class does not
// support counting of elements and is intended to be very light weight.
//---------------------------------------------------------------------------------------------

template <class T>
class LTList 
{
public:

	//lifetime operators
	LTList()			{}
	~LTList()			{}

	//this type indicates the iterator type that should be used for iterating through the list
	typedef LTListIter<T>  Iterator;
	typedef LTRListIter<T> RIterator;

	//called to initialize the list to an empty state
	void	Clear()
	{
		m_Head.Remove();
	}

	//called to determine if this list is empty
	bool	IsEmpty() const			{ return m_Head.IsTiedOff(); }

	//called to add an element in the list after the specified link
	void	AddAfter(LTLink<T> *pAfter, LTLink<T> *pLink)
	{
		LTASSERT(IsInList(pAfter), "Error: Adding to a link that is not in the list");
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");
		pAfter->AddAfter(pLink);
	}

	//called to add an element in the list before the specified link
	void	AddBefore(LTLink<T> *pBefore, LTLink<T> *pLink)
	{
		LTASSERT(IsInList(pBefore), "Error: Adding to a link that is not in the list");
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");
		pBefore->AddBefore(pLink);
	}

	//adds a link to the head of the list
	void	AddHead(LTLink<T> *pLink)
	{
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");
		m_Head.AddAfter(pLink);
	}

	//adds a link to the tail of the list
	void	AddTail(LTLink<T> *pLink)
	{
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");
		m_Head.AddBefore(pLink);
	}

	//removes a link from the list and ties off the link
	void	Remove(LTLink<T>* pLink)
	{
		LTASSERT(pLink != &m_Head, "Error: Trying to remove the head node of the list");
		LTASSERT(IsInList(pLink), "Error: Removing a link that is not in the list");
		pLink->Remove();
	}

	//this will determine the count of the number of objects in the list by actually iterating through
	//the list. This is useful for verifying that the list size is correct
	uint32	CountObjects()
	{
		uint32 nResult = 0;
		for(Iterator it = Begin(); it != End(); it++)
			nResult++;

		return nResult;
	}

	//given a data pointer, this will run through the links, and if it finds an item that matches it,
	//will return the link pointer, otherwise NULL if it can't be found
	LTLink<T>*	FindData(const T& Data)
	{
		for(Iterator it = Begin(); it != End(); it++)
		{
			if(*it == Data)
				return it.GetLink();
		}
		return NULL;
	}

	//given a link, this will return whether or not it is already in the list
	bool	IsInList(const LTLink<T>* pLink)
	{
		for(Iterator it = Begin(); it != End(); it++)
		{
			if(it.GetLink() == pLink)
				return true;
		}
		return false;
	}

	//iterator like support for the links. These provide forward iteration and should be used like:
	//for(List::Iterator it = List.Begin(); it != List.End(); it++)
	LTLink<T>*	Begin()					{ return m_Head.GetNext(); }
	LTLink<T>*	End()					{ return (LTLink<T>*)&m_Head; }

	//reverse iterator like support for the links. These provide reverse iteration and should be used like:
	//for(List::RIterator it = List.RBegin(); it != List.REnd(); it++)
	LTLink<T>*	RBegin()				{ return m_Head.GetPrev(); }
	LTLink<T>*	REnd()					{ return (LTLink<T>*)&m_Head; }


private:

	//we can't copy this list since it would corrupt the list if we did so
	LTList(const LTList&);
	LTList& operator=(const LTList&);

	//the head of the list
    LTHeadLink<T>	m_Head;
};

//---------------------------------------------------------------------------------------------
// LTCountedList
//
// Provides a list implementation that uses the above link types. This list tracks the count
// of all the elements within, and therefore occupies 4 bytes more. In addition it is absolutely
// critical that all addition and removal operations be called through this list and not the
// nodes as that will otherwise invalidate the count of the objects.
//---------------------------------------------------------------------------------------------

template <class T>
class LTCountedList 
{
public:

	//lifetime operators
	LTCountedList() : m_nElements(0)			{}
	~LTCountedList()							{}

	//this type indicates the iterator type that should be used for iterating through the list
	typedef LTListIter<T>  Iterator;
	typedef LTRListIter<T> RIterator;

	//called to initialize the list to an empty state
	void	Clear()
	{
		m_Head.Remove();
		m_nElements = 0;
	}

	//called to determine if this list is empty
	bool	IsEmpty() const			{ return m_Head.IsTiedOff(); }

	//called to access the number of elements in the list. Note that if this is to be relied upon,
	//care must be taken to always add and remove items from the list using the list, not the
	//nodes themselves (ie use List.Remove(node), not Node.Remove())
	uint32	GetSize() const		{ return m_nElements; }                    
    
	//called to add an element in the list after the specified link
	void	AddAfter(LTLink<T> *pAfter, LTLink<T> *pLink)
	{
		LTASSERT(IsInList(pAfter), "Error: Adding to a link that is not in the list");
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");

		pAfter->AddAfter(pLink);
		m_nElements++;
	}

	//called to add an element in the list before the specified link
	void	AddBefore(LTLink<T> *pBefore, LTLink<T> *pLink)
	{
		LTASSERT(IsInList(pBefore), "Error: Adding to a link that is not in the list");
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");

		pBefore->AddBefore(pLink);
		m_nElements++;
	}

	//adds a link to the head of the list
	void	AddHead(LTLink<T> *pLink)
	{
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");
		m_Head.AddAfter(pLink);
		m_nElements++;
	}

	//adds a link to the tail of the list
	void	AddTail(LTLink<T> *pLink)
	{
		LTASSERT(!IsInList(pLink), "Error: Adding an object that is already in the list");
		m_Head.AddBefore(pLink);
		m_nElements++;
	}

	//removes a link from the list and ties off the link
	void	Remove(LTLink<T>* pLink)
	{
		LTASSERT(pLink != &m_Head, "Error: Trying to remove the head node of the list");
		LTASSERT(IsInList(pLink), "Error: Removing a link that is not in the list");
		pLink->Remove();
		m_nElements--;
	}

	//this will determine the count of the number of objects in the list by actually iterating through
	//the list. This is useful for verifying that the list size is correct
	uint32	CountObjects()
	{
		uint32 nResult = 0;
		for(Iterator it = Begin(); it != End(); it++)
			nResult++;

		return nResult;
	}

	//given a data pointer, this will run through the links, and if it finds an item that matches it,
	//will return the link pointer, otherwise NULL if it can't be found
	LTLink<T>*	FindData(const T& Data)
	{
		for(Iterator it = Begin(); it != End(); it++)
		{
			if(*it == Data)
				return it.GetLink();
		}
		return NULL;
	}

	//given a link, this will return whether or not it is already in the list
	bool	IsInList(const LTLink<T>* pLink)
	{
		for(Iterator it = Begin(); it != End(); it++)
		{
			if(it.GetLink() == pLink)
				return true;
		}
		return false;
	}

	//iterator like support for the links. These provide forward iteration and should be used like:
	//for(List::Iterator it = List.Begin(); it != List.End(); it++)
	LTLink<T>*	Begin()					{ return m_Head.GetNext(); }
	LTLink<T>*	End()					{ return (LTLink<T>*)&m_Head; }

	//reverse iterator like support for the links. These provide reverse iteration and should be used like:
	//for(List::RIterator it = List.RBegin(); it != List.REnd(); it++)
	LTLink<T>*	RBegin()				{ return m_Head.GetPrev(); }
	LTLink<T>*	REnd()					{ return (LTLink<T>*)&m_Head; }

private:

	//we can't copy this list since it would corrupt the list if we did so
	LTCountedList(const LTCountedList&);
	LTCountedList& operator=(const LTCountedList&);

	//the head of the list
    LTHeadLink<T>	m_Head;

	//the count of elements in the list
	uint32			m_nElements;
};

#endif  //! __LTLINK_H__

