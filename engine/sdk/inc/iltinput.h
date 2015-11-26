/*
*	Input management interface - ILTInput
*/

#ifndef __ILTINPUT_H__
#define __ILTINPUT_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __LTMODULE_H__
#include "ltmodule.h"
#endif

class ILTInput : public IBase
{
public:
	interface_version(ILTInput, 1);

	// Input device categories
	enum EDeviceCategory {
		eDC_Keyboard,
		eDC_Mouse,
		eDC_Gamepad
	};

	// Input device object categories
	enum EDeviceObjectCategory {
		eDOC_Axis,
		eDOC_Button,
		eDOC_POV,
		eDOC_Rumble
	};

	// Use this value to indicate an invalid index
	enum { k_InvalidIndex = -1 };

	// Device description structure
	struct SDeviceDesc
	{
		enum { k_nMaxNameLength = MAX_PATH };
		wchar_t m_sName[k_nMaxNameLength + 1];
		wchar_t m_sDisplayName[k_nMaxNameLength + 1];
		ILTInput::EDeviceCategory m_eCategory;
		uint32 m_nNumObjects;
	};

	// Device object description structure
	struct SDeviceObjectDesc
	{
		enum { k_nMaxNameLength = MAX_PATH };
		wchar_t m_sName[k_nMaxNameLength + 1];
		wchar_t m_sDisplayName[k_nMaxNameLength + 1];
		ILTInput::EDeviceObjectCategory m_eCategory;
		bool m_bWriteable;
		float m_fResolution;
		uint32 m_nControlCode;
	};

	/* Queries the number of devices available to the input manager.
		Parameters:
			pNumDevices - Result containing the number of devices
		Returns:
			LT_INVALIDPARAMS - pNumDevices was NULL
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT GetNumDevices(uint32 *pNumDevices) = 0;

	/* Queries the number of devices of a specific category available to the input manager.
		Parameters:
			eCategory - Device category to query
			pNumDevices - Result containing the number of devices
		Returns:
			LT_INVALIDPARAMS - pNumDevices was NULL
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT GetNumDevicesByCategory(EDeviceCategory eCategory, uint32 *pNumDevices) = 0;

	/* Retrieves the description of a device
		Parameters:
			nDeviceIndex - Index of the device to retreive the description for
			pDevice - Result containing the description of the device
		Returns:
			LT_INVALIDPARAMS - pDevice was NULL, or nDeviceIndex specified an invalid device
			LT_OK - Operation completed successfully
	*/ 
	virtual LTRESULT GetDeviceDesc(uint32 nDeviceIndex, SDeviceDesc *pDevice) = 0;

	/* Finds a device by name
		Parameters:
			pName - Indicates the name of the device
			pDeviceIndex - Result containing index of the device, if found
		Returns:
			LT_INVALIDPARAMS - pName or pDeviceIndex was NULL
			LT_NOTFOUND - No device was found matching the provided name
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT FindDeviceByName(const wchar_t *pName, uint32 *pDeviceIndex) = 0;

	/* Finds the first device with the specified category
		Parameters:
			eCategory - The category to search for
			pDeviceIndex - Result containing the index of the device, if found
		Returns:
			LT_INVALIDPARAMS - pDeviceIndex was NULL
			LT_NOTFOUND - No device was found with the specified category
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT FindFirstDeviceByCategory(EDeviceCategory eCategory, uint32 *pDeviceIndex) = 0;

	/* Queries whether or not the device is currently ready to receive input
		Parameters :
			nDeviceIndex - The device to search for
			pReady - Result containing the ready state of the device
		Returns :
			LT_INVALIDPARAMS - nDeviceIndex specified an invalid device, or pReady was NULL
			LT_OK - Operation completed successfully
	 */
	virtual LTRESULT IsDeviceReady(uint32 nDeviceIndex, bool *pReady) = 0;


