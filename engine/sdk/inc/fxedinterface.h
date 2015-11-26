#ifndef __FXEDINTERFACE_H__
#define __FXEDINTERFACE_H__

//a union that represents a block of data that holds a value for a property
union UEffectPropertyData
{
	char				m_pszStringVal[256];
	int					m_nIntVal;
	float				m_fFloatVal;
	float				m_fVectorVal[3];
	uint32				m_nColorVal;
};

//describes the different property types available.
enum ECurveType
{
	eCurve_None,
	eCurve_Step,
	eCurve_Linear
};

class CEffectPropertyDesc
{
public:

	//the different property types that are allowed, each one has a different
	//data format, and different uses for auxilery data primarily for user interface
	//purposes
	enum EPropType
	{
		ePropType_Invalid,
		ePropType_String,
		ePropType_Filename,
		ePropType_Enum,
		ePropType_Int,
		ePropType_Float,
		ePropType_Color,
		ePropType_Vector,
		ePropType_UI,
		ePropType_Animation,
		ePropType_ClientFXRef,
	};

	CEffectPropertyDesc()
	{
		memset(this, 0, sizeof(CEffectPropertyDesc));
	}

	//------------------------------------
	// Integer
	//------------------------------------
	void SetupInt(const char *pszName, int32 nDefault, ECurveType eCurve, const char* pszDescription) 
	{
		SetInfo(ePropType_Int, pszName, "", pszDescription, eCurve, true);
		m_uDefault.m_nIntVal	= nDefault;
	}

	void SetupIntMin(const char *pszName, int32 nDefault, int32 nMin, ECurveType eCurve, const char* pszDescription) 
	{
		SetupInt(pszName, nDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), "%d;", nMin);
	}

	void SetupIntMax(const char *pszName, int32 nDefault, int32 nMax, ECurveType eCurve, const char* pszDescription) 
	{
		SetupInt(pszName, nDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), ";%d", nMax);
	}

	void SetupIntMinMax(const char *pszName, int32 nDefault, int32 nMin, int32 nMax, ECurveType eCurve, const char* pszDescription) 
	{
		SetupInt(pszName, nDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), "%d;%d", nMin, nMax);
	}

	//------------------------------------
	// Float
	//------------------------------------

	void SetupFloat(const char *pszName, float fDefault, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_Float, pszName, "", pszDescription, eCurve, true);
		m_uDefault.m_fFloatVal	= fDefault;
	}

	void SetupFloatMin(const char *pszName, float fDefault, float fMin, ECurveType eCurve, const char* pszDescription) 
	{
		SetupFloat(pszName, fDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), "%.6f;", fMin);
	}

	void SetupFloatMax(const char *pszName, float fDefault, float fMax, ECurveType eCurve, const char* pszDescription) 
	{
		SetupFloat(pszName, fDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), ";%.6f", fMax);
	}

	void SetupFloatMinMax(const char *pszName, float fDefault, float fMin, float fMax, ECurveType eCurve, const char* pszDescription) 
	{
		SetupFloat(pszName, fDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), "%.6f;%.6f", fMin, fMax);
	}

	//------------------------------------
	// Vector
	//------------------------------------

	void SetupVector(const char *pszName, const LTVector& vDefault, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_Vector, pszName, "", pszDescription, eCurve, true);
		m_uDefault.m_fVectorVal[0]	= vDefault.x;
		m_uDefault.m_fVectorVal[1]	= vDefault.y;
		m_uDefault.m_fVectorVal[2]	= vDefault.z;
	}

	void SetupVectorMin(const char *pszName, const LTVector& vDefault, float fMin, ECurveType eCurve, const char* pszDescription) 
	{
		SetupVector(pszName, vDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), "%.6f;", fMin);
	}

	void SetupVectorMax(const char *pszName, const LTVector& vDefault, float fMax, ECurveType eCurve, const char* pszDescription) 
	{
		SetupVector(pszName, vDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), ";%.6f", fMax);
	}

	void SetupVectorMinMax(const char *pszName, const LTVector& vDefault, float fMin, float fMax, ECurveType eCurve, const char* pszDescription) 
	{
		SetupVector(pszName, vDefault, eCurve, pszDescription);
		LTSNPrintF(m_pszInfoString, LTARRAYSIZE(m_pszInfoString), "%.6f;%.6f", fMin, fMax);
	}

	//------------------------------------
	// String
	//------------------------------------

	void SetupString(const char *pszName, const char* pszDefault, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_String, pszName, "", pszDescription, eCurve, false);
		LTStrCpy(m_uDefault.m_pszStringVal, pszDefault, LTARRAYSIZE(m_uDefault.m_pszStringVal));
	}

	//------------------------------------
	// Enumeration
	//------------------------------------

	void SetupEnum(const char *pszName, const char* pszDefault, const char* pszEnum, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_Enum, pszName, pszEnum, pszDescription, eCurve, false);
		LTStrCpy(m_uDefault.m_pszStringVal, pszDefault, LTARRAYSIZE(m_uDefault.m_pszStringVal));
	}

	//------------------------------------
	// Bool
	//------------------------------------

	void SetupEnumBool(const char* pszName, bool bDefault, ECurveType eCurve, const char* pszDescription)
	{
		SetupEnum(pszName, (bDefault) ? "Yes" : "No", "No,Yes", eCurve, pszDescription);
	}

	//------------------------------------
	// Color
	//------------------------------------

	void SetupColor(const char *pszName, uint32 nDefault, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_Color, pszName, "", pszDescription, eCurve, true);
		m_uDefault.m_nColorVal	= nDefault;
	}

	//------------------------------------
	// Filename
	//------------------------------------

	void SetupPath(const char *pszName, const char* pszDefault, const char* pszFilter, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_Filename, pszName, pszFilter, pszDescription, eCurve, false);
		LTStrCpy(m_uDefault.m_pszStringVal, pszDefault, LTARRAYSIZE(m_uDefault.m_pszStringVal));
	}

	//------------------------------------
	// Animation
	//------------------------------------

	void SetupAnimation(const char *pszName, const char* pszDefault, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_Animation, pszName, "", pszDescription, eCurve, false);
		LTStrCpy(m_uDefault.m_pszStringVal, pszDefault, LTARRAYSIZE(m_uDefault.m_pszStringVal));
	}

	//------------------------------------
	// ClientFXRef
	//------------------------------------

	void SetupClientFXRef(const char *pszName, const char* pszDefault, ECurveType eCurve, const char* pszDescription) 
	{ 
		SetInfo(ePropType_ClientFXRef, pszName, "", pszDescription, eCurve, false);
		LTStrCpy(m_uDefault.m_pszStringVal, pszDefault, LTARRAYSIZE(m_uDefault.m_pszStringVal));
	}

	//------------------------------------
	// UI Controls
	//------------------------------------

	void SetupTextLine(const char* pszText) 
	{ 
		SetInfo(ePropType_UI, "UIType_TextLine", pszText, "", eCurve_None, false);
	}

	//------------------------------------
	// Internals
	//------------------------------------

	//the name of this property
	char					m_pszName[128];

	//an auxillery information string that is used to communicate extra information about a type
	//that can be used by the tools
	char					m_pszInfoString[256];

	//a textual description string for this parameter
	char					m_pszDescription[256];
	
	//the type of this property
	EPropType				m_eType;

	//flag indicating whether or not this parameter supports a function curve
	bool					m_bFunctionCurve;

	//flag indicating whether or not this parameter is interpolated
	bool					m_bInterpolated;

	//a union that contains room to hold the default value of the parameter
	UEffectPropertyData		m_uDefault;

