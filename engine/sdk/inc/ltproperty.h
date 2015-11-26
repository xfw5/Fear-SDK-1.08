//////////////////////////////////////////////////////////////////////////////
// Property declarations and types

#pragma warning (disable:4786) // No truncation warnings, please..

#ifndef __LTPROPERTY_H__
#define __LTPROPERTY_H__

#include <ctype.h>
#include <stdio.h>

/*!
Different property types.
*/
enum
{
    LT_PT_STRING        = 0,
    LT_PT_VECTOR        = 1,
    LT_PT_COLOR         = 2,
    LT_PT_REAL          = 3,
    LT_PT_BOOL          = 4,
    LT_PT_LONGINT       = 5,
    LT_PT_ROTATION      = 6,
    LT_PT_COMMAND       = 7,
	LT_PT_STRINGID      = 8,
    LT_NUM_PROPERTYTYPES = 9  
};
    
/*!
GenericProp -
Here's a list of which property types map to which variables:

LT_PT_STRING

- m_Vec if it's formatted like '1 2 3', m_String, m_Long, m_Float, m_Bool.
- If this is 'true' or 'false', it will set m_Long, m_Float, and m_Bool to 0 or 1.

LT_PT_VECTOR   - m_Vec, m_String ('1 2 3')
LT_PT_COLOR    - m_Vec, m_String ('1 2 3')
LT_PT_REAL     - m_String, m_Long, m_Float, m_Bool
LT_PT_BOOL     - m_String, m_Long, m_Float, m_Bool
LT_PT_LONGINT - m_String, m_Long, m_Float, m_Bool
LT_PT_ROTATION - m_Rotation and m_Vec (with euler rotation).
*/

#define MAX_GP_STRING_LEN	256

//the maximum length for a property name including the null terminator. Property names
//must be less than this otherwise they are not guaranteed to be loaded or accessed properly
#define MAX_PROP_NAME_LEN	32

struct GenericProp 
{
	GenericProp() 
	{
		InitString();
	}

	~GenericProp()
	{
		FreeStringValue();
	}

	//------------------------------------------------------
	// Copy operations

	GenericProp(const GenericProp &cOther) 
	{ 
		m_Type = cOther.m_Type;
		m_Vec = cOther.m_Vec;
		m_Rotation = cOther.m_Rotation;
		m_Long = cOther.m_Long;
		m_Float = cOther.m_Float;
		m_Bool = cOther.m_Bool;

		InitString();
		SetStringValue(cOther.m_pszString); 
	}

	GenericProp &operator=(const GenericProp &cOther)
	{
		if (&cOther != this)
		{
			this->~GenericProp();
			new (this) GenericProp(cOther);
		}
		return *this;
	}

	//------------------------------------------------------
	// Creation and initialization

	//create as a vector
	GenericProp(const LTVector &vValue, uint32 nType) : 
		m_Type(nType),
		m_Vec(vValue)
	{
		InitString();
	}

	//create as a float
	GenericProp(float fValue, uint32 nType) : 
		m_Type(nType),
		m_Float(fValue)
	{
		InitString();
	}

	//create as an integer
	GenericProp(int32 nValue, uint32 nType) : 
		m_Type(nType),
		m_Long(nValue)
	{
		InitString();
	}

	//create as a rotation
	GenericProp(const LTRotation &rRotation, uint32 nType) : 
		m_Type(nType),
		m_Rotation(rRotation)
	{
		InitString();
	}

	//create as a boolean
	GenericProp(bool bValue, uint32 nType) : 
		m_Type(nType),
		m_Bool(bValue)
	{
		InitString();
	}

	//create as a string
	GenericProp(const char* pszString, uint32 nType) :
		m_Type(nType)
	{
		InitString();
		SetStringValue(pszString);
	}

	//------------------------------------------------------
	// Access to set individual property types

	void SetLongInt(int32 nVal)
	{ 
		m_Type = LT_PT_LONGINT;
		m_Long = nVal; 
	}

	void SetReal(float fVal)
	{ 
		m_Type = LT_PT_REAL;
		m_Float = fVal; 
	}

