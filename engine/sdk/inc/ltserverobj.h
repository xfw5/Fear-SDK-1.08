/*!
ltserverobj defines all the necessary things for writing an object.
*/

#ifndef __LTSERVEROBJ_H__
#define __LTSERVEROBJ_H__

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

#ifndef __ISERVERSHELL_H__
#include "iservershell.h"
#endif

#ifndef __ILTSERVER_H__
#include "iltserver.h"
#endif

/*!
Property flags.
*/
enum
{
//! Property doesn't show up in WorldEdit.
    PF_HIDDEN =           (1<<0),

//! Property is a number to use as radius for drawing circle.  There can be more than one.
    PF_RADIUS =           (1<<1),

//! Property is a vector to use as dimensions for drawing box. There can be only one.
    PF_DIMS =             (1<<2),

//! Property is a field of view.
    PF_CONEFOV =		  (1<<3),

//! Used with \b PF_DIMS.  Causes WorldEdit to show dimensions rotated with the object.
    PF_LOCALDIMS =        (1<<4),

//! This property owns the group it's in.
    PF_GROUPOWNER =       (1<<5),

	// NOTE: Bits (1<<6) through (1<<11) are reserved for the group number as described below...

//! If \b PF_CONEFOV or \b PF_RECTFOVX or \b PF_RECTFOVY is set, this defines the radius for it.
    PF_FOVFARZ =		(1<<12),

/*!
If the object is selected, WorldEdit draws a line to any objects
referenced (by name) in \b PF_OBJECTLINK properties.  It won't
draw any more than \b MAX_OBJECTLINK_OBJECTS.
*/
    PF_OBJECTLINK =       (1<<13),

//! This indicates to WorldEdit that a string property is a filename in the resource.
    PF_FILENAME =         (1<<14),

/*!
If this property is a vector and its object is on a path,
the path is drawn as a bezier curve.  The curve segment from
this object to the next is defined as
(THIS.Pos, THIS.Pos + THIS.NextTangent, NEXT.Pos + NEXT.PrevTangent, NEXT.Pos).
*/
    PF_BEZIERPREVTANGENT =        (1<<15) ,
    PF_BEZIERNEXTTANGENT =        (1<<16),

//! This string property has a populatable combobox with dropdown-list style (ie listbox, no edit control)
    PF_STATICLIST =       (1<<17),

//! This string property has a populatable combobox with dropdown style (ie listbox+edit control)
    PF_DYNAMICLIST =      (1<<18),

//! If \b PF_RECTFOVX or \b PF_RECTFOVY is set, this defines the near clip plane for it.
	PF_FOVNEARZ =		  (1<<19),

/*!
This property defines a measurement or other value that is in,
or relative to, world coordinates.  If this flag is specified,
any scaling done to the world as a whole for unit conversion
will also be applied to this property.
*/
    PF_DISTANCE =         (1<<20),

/*!
This property defines the filename of a model to be displayed
for the object. This is usually used in conjunction with PF_FILENAME.
If the path is not absolute, it will append the filename to the
project directory. If no extension, or LTB is provided, it will look
first for an LTA file, then an LTC.
*/
    PF_MODEL =			(1<<21),

/*!
This property defines that the associated vector should be used
to render an orthographic frustum from the object. This frustum will
have the width and height specified in the X and Y values of the vector
and the far clip plane specified in the Z value.
*/
    PF_ORTHOFRUSTUM =	(1<<22),

/*!
If this property changes, PreHook_PropChanged() will be called to give it's object
a chance to debug the new value.
*/

	PF_NOTIFYCHANGE	=	(1<<23),

/*!
This value specifies that the group should be treated as an event, and use the appropriate
event editor dialog to edit the fields.
*/

