#ifndef __ILTCURSOR_H__
#define __ILTCURSOR_H__

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif


typedef enum
{
    CM_None=0,
    CM_Hardware
} CursorMode;

class CLTCursorInst;
typedef CLTCursorInst *HLTCURSOR;

/*!
The ILTCursor interface provides functions to manipulate the cursor.

Define a holder to get this interface like this:
\code
define_holder(ILTCursor, your_var);
\endcode
*/

class ILTCursor : public IBase {
public:
    interface_version(ILTCursor, 0);

/*!
\param cMode Type of cursor mode to set (CM_None or CM_Hardware)
\param bForce true if the cursor mode should be set whether or not we think we're already in this mode
\return An LTRESULT specifying if the cursor mode was correctly set \b LT_OK \b on success 
or \b LT_UNSUPPORTED \b on failure)

Enable/disable hardware cursor.

Used for: Input.
*/
    virtual LTRESULT SetCursorMode(CursorMode cMode, bool bForce = false) = 0;

/*!
\param cMode A reference to the CursorMode type of this cursor.
\return Always returns \b LT_OK.

Get current cursor mode.  Always fills in \b cMode.

Used for: Input.
*/
    virtual LTRESULT GetCursorMode(CursorMode &cMode) = 0;

/*! 
\param cMode Type of cursor mode to set (CM_None or CM_Hardware)

\return Should return \b LT_YES if a hardware cursor can be used, \b
LT_NO otherwise.  Since we can't detect this, it just returns \b
LT_YES for now.

Used for: Input.  
*/
    virtual LTRESULT IsCursorModeAvailable(CursorMode cMode) = 0;

/*!
\param pName The file name of the bitmap file to open.
\param hCursor A reference to the HCURSOR handle to the cursor to be created
\return Should return \b LT_OK \b if the bitmap file could be opened,
\b LT_NOTINITIALIZED \b if the engine has not been initialised.

This will load a cursor from the specified resource. This should be relative to the
game root, and will handle loading from archives appropriately.
*/
    virtual LTRESULT LoadCursorFromFile(const char *pszFilename, HLTCURSOR &hCursor) = 0;

/*!
\param hCursor The handle to the cursor that was originally created using LoadCursorBitmapResource
\return Always returns \b LT_OK.

Free a cursor.

Used for: Input.
*/
    virtual LTRESULT FreeCursor(const HLTCURSOR hCursor) = 0;

/*!
\param hCursor A handle to a cursor that was originally created using LoadCursorBitmapResource
\return Returns \b LT_OK \b if the cursor was set to the hCursor handle passed in,
\b LT_INVALIDPARAMS \b if the hCursor handle passed in was invalid.

Set the current cursor.

Used for: Input.
*/
    virtual LTRESULT SetCursor(HLTCURSOR hCursor) = 0;

/*!
\return Always returns \b LT_OK.

Refreshes the cursor (displays it if type CM_Hardware or hides it if CM_None).

Used for: Input.
*/
    virtual LTRESULT RefreshCursor() = 0;
};

 
#endif