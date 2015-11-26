#ifndef __LTOBJREF_H__
#define __LTOBJREF_H__

#ifndef __LTBASETYPES_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTLINK_H__
#include "ltlink.h"
#endif

#include "ltassert.h"

class ILTBaseClass;

//////////////////////////////////////////////////////////////////////////////
// Object reference type implementation

// ----------------------------------------------------------------------- //
//
//	ILTObjRefReceiver
//
//	PURPOSE:	For a class to get a notification when a LTObjRefNotifier gets
//				deleted, you must implement this interface.  OnLinkBroken
//				will get called when the object gets deleted.  
//				The LTObjRefNotifier will be cleared of the HOBJECT before
//				the call to OnLinkBroken, so you cannot compare it against
//				other HOBJECTs.  You can compare the pointer value of the 
//				LTObjRefNotifier against the address of your variables to
//				identify the correct variable.  The HOBJECT value is also
//				provided if needed.
//
// ----------------------------------------------------------------------- //
class LTObjRefNotifier;
class ILTObjRefReceiver
{
	public:
	
		// Implementing classes will have this function called
		// when HOBJECT ref points to gets deleted.
		virtual void OnLinkBroken( LTObjRefNotifier *pRef, HOBJECT hObj ) = 0;
};


// The base object reference class
class LTObjRef : 
	public LTLink<HOBJECT>
{
public:
	// ctor's, dtor's
	LTObjRef() { SetData(NULL); }
	LTObjRef(HOBJECT hObj) { Link(hObj); }
	LTObjRef( const LTObjRef& cOther )
	{ 
		Link(cOther.GetData());
	}
	virtual ~LTObjRef() { Unlink(); }

	LTObjRef &operator=(const LTObjRef &cOther) {
		if (cOther.GetData() == GetData())
			return *this;
		Unlink();
		Link(cOther.GetData());
		return *this;
	}

	// Functions to make it act like an HOBJECT
	LTObjRef &operator=(HOBJECT hObj) {
		if (hObj == GetData())
			return *this;
		Unlink();
		Link(hObj);
		return *this;
	}
	operator HOBJECT() const { return GetData(); }

	// The callback from the engine for when the object gets deleted
	virtual void OnObjDelete() { Unlink(); }

	// Set/Get the containing object.  No actual containing
	// object exists for LTObjRef's.  Only for LTObjRefNotifier's.
	virtual void SetReceiver( ILTObjRefReceiver& receiver ) { LTUNREFERENCED_PARAMETER(receiver); }
	virtual ILTObjRefReceiver* GetReceiver( ) const { return NULL; }

private:
	// Link to the specified object
	void Link(HOBJECT hObj);
	// Unlink from the specified object
	void Unlink();

	// The actual object is stored in the parent class's m_pData member
};

// ----------------------------------------------------------------------- //
//
//	LTObjRefNotifier
//
//	PURPOSE:	This version of LTObjRef provides a way to get notifications
//				when the referred to HOBJECT is deleted.  The containing
//				class must implement ILTObjRefReceiver.
//
// ----------------------------------------------------------------------- //

class LTObjRefNotifier : public LTObjRef
{
	public:

		LTObjRefNotifier( ) { m_pReceiver = NULL; }
		LTObjRefNotifier( HOBJECT hObj ) : LTObjRef( hObj ) { m_pReceiver = NULL; }
		LTObjRefNotifier( ILTObjRefReceiver& receiver ) { m_pReceiver = &receiver; }

	private:

		// Copy ctor and assignment operator are disabled.  Since the behaviour
		// of m_pReceiver under these circumstances is undefined, copy of LTObjRefNotifier to
		// LTObjRefNotifier must be done through the LTObjRef assignment operator.
		LTObjRefNotifier( const LTObjRefNotifier& cOther );
		LTObjRefNotifier& operator=( LTObjRefNotifier const& cOther );

	public:

		// Functions to make it act like an HOBJECT
		LTObjRefNotifier &operator=( HOBJECT hObj )
		{
			ASSERT( !hObj || ( hObj && m_pReceiver ));
			LTObjRef::operator=( hObj );
			return *this;
		}