	PF_EVENT		=	(1<<24),

/*!
This specifies that a rectangular FOV should be used with this value being the X field
of view. This is often used with PF_RECTFOVY which specifies the Y field of view in degrees.
*/
	PF_RECTFOVX =		(1<<25),
	PF_RECTFOVY =		(1<<26),


/*!
This specifies that the associated PT_ROTATION property indicates a relative rotation to the
object that can be used for a secondary orientation. This will draw the forward axis of the 
second rotation as an arrow emitting from center of the object.
*/
	PF_RELATIVEORIENTATION =		(1<<27)

};


//the number of bits we have allocated for the groups (0 is not a valid group)
#define NUM_GROUP_BITS		6

//starting bit to store the groups in
#define FIRST_GROUP_BIT		6

//helper macro to specify the appropriate group
#define PF_GROUP(GroupNum)	(((GroupNum) & ((1 << NUM_GROUP_BITS) - 1)) << FIRST_GROUP_BIT)

//mask for pulling out all the group bits
#define PF_GROUPMASK		(((1 << NUM_GROUP_BITS) - 1) << FIRST_GROUP_BIT)

//helper macro to get the group out of a flag
#define GET_PF_GROUP(Flags)	((Flags & PF_GROUPMASK) >> FIRST_GROUP_BIT)
    
/*!
Class flags.
*/
enum
{
//! Instances of the class can't be created in WorldEdit.
    CF_HIDDEN =       (1<<0),

/*!
This class doesn't get used at runtime (the engine
won't instantiate these objects out of the world file).
*/
    CF_NORUNTIME =    (1<<1),

/*!
Objects of this class and sub-classes are always loaded from the level
file and can't be saved to a save game.
*/
    CF_DONTSAVE =   (1<<3),

/*!
This class is a worldmodel.  This is used to control
the binding of the geometry to the object at processing time.
*/
	CF_WORLDMODEL =	  (1<<4),

};

//determine if we want to pack description strings into the binary or not (this should
//be taken out of final builds or builds that cannot be loaded by tools to reduce the binary size)
#if !defined(_FINAL) && !defined(_SEM)
#	define COMPILE_OBJECT_DESCRIPTIONS
#endif

/*!
These are your (optional) construct/destruct functions.
In C++, you should \em always use the default ones, because they
will call your constructor/destructor.  In C, you can use
them to init and term your object.  In most cases, you should
add any aggregates you have in your construct function, since
ReadProp/PostPropRead get called right after and your aggregates
might want to look at them.
*/
typedef void (*ConstructObjectFn)(void *pObject);
typedef void (*DestructObjectFn)(void *pObject);

/*!
Callbacks used for creating plugins.
*/
class IObjectPlugin;
typedef IObjectPlugin* (*CreateObjectPluginFn)();

/*!
Callbacks used for gathering resources for the prefetching system. This will provide an interface
that the game code can then use to register resources that it uses
*/
class IObjectResourceGatherer;
typedef void (*GatherObjectResourcesFn)(const char* pszObjectName, IObjectResourceGatherer* pInterface);

/*!
Forward declaration of class WorldEdit internally augments \b PropDef data with
*/
class DEditInternal;

struct PropDef {

    PropDef(const char *pName, short type, const LTVector& valVec,
        float valFloat, const char *valString, unsigned long propFlags, const char* pszDesc )
    {
        m_PropName				= pName;
		m_PropDesc				= pszDesc;
        m_PropType				= type;
        m_DefaultValueVector	= valVec;
        m_DefaultValueFloat		= valFloat;
        m_DefaultValueString	= valString;
        m_PropFlags				= propFlags;
        m_pInternal				= NULL;
        m_pDEditInternal		= NULL;
    }

/*!
The textual name of this property
*/
   const char*		m_PropName;

/*!
The textual description of this property
*/
   const char*		m_PropDesc;

/*!
One of the \b PT_ defines above.
*/
    short			m_PropType;

/*!
Default vector/color value.
*/
    LTVector		m_DefaultValueVector;

/*!
Default float value.
*/
    float			m_DefaultValueFloat;

/*!
Default string value.
*/
    const char*		m_DefaultValueString;

/*!
The flags for this property
*/
   uint32			m_PropFlags;

/*!
Internal WorldEdit information, do not modify
*/
    DEditInternal*	m_pDEditInternal;

/*!
Internal pointer, do not modify
*/
    void *m_pInternal;

};


