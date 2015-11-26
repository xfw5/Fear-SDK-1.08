/*!

 MODULE  : ltengineobjects.h.

 PURPOSE : C++ Engine object class(es) definition(s)

 CREATED : 9/17/97

*/

#ifndef __LTENGINEOBJECTS_H__
#define __LTENGINEOBJECTS_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTSERVEROBJ_H__
#include "ltserverobj.h"
#endif

#ifndef __ILTBASECLASS_H__
#include "iltbaseclass.h"
#endif

class ILTServer;
extern ILTServer *g_pLTServer;


/*!
Defines basic object functionality for all objects in a game.

All server-side objects in your game must derive from \b BaseClass.

Each object must have an object type (defined by the \b OT_* defines).
Use the \b SetType function to set an object's type.

This class provides basic engine-to-object and object-to-object event
notification functionality (using the \b ILTBaseClass notification interface,
\b EngineMessageFn (obsolete) and \b ObjectMessageFn).

Generic behavior across different objects can be implemented by using aggregates.
An object will forward event notification messages to all its associated
aggregates. For example, a damage aggregate can be used to control damage
logic for all objects that can be damaged. Use the \b AddAggregate and \b RemoveAggregate functions to implement
aggregate functionality.

You can implement any custom behavior required for your derived object.
*/

class BaseClass : public ILTBaseClass {
public:

    BaseClass(uint8 nType = OT_NORMAL)
       : ILTBaseClass(nType) {}

    virtual ~BaseClass();

/*!
\param messageID One of the \b MID_* define values.
\param pData Optional. A void pointer to data appropriate to the message type.
\param lData Optional. A float value appropriate to the message type.
\return Returns 1.

Receives event notifications from the engine.  Note that the engine actually calls
one of the \b "On*" notification functions, where the default implementation is to call
\b EngineMessageFn() with the appropriate \b MID_*.

The meaning of \b pData and \b lData vary by message type. See the Object chapter of the
Programming Guide for more information.

Before returning, this function should call the \b EngineMessageFn() of the object's parent
class to allow for inherited behavior and to allow aggregates to process the message.

\b EngineMessageFn will be deprecated in a future release in favor of the virtual function
interface inherited from \b ILTBaseClass.

Used For: Obsolete.
*/
    virtual uint32 EngineMessageFn(uint32 messageID, void *pData, float lData);

/*!
\return A pointer to the engine's \b ILTServer instance.

Returns a pointer to the engine's \b ILTServer instance.

Used For: Object.
*/
    static ILTServer *GetServerDE() { return (ILTServer*)g_pLTServer; }

}; // Baseclass

#endif  //! __LTENGINEOBJECTS_H__