	void SetBool(bool bVal)
	{ 
		m_Type = LT_PT_BOOL;
		m_Bool = bVal; 
	}

	void SetRotation(const LTRotation& rRot) 		
	{ 
		m_Type = LT_PT_ROTATION;
		m_Rotation = rRot; 
	}

	void SetVector(const LTVector& vVec)		
	{ 
		m_Type = LT_PT_VECTOR;
		m_Vec = vVec;
	}

	void SetColor(const LTVector& vColor)		
	{ 
		m_Type = LT_PT_COLOR;
		m_Vec = vColor;
	}

	void SetString(const char* pszStr)
	{ 
		m_Type = LT_PT_STRING;
		SetStringValue(pszStr);
	}

	void SetCommand(const char* pszStr)
	{ 
		m_Type = LT_PT_COMMAND;
		SetStringValue(pszStr);
	}

	void SetStringID(const char* pszStr)
	{ 
		m_Type = LT_PT_STRINGID;
		SetStringValue(pszStr);
	}

	//------------------------------------------------------
	// Access to read individual property types

	uint32 GetType() const			
	{ 
		return m_Type; 
	}

	int32 GetLongInt() const		
	{ 
		LTASSERT(GetType() == LT_PT_LONGINT, "Error: Invalid long int access");
		return m_Long; 
	}

	float GetReal() const		
	{ 
		LTASSERT(GetType() == LT_PT_REAL, "Error: Invalid float access");
		return m_Float; 
	}

	bool GetBool() const		
	{ 
		LTASSERT(GetType() == LT_PT_BOOL, "Error: Invalid boolean access");
		return m_Bool; 
	}

	const LTRotation& GetRotation() const		
	{ 
		LTASSERT(GetType() == LT_PT_ROTATION, "Error: Invalid rotation access");
		return m_Rotation; 
	}

	const LTVector& GetVector() const		
	{ 
		LTASSERT(GetType() == LT_PT_VECTOR, "Error: Invalid vector access");
		return m_Vec; 
	}

	const LTVector& GetColor() const		
	{ 
		LTASSERT(GetType() == LT_PT_COLOR, "Error: Invalid color access");
		return m_Vec; 
	}

	const char* GetString() const		
	{ 
		LTASSERT(GetType() == LT_PT_STRING, "Error: Invalid string access");
		return m_pszString; 
	}

	const char* GetCommand() const		
	{ 
		LTASSERT(GetType() == LT_PT_COMMAND, "Error: Invalid command access");
		return m_pszString; 
	}

	const char* GetStringID() const		
	{ 
		LTASSERT(GetType() == LT_PT_STRINGID, "Error: Invalid string id access");
		return m_pszString; 
	}

	//------------------------------------------------------
	// Initializes a property from a string

	void FromString(uint32 nType, const char* pszString)
	{
		//copy the type
		m_Type = nType;

		//and now setup our data based upon the string
		switch(nType)
		{
		case LT_PT_STRING:
		case LT_PT_COMMAND:
		case LT_PT_STRINGID:
			SetStringValue(pszString);
			break;
		case LT_PT_VECTOR:
		case LT_PT_COLOR:
			sscanf(pszString, "%f %f %f", &m_Vec.x, &m_Vec.y, &m_Vec.z);
			break;
		case LT_PT_ROTATION:
			{
				LTVector vAngles;
				sscanf(pszString, "%f %f %f", &vAngles.x, &vAngles.y, &vAngles.z);
				m_Rotation = LTRotation(VEC_EXPAND(vAngles));
			}
			break;
		case LT_PT_REAL:
			m_Float = (float)atof(pszString);
			break;
		case LT_PT_LONGINT:
			m_Long = (int32)atoi(pszString);
			break;
		case LT_PT_BOOL:
			m_Bool = ((int32)atoi(pszString) != 0);
			break;
		}
	}

private:

	//called to initialize the string buffer to point to an string buffer
	void InitString()	
	{ 
		LTStrCpy(m_szStringBuffer, "", LTARRAYSIZE(m_szStringBuffer)); 
		m_pszString = m_szStringBuffer; 
	}