class CProjectClass;

struct ClassDef 
{
public:

    CProjectClass *DEditGetClassHook() {
        return (CProjectClass*)m_pInternal[0];
    }

    void DEditSetClassHook(CProjectClass *pClass) {
        m_pInternal[0] = pClass;
    }

public:

    const char*				m_ClassName;
	const char*				m_ClassDesc;
    ClassDef*				m_ParentClass;

/*!
A combination of the \b CF_ flags above.
*/
    uint32					m_ClassFlags;

    ConstructObjectFn		m_ConstructFn;
    DestructObjectFn		m_DestructFn;
    CreateObjectPluginFn	m_PluginFn;
	GatherObjectResourcesFn	m_GatherResourcesFn;

    uint32					m_nProps;
    PropDef*				m_Props;

/*!
How big an object of this class is (set automatically).
*/
    long m_ClassObjectSize;

/*!
Don't touch!
*/
    void *m_pInternal[2];
};

/*!
This is always available, once you create your server shell.
*/
extern ILTServer *g_pLTServer;

/*!
Used to avoid crashes from version mismatches.
*/
#define SERVEROBJ_VERSION 1

/*!
In C++, you can use this macro instead of the \b BEGIN_CLASSDEFS macros
so you don't have to add each object to the list.
*/
class __ClassDefiner;
extern __ClassDefiner *__g_ClassDefinerHead;

class __ClassDefiner
{
    public:

        __ClassDefiner(ClassDef *pDef)
        {
            m_pClass = pDef;
            m_pNext = __g_ClassDefinerHead;
            __g_ClassDefinerHead = this;
        }

        ClassDef        *m_pClass;
        __ClassDefiner  *m_pNext;

};

extern ClassDef **__g_cpp_classlist;
class __cpp_classlist_auto_free {
    public:
        ~__cpp_classlist_auto_free() {
            if (__g_cpp_classlist) {
                free(__g_cpp_classlist);
                __g_cpp_classlist = 0;
            }
        }
};

#define DEFINE_CLASSES() \
    ClassDef **__g_cpp_classlist=0;\
    __cpp_classlist_auto_free __free_the_g_cpp_classlist;\
    BEGIN_EXTERNC() \
        MODULE_EXPORT ClassDef** ObjectDLLSetup(int *nDefs, ILTServer *pServer, int *version); \
    END_EXTERNC() \
    __ClassDefiner *__g_ClassDefinerHead=0;\
    ClassDef** ObjectDLLSetup(int *nDefs, ILTServer *pServer, int *version) \
    {\
        int nClasses;\
        __ClassDefiner *pCurDefiner;\
        *version = SERVEROBJ_VERSION;\
        g_pLTServer = pServer;\
        nClasses=0;\
        pCurDefiner = __g_ClassDefinerHead;\
        while (pCurDefiner)\
        {\
            pCurDefiner = pCurDefiner->m_pNext;\
            ++nClasses;\
        }\
        __g_cpp_classlist = (ClassDef**)malloc(sizeof(ClassDef*) * nClasses);\
        nClasses=0;\
        pCurDefiner = __g_ClassDefinerHead;\
        while (pCurDefiner)\
        {\
            __g_cpp_classlist[nClasses] = pCurDefiner->m_pClass;\
            pCurDefiner = pCurDefiner->m_pNext;\
            ++nClasses;\
        }\
        *nDefs = nClasses; \
        return __g_cpp_classlist; \
    }

    inline void* operator new(size_t size, void *ptr, int asdf, char a)
    {
		LTUNREFERENCED_PARAMETER(size);
		LTUNREFERENCED_PARAMETER(ptr);
		LTUNREFERENCED_PARAMETER(asdf);
		LTUNREFERENCED_PARAMETER(a);

        return ptr;
    }

    #if _MSC_VER != 1100
        inline void operator delete(void *pDataPtr, void *ptr, int asdf, char a)
        {
			LTUNREFERENCED_PARAMETER(pDataPtr);
			LTUNREFERENCED_PARAMETER(ptr);
			LTUNREFERENCED_PARAMETER(asdf);
			LTUNREFERENCED_PARAMETER(a);
        }
    #endif

