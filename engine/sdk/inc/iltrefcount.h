/*!  This file defines the standard reference counting interface, as well
as a convenient class for properly using objects which implement this 
interface. */

#ifndef __ILTREFCOUNT_H__
#define __ILTREFCOUNT_H__

//---------------------------------------
// CRefCount32
//
// This class encompasses a reference count object that can be AddRef'd and Released without having
// to pull in all the overhead of deriving from a base class.
//
class CRefCount32
{
public:
	
	CRefCount32(uint32 nStartCount = 0) :
		m_nRefCount(nStartCount)
	{
	}

	~CRefCount32()
	{
		VerifyNoReferences();
	}

	//called to add a new reference to this reference count.
	void AddRef()
	{
		LTASSERT(m_nRefCount < 0xFFFFFFFF, "Error: Overflow detected in reference count");
		m_nRefCount++;
	}

	//called to release a reference to this reference count. This will return true if the object
	//is no longer referenced.
	bool Release()
	{
		LTASSERT(m_nRefCount > 0, "Error: Underflow detected in reference count");
		m_nRefCount--;
		return (m_nRefCount == 0);
	}

	//called to access the current number of references to this object. Note that in practice this
	//should really only be used for debugging purposes as multithreading can make this value
	//incorrect, and only the result from Release should be relied upon.
	uint32 GetRefCount() const
	{
		return m_nRefCount;
	}

	//called to check and make sure that there are no outstanding references on this object. This is useful
	//for objects that contain a reference count so that they can call this in their destructor before
	//cleaning up the object to a state that debugging which object was leaked is impossible.
	void	VerifyNoReferences() const
	{
		LTASSERT(m_nRefCount == 0, "Warning: Encountered reference count that did not have all references freed");
	}

private:

	//our current reference count
	uint32		m_nRefCount;
};


//---------------------------------------
// ILTRefCount
//
// A base class that provides support for reference counting through AddRef and Release. This will call
// the overloaded 'Free' function when the object is destroyed
//
class ILTRefCount 
{
public:
	ILTRefCount()			{}
	virtual ~ILTRefCount()	{}

	virtual void	AddRef()				{ m_RefCount.AddRef(); }
	virtual void	Release()				{ if(m_RefCount.Release()) Free(); }
	virtual uint32	GetRefCount() const		{ return m_RefCount.GetRefCount(); }

	// Decrease the reference count on an object which was not allocated dynamically.
	virtual void	StaticRelease()			{ m_RefCount.Release(); }
private:

	//overridden by derived classes to handle when the reference count reaches zero
	virtual void Free() = 0;
	CRefCount32	m_RefCount;
};

//---------------------------------------
// CLTReference
//
// A smart pointer that can be used for more easily tracking and managing references to reference
// counted objects that support AddRef and Release
//
template <class T>
class CLTReference
{
public:
	CLTReference() : m_pPtr(0) {}
	CLTReference(const CLTReference &cOther) : m_pPtr(cOther.m_pPtr) {
		if (m_pPtr)
			m_pPtr->AddRef();
	}
	CLTReference(T *pPtr) : m_pPtr(pPtr) {
		if (m_pPtr)
			m_pPtr->AddRef();
	}
	~CLTReference() {
		if (m_pPtr)
			m_pPtr->Release(); 
	}
	CLTReference &operator=(const CLTReference &cOther) { return *this = (T*)cOther; }
	CLTReference &operator=(T *pPtr) {
		if (m_pPtr == pPtr)
			return *this;
		if (m_pPtr)
			m_pPtr->Release();
		m_pPtr = pPtr;
		if (m_pPtr)
			m_pPtr->AddRef();
		return *this;
	}
	bool operator==(const T *pPtr) const {
		return m_pPtr == pPtr;
	}
	bool operator!=(const T *pPtr) const { 
		return m_pPtr != pPtr;
	}
	T &operator*() const { return *m_pPtr; }
	T *operator->() const { return m_pPtr; }
	operator T*() const { return m_pPtr; }
private:
	T *m_pPtr;
};


#endif //__ILTREFCOUNT_H__