private:

	//Utility functions for setting up the parameter types
	void SetInfo(EPropType eType, const char* pszName, const char* pszInfoString, const char* pszDescription, ECurveType eCurve, bool bCanInterpolate)
	{
		LTStrCpy(m_pszName, pszName, LTARRAYSIZE(m_pszName));
		LTStrCpy(m_pszInfoString, pszInfoString, LTARRAYSIZE(m_pszInfoString));
		LTStrCpy(m_pszDescription, pszDescription, LTARRAYSIZE(m_pszDescription));
		m_eType = eType;
			
		//determine if it is a function curve
		if(eCurve != eCurve_None)
		{
			m_bFunctionCurve = true;
			m_bInterpolated  = false;

			if(bCanInterpolate && (eCurve == eCurve_Linear))
			{
				m_bInterpolated = true;
			}
			
			//sanity check
			LTASSERT((eCurve == eCurve_Step) || bCanInterpolate, "Error: Invalid interpolation type provided");
		}
		else
		{
			m_bFunctionCurve = false;
			m_bInterpolated  = false;
		}
	}
};

//describes an actual effect type. This has a textual name, an integer name for packing,
//and a means of acquiring a list of properties assocaited with this type
class CEffectTypeDesc
{
public:

	CEffectTypeDesc() :
		m_nIdentifier(0),
		m_nNumProperties(0),
		m_pProperties(NULL)
	{
		m_pszName[0] = '\0';
	}

	~CEffectTypeDesc()
	{
		delete [] m_pProperties;
	}

	//the textual name of this effect type
	char					m_pszName[128];

	//an integer identifier for this type for when packing
	uint32					m_nIdentifier;

	//the number of properties associated with this effect type
	uint32					m_nNumProperties;

	//a list of property descriptions
	CEffectPropertyDesc*	m_pProperties;
};

//information exported from the runtime in order to allow the tool to edit the data
class CEffectSystemDesc
{
public:

	CEffectSystemDesc() :
		m_nVersion(0),
		m_nNumEffectTypes(0),
		m_pEffectTypes(NULL)
	{
	}

	~CEffectSystemDesc()
	{
		delete [] m_pEffectTypes;
	}

	//a version number of this compiled library, used when packing to help avoid version issues
	uint32				m_nVersion;

	//the number of effect types that this system uses
	uint32				m_nNumEffectTypes;

	//a list of the different effect types
	CEffectTypeDesc*	m_pEffectTypes;
};

//function prototypes for the functions that the runtime must export
typedef void (*TFxEdRuntimeGetInfo)(CEffectSystemDesc*&);
typedef void (*TFxEdRuntimeReleaseInfo)(CEffectSystemDesc*);

//the names of the functions that match the above format
#define FXED_RUNTIME_GET_INFO_FN		"FxEdGetInfo"
#define FXED_RUNTIME_RELEASE_INFO_FN	"FxEdReleaseInfo"

#endif