/*!
If you want level designers to be able to set object flags, you should
use these macros so the property names are standardized.
*/

#if defined(COMPILE_OBJECT_DESCRIPTIONS)
	#define ADD_PROP_FLAG(PropName, PropType, VecDefault, FloatDefault, StrDefault, PropFlags, PropDesc)	\
		PropDef(#PropName, PropType, VecDefault, FloatDefault, StrDefault, PropFlags, PropDesc),
#else
	#define ADD_PROP_FLAG(PropName, PropType, VecDefault, FloatDefault, StrDefault, PropFlags, PropDesc)	\
		PropDef(#PropName, PropType, VecDefault, FloatDefault, StrDefault, PropFlags, ""),
#endif
	

/*!
Use the \b ADD_X_FLAG macros in your property list. 
Note that the name of the Prop Flag added cannot have spaces, or ButeMgr breaks.
*/
#define ADD_VISIBLE_FLAG(defVal, flags) \
    ADD_BOOLPROP_FLAG(Visible, defVal, flags, "Determines if this object is visible by default")

#define ADD_SHADOW_FLAG(defVal, flags) \
    ADD_BOOLPROP_FLAG(Shadow, defVal, flags, "Determines if this object casts a shadow")

#define ADD_SOLID_FLAG(defVal, flags) \
    ADD_BOOLPROP_FLAG(Solid, defVal, flags, "Determines if the object should obstruct movement of other objects")

#define ADD_GRAVITY_FLAG(defVal, flags) \
    ADD_BOOLPROP_FLAG(Gravity, defVal, flags, "Determines if gravity should be applied to this object")

#define ADD_RAYHIT_FLAG(defVal, flags) \
    ADD_BOOLPROP_FLAG(Rayhit, defVal, flags, "Determines if this object can be hit by ray intersection tests")

/*!
Full property definitions.
*/
#define ADD_REALPROP_FLAG(name, val, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_REAL, LTVector(0.0f, 0.0f, 0.0f), val, NULL, flags, description)

#define ADD_STRINGPROP_FLAG(name, val, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_STRING, LTVector(0.0f, 0.0f, 0.0f), 0.0f, val, flags, description)

#define ADD_VECTORPROP_FLAG(name, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_VECTOR, LTVector(0.0f, 0.0f, 0.0f), 0.0f, NULL, flags, description)

#define ADD_VECTORPROP_VAL_FLAG(name, defX, defY, defZ, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_VECTOR, LTVector(defX, defY, defZ), 0.0f, NULL, flags, description)

#define ADD_LONGINTPROP_FLAG(name, val, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_LONGINT, LTVector(0.0f, 0.0f, 0.0f), (float)val, NULL, flags, description)

#define ADD_ROTATIONPROP_FLAG(name, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_ROTATION, LTVector(0.0f, 0.0f, 0.0f), 0.0f, NULL, flags, description)

#define ADD_ROTATIONPROP_VAL_FLAG(name, defX, defY, defZ, flags, description) \
	ADD_PROP_FLAG(name, LT_PT_ROTATION, LTVector(defX, defY, defZ), 0.0f, NULL, flags, description)

#define ADD_BOOLPROP_FLAG(name, val, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_BOOL, LTVector(0.0f, 0.0f, 0.0f), (float)val, NULL, flags, description)

#define ADD_COLORPROP_FLAG(name, valR, valG, valB, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_COLOR, LTVector(valR, valG, valB), 0.0f, NULL, flags, description)

#define ADD_OBJECTPROP_FLAG(name, val, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_STRING, LTVector(0.0f, 0.0f, 0.0f), 0.0f, val, flags | PF_OBJECTLINK, description)

#define ADD_COMMANDPROP_FLAG(name, val, flags, description) \
	ADD_PROP_FLAG(name, LT_PT_COMMAND, LTVector(0.0f, 0.0f, 0.0f), 0.0f, val, flags, description)

#define ADD_STRINGIDPROP_FLAG(name, val, flags, description) \
    ADD_PROP_FLAG(name, LT_PT_STRINGID, LTVector(0.0f, 0.0f, 0.0f), 0.0f, val, flags, description)

/*!
Add properties without flags (only here for backward compatibility).
*/
#define ADD_REALPROP(name, val, description) \
    ADD_REALPROP_FLAG(name, val, 0, description)

#define ADD_STRINGPROP(name, val, description) \
    ADD_STRINGPROP_FLAG(name, val, 0, description)

#define ADD_VECTORPROP(name, description) \
    ADD_VECTORPROP_FLAG(name, 0, description)

#define ADD_VECTORPROP_VAL(name, defX, defY, defZ, description) \
    ADD_VECTORPROP_VAL_FLAG(name, defX, defY, defZ, 0, description)

#define ADD_LONGINTPROP(name, val, description) \
    ADD_LONGINTPROP_FLAG(name, val, 0, description)

#define ADD_ROTATIONPROP(name, description) \
    ADD_ROTATIONPROP_FLAG(name, 0, description)

#define ADD_BOOLPROP(name, val, description) \
    ADD_BOOLPROP_FLAG(name, val, 0, description)

#define ADD_COLORPROP(name, valR, valG, valB, description) \
    ADD_COLORPROP_FLAG(name, valR, valG, valB, 0, description)

#define ADD_OBJECTPROP(name, val, description) \
    ADD_OBJECTPROP_FLAG(name, val, 0, description)

#define ADD_COMMANDPROP(name, val, description) \
	ADD_COMMANDPROP_FLAG(name, val, 0, description)

/*!
Define a group with this.
*/
#define PROP_DEFINEGROUP(groupName, groupFlag, description) \
    PropDef(#groupName, LT_PT_LONGINT, LTVector(0.0f, 0.0f, 0.0f), (float)0.0f, (char*)0, groupFlag|PF_GROUPOWNER, description),

#define BEGIN_CLASS(name) \
    static PropDef  _##name##_Props__[] = { \
        ADD_STRINGPROP("__NOPROP__!!", "", "")

/*!
Define the default constructor/destructor functions.
*/
#define DO_AUTO_CLASSLIST(name) \
    static __ClassDefiner __##name##_definer(&_##name##_Class__);

#define INTERNAL_END_CLASS_SYMBOL(ClassName, parentSymbol, flags, pluginFn, prefetchFn, description) \
	void Default##ClassName##Constructor(void *ptr)\
	{\
		::new(ptr, (int)0, (char)0) ClassName;\
	}\
	void Default##ClassName##Destructor(void *ptr)\
	{\
		ClassName *thePtr = (ClassName*)ptr;\
		thePtr->~ClassName();\
	}\
    ClassDef _##ClassName##_Class__ = { \
        #ClassName, \
		description, \
		parentSymbol, \
        flags,\
        Default##ClassName##Constructor,\
		Default##ClassName##Destructor, \
        pluginFn,\
		prefetchFn,\
        (sizeof(_##ClassName##_Props__) / sizeof(PropDef)) - 1,\
        &_##ClassName##_Props__[1], \
        sizeof(ClassName),\
        (void*)0, (void*)0 }; \
        DO_AUTO_CLASSLIST(ClassName)\


#if defined(COMPILE_OBJECT_DESCRIPTIONS)
	#define END_CLASS_SYMBOL(ClassName, parentSymbol, flags, pluginFn, prefetchFn, description) \
		INTERNAL_END_CLASS_SYMBOL(ClassName, parentSymbol, flags, pluginFn, prefetchFn, description)
#else
	#define END_CLASS_SYMBOL(ClassName, parentSymbol, flags, pluginFn, prefetchFn, description) \
		INTERNAL_END_CLASS_SYMBOL(ClassName, parentSymbol, flags, pluginFn, prefetchFn, "")
#endif

/*!
End class macros with a plugin interface and a function to collect resources.
Just specify the plugin class name.  The class must derive from IObjectPlugin.
*/
#define END_CLASS_FLAGS_PLUGIN_PREFETCH(name, parent, flags, pluginClassName, prefetchFn, description)\
	}; \
	extern ClassDef _##parent##_Class__; \
	IObjectPlugin* __Create##name##__Plugin() {return new pluginClassName;}\
	END_CLASS_SYMBOL(name, &_##parent##_Class__, flags, __Create##name##__Plugin, prefetchFn, description)


/*!
End class macros with a plugin interface.
Just specify the plugin class name.  The class must derive from IObjectPlugin.
*/
#define END_CLASS_FLAGS_PLUGIN(name, parent, flags, pluginClassName, description)\
    }; \
    extern ClassDef _##parent##_Class__; \
    IObjectPlugin* __Create##name##__Plugin() {return new pluginClassName;}\
    END_CLASS_SYMBOL(name, &_##parent##_Class__, flags, __Create##name##__Plugin, NULL, description)

/*!
End class macros with flags and a resource gathering function.
*/
#define END_CLASS_FLAGS_PREFETCH(name, parent, flags, prefetchFn, description)\
	}; \
	extern ClassDef _##parent##_Class__; \
	END_CLASS_SYMBOL(name, &_##parent##_Class__, flags, NULL, prefetchFn, description)

/*!
End class macros with flags.
*/
#define END_CLASS_FLAGS(name, parent, flags, description)\
    }; \
    extern ClassDef _##parent##_Class__; \
    END_CLASS_SYMBOL(name, &_##parent##_Class__, flags, NULL, NULL, description)

/*!
Normal end class macros.
*/
#define END_CLASS(name, parent, description)\
    END_CLASS_FLAGS(name, parent, 0, description)

/*!
Only used internally.
*/
#define END_CLASS_NOPARENT(name, description) \
    }; \
    END_CLASS_SYMBOL(name, NULL, 0, NULL, NULL, description)

/*!
An interface class for commands to derive from that exposes critical information to WorldEdit.
*/
class ICommandDef
{
	public:

		ICommandDef( const char *pszName = "", const char *pszSyntax = "", const char *pszDesciption = "" )
		:	m_pszName		( pszName ),
			m_pszSyntax		( pszSyntax ),
			m_pszDescription( pszDesciption )
		{ }

		const char	*m_pszName;
		const char	*m_pszSyntax;
		const char	*m_pszDescription;
};

/*!
An interface class for command messages to derive from that exposes critical information to WorldEdit.
*/
class ICommandMessageDef
{
	public:

		ICommandMessageDef( const char *pszName = "", const char *pszSyntax = "", const char *pszDesciption = "", const char *pszExample = "" )
		:	m_pszName		( pszName ),
			m_pszSyntax		( pszSyntax ),
			m_pszDescription( pszDesciption ),
			m_pszExample	( pszExample )
		{ }

		const char	*m_pszName;
		const char	*m_pszSyntax;
		const char	*m_pszDescription;
		const char	*m_pszExample;
};

/*!
An interface class for command classes to derive from that exposes critical information to WorldEdit.
*/
class ICommandClassDef
{
public:

	ICommandClassDef( const char *pszName = "", const char *pszParent = "", uint32 nNumMsgs = 0 )
		:	m_pszName	( pszName ),
			m_pszParent	( pszParent ),
			m_nNumMsgs	( nNumMsgs ),
			m_pMsgDefs	( NULL )
	{ }

	const char			*m_pszName;
	const char			*m_pszParent;
	uint32				m_nNumMsgs;
	ICommandMessageDef	*m_pMsgDefs;
};

#endif  //! __LTSERVEROBJ_H__
 

 



