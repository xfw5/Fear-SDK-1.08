//////////////////////////////////////////////////////////////////////////////
// The implementation of the LTObjRef classes

#include "ltobjref.h"

#include "iltbaseclass.h"


// Interface holders for the client/server differentiation
#include "iltserver.h"
static ILTServer *g_pLTObjRef_InterfaceS;
define_holder(ILTServer, g_pLTObjRef_InterfaceS);

#include "iltclient.h"
static ILTClient *g_pLTObjRef_InterfaceC;
define_holder(ILTClient, g_pLTObjRef_InterfaceC);


// Base LTObjRef implementation

void LTObjRef::Link(HOBJECT hObj)
{
	if (hObj)
	{
		// Only query for the interface when we are going to use it.  If code
		// calls the LTObjRef constructor with NULL as the HOBJECT (to clear 
		// it manually), we may get an assert due to static variable 
		// initialization order issues (conflicts between the code which has 
		// an LTObjRef it is initializing to NULL and the static pointers used 
		// to store these interfaces. 

		ILTCSBase *pInterface = g_pLTObjRef_InterfaceS ? (ILTCSBase *)g_pLTObjRef_InterfaceS : (ILTCSBase *)g_pLTObjRef_InterfaceC;
		if (!pInterface)
		{
			ASSERT(!"No interface available for object link");
			return;
		}

		if( pInterface->LinkObjRef(hObj, this) != LT_OK )
		{
			SetData(NULL);
			return;
		}
	}

	SetData(hObj);
}

void LTObjRef::Unlink()
{
	Remove();
	SetData(NULL);
}