	//called to set the string to the specified value
	void SetStringValue(const char* pszValue)
	{
		FreeStringValue();

		if(LTStrLen(pszValue) + 1 > LTARRAYSIZE(m_szStringBuffer))
		{
			m_pszString = LTStrDup(pszValue);
		}
		else
		{
			LTStrCpy(m_szStringBuffer, pszValue, LTARRAYSIZE(m_szStringBuffer));
			m_pszString = m_szStringBuffer;
		}
	}

	//called to free the string
	void FreeStringValue()
	{
		if(m_pszString != m_szStringBuffer)
			delete [] m_pszString;
		InitString();
	}

	// Type of the property
	uint32 m_Type;

	// Storage for LT_PT_VECTOR
	LTVector m_Vec;

	// Storage for LT_PT_ROTATION
	LTRotation m_Rotation;
	// Storage for LT_PT_LONGINT
	int32 m_Long;
	// Storage for LT_PT_REAL
	float m_Float;
	// Storage for LT_PT_BOOL
	bool m_Bool;

	//the strings are private because it can either be placed into a) an internal small buffer,
	//or b) a block of allocated memory. This allows for unlimited length strings without needing to
	//perform numerous allocations

	//this points to the actual buffer to be used (either the internal buffer or allocated memory)
	char*		m_pszString;

	//the internal buffer that can be used to hold onto short strings
	char		m_szStringBuffer[MAX_GP_STRING_LEN+1];
};

class GenericPropList
{
public:
	GenericPropList() : 
		m_nNumProps(0), m_nMaxProps(0), m_pProps(NULL), m_pPropIndices(NULL) 
	{
	}

	GenericPropList(const GenericPropList &cOther) : 
		m_nNumProps(0), m_nMaxProps(0), m_pProps(NULL), m_pPropIndices(NULL)
	{
		if(cOther.GetMaxProps())
		{
			if(ReserveProps(cOther.GetNumProps(), false))
			{
				//copy over the properties and init our indices
				for(uint32 nCurrProp = 0; nCurrProp < GetMaxProps(); nCurrProp++)
				{
					LTStrCpy(m_pProps[nCurrProp].m_pszName, cOther.m_pPropIndices[nCurrProp]->m_pszName, MAX_PROP_NAME_LEN);
					m_pProps[nCurrProp].m_Prop = cOther.m_pPropIndices[nCurrProp]->m_Prop;

					m_pPropIndices[nCurrProp] = &m_pProps[nCurrProp];
				}
				m_nNumProps = cOther.GetNumProps();
			}
		}
	}

	~GenericPropList() { FreeProps(); }

	GenericPropList &operator=(const GenericPropList &cOther) 
	{
		if (this == &cOther)
			return *this;

		if(cOther.GetMaxProps())
		{
			if(ReserveProps(cOther.GetNumProps(), false))
			{
				//copy over the properties and init our indices
				for(uint32 nCurrProp = 0; nCurrProp < cOther.GetNumProps(); nCurrProp++)
				{
					LTStrCpy(m_pProps[nCurrProp].m_pszName, cOther.m_pPropIndices[nCurrProp]->m_pszName, MAX_PROP_NAME_LEN);
					m_pProps[nCurrProp].m_Prop = cOther.m_pPropIndices[nCurrProp]->m_Prop;

					m_pPropIndices[nCurrProp] = &m_pProps[nCurrProp];
				}
				m_nNumProps = cOther.GetNumProps();
			}
		}
		else
		{
			Reset();
		}

		return *this;
	}
/*! 
Get the LT_PT_VECTOR property associated with pName.
Returns vDefault if that property was not found.
*/
	LTVector GetVector(const char *pName, const LTVector &vDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_VECTOR))
			return vDefault;
		return pResult->GetVector();
	}
/*! 
Get the LT_PT_COLOR property associated with pName.
Returns vDefault if that property was not found.
*/
	LTVector GetColor(const char *pName, const LTVector &vDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_COLOR))
			return vDefault;
		return pResult->GetColor();
	}