	/* Retrieves the description of a device object
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			nObjectIndex - Index of the object in the device
			pObject - Result containing the description of the device object
		Returns:
			LT_INVALIDPARAMS - pDevice was NULL, nDeviceIndex specified an invalid device, 
				or nObjectIndex specified an invalid device object
			LT_OK - Operation completed successfully
	*/ 
	virtual LTRESULT GetDeviceObjectDesc(uint32 nDeviceIndex, uint32 nObjectIndex, SDeviceObjectDesc *pObject) = 0;

	/* Queries the number of device objects of a specific category on the device.
		Parameters:
			pNumObjects - Result containing the number of objects
			eCategory - Object category to query
		Returns:
			LT_INVALIDPARAMS - pNumObjects was NULL
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT GetNumDeviceObjectsByCategory(uint32 nDeviceIndex, EDeviceObjectCategory eCategory, uint32 *pNumObjects) = 0;

	/* Finds a device object by name
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			pName - Indicates the name of the device object
			pObjectIndex - Result containing index of the deviceobject , if found
		Returns:
			LT_INVALIDPARAMS - pName or pObjectIndex was NULL, or nDeviceIndex specified an invalid device
			LT_NOTFOUND - No device object was found matching the provided name
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT FindDeviceObjectByName(uint32 nDeviceIndex, const wchar_t *pName, uint32 *pObjectIndex) = 0;

	/* Finds the first device object with the specified category
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			eCategory - The object category to search for
			pObjectIndex - Result containing the index of the device object, if found
		Returns:
			LT_INVALIDPARAMS - pObjectIndex was NULL, or nDeviceIndex specified an invalid device
			LT_NOTFOUND - No device object was found with the specified category
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT FindFirstDeviceObjectByCategory(uint32 nDeviceIndex, EDeviceObjectCategory eCategory, uint32 *pObjectIndex) = 0;


	/* Retrieves the current value of a device object
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			nObjectIndex - Index of the object in the device
			pValue - Result containing the current value of the device object
		Returns:
			LT_INVALIDPARAMS - pValue was NULL, nDeviceIndex specified an invalid device, 
				or nObjectIndex specified an invalid device object
			LT_RETRY - Device is not currently available.  Try again later.
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT GetDeviceObjectValue(uint32 nDeviceIndex, uint32 nObjectIndex, float *pValue) = 0;

	/* Retrieves the current values of a range of device objects
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			nFirstObjectIndex - Index of the first object to query in the device
			nNumObjects - Number of objects to query
			pValues - Result containing the current values of the device objects
		Returns:
			LT_INVALIDPARAMS - pValues was NULL, nDeviceIndex specified an invalid device, 
				or the range [nFirstObjectIndex..nFirstObjectIndex + nNumObjects - 1] specified an 
				invalid device object
			LT_RETRY - Device is not currently available.  Try again later.
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT GetDeviceValues(uint32 nDeviceIndex, uint32 nFirstObjectIndex, uint32 nNumObjects, float *pValues) = 0;


	/* Sets the current value of a device object
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			nObjectIndex - Index of the object in the device
			fValue - The value the device object will be set to
		Returns:
			LT_INVALIDPARAMS - nDeviceIndex specified an invalid device, nObjectIndex specified an 
				invalid device object, or the specified device object was not writeable
			LT_RETRY - Device is not currently available.  Try again later.
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT SetDeviceObjectValue(uint32 nDeviceIndex, uint32 nObjectIndex, float fValue) = 0;

	/* Sets the current values of a range of device objects
		Parameters:
			nDeviceIndex - Index of the device the object belongs to
			nFirstObjectIndex - Index of the first object to modify in the device
			nNumObjects - Number of objects to modify
			pValues - An array containing the values to write into the device objects
		Returns:
			LT_INVALIDPARAMS - pValues was NULL, nDeviceIndex specified an invalid device, 
				or the range [nFirstObjectIndex..nFirstObjectIndex + nNumObjects - 1] specified an 
				invalid device object or a device object that was not writeable
			LT_RETRY - Device is not currently available.  Try again later.
			LT_OK - Operation completed successfully
	 */ 
	virtual LTRESULT SetDeviceValues(uint32 nDeviceIndex, uint32 nFirstObjectIndex, uint32 nNumObjects, float *pValues) = 0;
};

#endif // __ILTINPUT_H__