		// Set/Get the containing object.
		virtual void SetReceiver( ILTObjRefReceiver& receiver ) { m_pReceiver = &receiver; }
		virtual ILTObjRefReceiver* GetReceiver( ) const { return m_pReceiver; }

	private:

		// Back pointer to the Container.
		ILTObjRefReceiver*	m_pReceiver;
};

// ----------------------------------------------------------------------- //
//
//	LTObjRefNotifierWithCopy
//
//	PURPOSE:	This version is identical to LTObjRefNotifier, but allows
//				copy constructor.  Use this within std containers, which 
//				do copies during re-allocation.  Still doesn't allow
//				implicit assignment operator.  Instead, pass HOBJECT.
//
// ----------------------------------------------------------------------- //

class LTObjRefNotifierWithCopy : public LTObjRefNotifier
{
	public:

		LTObjRefNotifierWithCopy( ) : LTObjRefNotifier( ) { }
		LTObjRefNotifierWithCopy( HOBJECT hObj ) : LTObjRefNotifier( hObj ) { }
		LTObjRefNotifierWithCopy( ILTObjRefReceiver& receiver ) : LTObjRefNotifier( receiver ) { }
		LTObjRefNotifierWithCopy( const LTObjRefNotifierWithCopy& cOther ) : 
			LTObjRefNotifier(( HOBJECT )cOther )
		{
			if( cOther.GetReceiver( ))
				SetReceiver( *cOther.GetReceiver( ));
		}
		LTObjRefNotifierWithCopy( const LTObjRefNotifier& cOther ) : 
			LTObjRefNotifier(( HOBJECT )cOther )
		{
			if( cOther.GetReceiver( ))
				SetReceiver( *cOther.GetReceiver( ));
		}

	private:

		// Stil don't allow implicit assignment operator.
		LTObjRefNotifierWithCopy& operator=( LTObjRefNotifierWithCopy const& cOther );
		LTObjRefNotifierWithCopy& operator=( LTObjRefNotifier const& cOther );
};

// ----------------------------------------------------------------------- //
//
//	LTObjRefNotifierUnsafe
//
//	PURPOSE:	This version is identical to LTObjRefNotifier, but allows
//				copy constructor and assignment.  This is useful when using
//				STL heavily with these objects, when none of the issues with
//				copying the receiver will apply.
//
// IMPORTANT NOTE:	Make sure that any assignment behavior is exactly what 
//					you expect when operating on instances of this class, since
//					the receiver will be copied when assigning from an 
//					LTObjRefNotifier, but will not be modified when assigning
//					from an LTObjRef or HOBJECT.  Do not mix instances of this
//					class between different notifiers, and do not pass 
//					instances of this class outside of the scope of their 
//					containers.
//
// ----------------------------------------------------------------------- //

class LTObjRefNotifierUnsafe : public LTObjRefNotifier
{
public:

	LTObjRefNotifierUnsafe( ) : LTObjRefNotifier( ) { }
	LTObjRefNotifierUnsafe( HOBJECT hObj ) : LTObjRefNotifier( hObj ) { }
	LTObjRefNotifierUnsafe( ILTObjRefReceiver& receiver ) : LTObjRefNotifier( receiver ) { }
	LTObjRefNotifierUnsafe( const LTObjRefNotifier& cOther ) : 
		LTObjRefNotifier(( HOBJECT )cOther )
	{
		SetReceiver( *cOther.GetReceiver( ));
	}
	LTObjRefNotifierUnsafe( const LTObjRefNotifierUnsafe& cOther )
	{
		new(this) LTObjRefNotifierUnsafe( (LTObjRefNotifier&)cOther );
	}

	LTObjRefNotifierUnsafe& operator=( HOBJECT hOther )
	{
		(( LTObjRef& )*this ) = ( HOBJECT )hOther;
		return *this;
	}
	LTObjRefNotifierUnsafe& operator=( LTObjRefNotifier const& cOther )
	{
		*this = ( HOBJECT )cOther;
		SetReceiver( *cOther.GetReceiver() );
		return *this;
	}
	LTObjRefNotifierUnsafe& operator=( LTObjRefNotifierUnsafe const& cOther )
	{
		return *this = ( LTObjRefNotifier const& )cOther;
	}
private:
};

#endif //__LTOBJREF_H__