/*! 
Get the LT_PT_STRING property associated with pName.
Returns pDefault if that property was not found.
*/
	const char *GetString(const char *pName, const char *pDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_STRING))
			return pDefault;
		return pResult->GetString();
	}
/*! 
Get the LT_PT_ROTATION property associated with pName.
Returns rDefault if that property was not found.
*/
	LTRotation GetRotation(const char *pName, const LTRotation &rDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_ROTATION))
			return rDefault;
		return pResult->GetRotation();
	}
/*! 
Get the LT_PT_LONGINT property associated with pName.
Returns nDefault if that property was not found.
*/
	int32 GetLongInt(const char *pName, int32 nDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_LONGINT))
			return nDefault;
		return pResult->GetLongInt();
	}
/*! 
Get the LT_PT_REAL property associated with pName.
Returns fDefault if that property was not found.
*/
	float GetReal(const char *pName, float fDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_REAL))
			return fDefault;
		return pResult->GetReal();
	}
/*! 
Get the LT_PT_BOOL property associated with pName.
Returns bDefault if that property was not found.
*/
	bool GetBool(const char *pName, bool bDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_BOOL))
			return bDefault;
		return pResult->GetBool();
	}
/*! 
Get the LT_PT_COMMAND property associated with pName.
Returns pDefault if that property was not found.
*/
	const char *GetCommand(const char *pName, const char *pDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_COMMAND))
			return pDefault;
		return pResult->GetCommand();
	}
/*! 
Get the LT_PT_STRINGID property associated with pName.
Returns pDefault if that property was not found.
*/
	const char *GetStringID(const char *pName, const char *pDefault) const { 
		const GenericProp *pResult = GetProp(pName);
		if (!pResult || (pResult->GetType() != LT_PT_STRINGID))
			return pDefault;
		return pResult->GetStringID();
	}

