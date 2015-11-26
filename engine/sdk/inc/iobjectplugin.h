#ifndef __IOBJECTPLUGIN_H__
#define __IOBJECTPLUGIN_H__

	
	#include "iltpreinterface.h"


	class IObjectPlugin
	{
	public:

		virtual ~IObjectPlugin() {}




/*!  Preprocessor plugin functions.  */

	public:



/*!
\param szRezPath  Path to the rez directory for the project (input).
\param szPropName Name of the property (input).
\param aszStrings List of string pointers (output). Just strcpy into \b aszStrings[\b i] for string \b i.
\param pcStrings How many strings were specified (output)
\param cMaxStrings Max number of specifiable strings (input)				
\param cMaxStringLength Max length of any string in the list	

This hook gives the object a chance to return an array of strings to
populate a combobox edit control in WorldEdit.  If you implement this
function, you should always return \b LT_OK.

Used for: Misc.  */
		virtual LTRESULT PreHook_EditStringList(
			const char* szRezPath,				
			const char* szPropName,				
			char** aszStrings,					
			uint32* pcStrings,	
			const uint32 cMaxStrings,		
			const uint32 cMaxStringLength)		
		{
			LTUNREFERENCED_PARAMETER(szRezPath);
			LTUNREFERENCED_PARAMETER(szPropName);
			LTUNREFERENCED_PARAMETER(aszStrings);
			LTUNREFERENCED_PARAMETER(pcStrings);
			LTUNREFERENCED_PARAMETER(cMaxStrings);
			LTUNREFERENCED_PARAMETER(cMaxStringLength);
			return LT_UNSUPPORTED;
		}



/*!
\param szRezPath Path to the rez directory for the project (input)
\param szPropName Property name of the currently selected list item (input) 
\param szPropValue Value of the currently selected list item (input) 
\param szModelFilenameBuf Buffer to copy model filename into (output)
\param nModelFilenameBufLen Max buffer length (input)	
\param vDims Dimensions to use (instead of using a model file) (output).
\param pszObjName Name of the object selected (input) 
\param pInterface Pointer to caller (CBaseEditObj) (input)

This hook gives the object a chance to return either the model
filename to use to calculate the object's dims in WorldEdit, or the dims
themselves (if \b szModelFilename [0] == '\0').

\note This is only called on \b PF_STATICLIST properties.

If you implement this function, you should always return \b LT_OK.

Used for: Misc.  */
		virtual LTRESULT PreHook_Dims(
			const char* szRezPath,		
			const char* szPropName,	
			const char* szPropValue,	
			char* szModelFilenameBuf,
			int	  nModelFilenameBufLen,	
			LTVector & vDims,
			const char* pszObjName,
			ILTPreInterface *pInterface)			
		{
			LTUNREFERENCED_PARAMETER( szRezPath );		
			LTUNREFERENCED_PARAMETER( szPropValue );
			LTUNREFERENCED_PARAMETER( szModelFilenameBuf );
			LTUNREFERENCED_PARAMETER( nModelFilenameBufLen );
			LTUNREFERENCED_PARAMETER( vDims );

			return LT_UNSUPPORTED;
		}



/*!
\param szObjName Name of the object whos property changed (input)
\param szPropName Name of the property that has changed (input)
\param nPropType Type (ie. LT_PT_STRING) of the property (input)
\param gpPropValue The new value of the property (input)
\param pInterface Pointer to caller (CBaseEditObj) (input)
\param szModifiers String of modifiers used by the game code to alter the use of the changed property (input)

This hook gives the object a chance to check the new value to see if it is
a valid value for this property.

\note This is only called on \b PF_NOTIFYCHANGE properties.

If you implement this function, you should always return \b LT_OK.

Used for:	Misc */
		virtual LTRESULT PreHook_PropChanged(
			const	char		*szObjName,
			const	char		*szPropName,
			const	int			nPropType,
			const	GenericProp	& /* gpPropValue */,
			ILTPreInterface *pInterface,
			const	char		*szModifiers)
		{
			LTUNREFERENCED_PARAMETER( szObjName );
			LTUNREFERENCED_PARAMETER( szPropName );
			LTUNREFERENCED_PARAMETER( nPropType );
			LTUNREFERENCED_PARAMETER( pInterface );
			LTUNREFERENCED_PARAMETER( szModifiers);

			return LT_UNSUPPORTED;
		}

	};


#endif  //! __IOBJECTPLUGIN_H__