public:

	//This will reset the properties in the list. No memory is freed by this
	void	Reset()						{ m_nNumProps = 0; }

	//Determines the number of properties that this list can hold
	uint32	GetNumProps() const			{ return m_nNumProps; }

	//Determines the number of properties that this list can hold
	uint32	GetMaxProps() const			{ return m_nMaxProps; }

	//Sets the maximum number of properties that this list can hold
	bool	ReserveProps(uint32 nMax, bool bPreserveProps)	
	{
		//see if we need to reset the number of properties
		if(!bPreserveProps)
			m_nNumProps = 0;

		//see if we can avoid the allocation
		if(nMax == m_nMaxProps)
		{
			return true;
		}

		SPropEntry*		pNewProps;
		SPropEntry**	pNewPropIndices;
		
		LT_MEM_TRACK_ALLOC(pNewProps = new SPropEntry[nMax], LT_MEM_TYPE_OBJECT);
		LT_MEM_TRACK_ALLOC(pNewPropIndices = new SPropEntry*[nMax], LT_MEM_TYPE_OBJECT);

		//check the allocation
		if(!pNewProps || !pNewPropIndices)
		{
			delete [] pNewProps;
			delete [] pNewPropIndices;
			return false;
		}

		//now copy over the properties (note that m_nNumProps will always be 0 if we are not
		//preserving)
		uint32 nNumToCopy = LTMIN(m_nNumProps, nMax);

		for(uint32 nCurrProp = 0; nCurrProp < nNumToCopy; nCurrProp++)
		{
			pNewPropIndices[nCurrProp] = &pNewProps[nCurrProp];

			LTStrCpy(pNewProps[nCurrProp].m_pszName, m_pPropIndices[nCurrProp]->m_pszName, MAX_PROP_NAME_LEN);
			pNewProps[nCurrProp].m_Prop = m_pPropIndices[nCurrProp]->m_Prop;
		}

		//clear out the old
		FreeProps();

		//this is important that this comes after the free props since that clears out the
		//number of properties.
		m_nNumProps = nNumToCopy;

		//setup the new
		m_pProps = pNewProps;
		m_pPropIndices = pNewPropIndices;

		m_nMaxProps = nMax;

		return true;
	}

	//adds a property into the list, and returns a pointer to that property, or NULL if it could
	//not be inserted
	GenericProp*	AddProp(const char* pszName)
	{
		//see where this property should go using a binary search
		int32 nLower = 0;
		int32 nUpper = (int32)GetNumProps() - 1;

		int32 nMiddle;
		int32 nDiff;

		while(nLower <= nUpper)
		{
			nMiddle = (nLower + nUpper) / 2;
			nDiff = LTStrICmp(pszName, m_pPropIndices[nMiddle]->m_pszName);

			if(nDiff == 0)
			{
				//the item already exists
				return &m_pPropIndices[nMiddle]->m_Prop;
			}

			if(nDiff < 0)
				nUpper = nMiddle - 1;
			else
				nLower = nMiddle + 1;
		}

		//insert it at the larger position
		uint32 nPropLocation = (uint32)LTMAX(nUpper, nLower);

		//make sure we have room
		if(GetNumProps() >= GetMaxProps())
		{
			static const int knExtraReserveBuffer = 5;
			if(!ReserveProps(GetNumProps() + 1 + knExtraReserveBuffer, true))
				return NULL;
		}

		//alright, we now need to move everything down to create room
		for(uint32 nCurrProp = GetNumProps(); nCurrProp > nPropLocation; nCurrProp--)
			m_pPropIndices[nCurrProp] = m_pPropIndices[nCurrProp - 1];

		//now add our item to the list and hook up our index
		m_pPropIndices[nPropLocation] = &m_pProps[GetNumProps()];
		LTStrCpy(m_pProps[GetNumProps()].m_pszName, pszName, MAX_PROP_NAME_LEN);

		//increment our property count
		m_nNumProps++;

		//success
		return &m_pPropIndices[nPropLocation]->m_Prop;
	}

	//adds a property into the list, this does an insertion sort
	bool	AddProp(const char* pszName, const GenericProp& Prop)
	{
		GenericProp* pProp = AddProp(pszName);
		if(!pProp)
			return false;

		*pProp = Prop;
		return true;
	}

	//gets the name of a property
	const char* GetPropName(uint32 nProp) const
	{
		if(nProp > GetNumProps())
			return "";

		return m_pProps[nProp].m_pszName;
	}

	//gets an actual property pointer
	const GenericProp* GetProp(uint32 nProp) const
	{
		if(nProp > GetNumProps())
			return NULL;

		return &m_pProps[nProp].m_Prop;
	}


/*! 
Get the property associated with pName.
Returns NULL if that property was not found.
*/
	const GenericProp *GetProp(const char *pszName) const 
	{
		//do a binary search
		int32 nLower = 0;
		int32 nUpper = (int32)GetNumProps() - 1;
		int32 nIndex;

		while(nLower <= nUpper)
		{
			nIndex = (nLower + nUpper) / 2;
			int32 nResult = LTStrICmp(pszName, m_pPropIndices[nIndex]->m_pszName);

			if(nResult < 0)
			{
				nUpper = nIndex - 1;
			}
			else if(nResult > 0)
			{
				nLower = nIndex + 1;
			}
			else
			{
				//we have a matching property
				return &m_pPropIndices[nIndex]->m_Prop;
			}
		}

		return NULL;
	}

protected:

	//frees the memory associated with this property list
	void FreeProps()
	{
		delete [] m_pProps;
		m_pProps = NULL;

		delete [] m_pPropIndices;
		m_pPropIndices = NULL;

		m_nNumProps = 0;
		m_nMaxProps = 0;
	}

	//a structure that represents a property and its name
	struct SPropEntry
	{
		char			m_pszName[MAX_PROP_NAME_LEN];
		GenericProp		m_Prop;
	};

	//the number of properties currently in the list
	uint32		m_nNumProps;

	//the maximum number of properties that we can have in the list
	uint32		m_nMaxProps;

	//the list of the actual properties and their names
	SPropEntry	*m_pProps;

	//pointers to elements in the property list so that insertion doesn't have to move so
	//much memory around
	SPropEntry	**m_pPropIndices;
};

#endif //__LTPROPERTY_H__
