//------------------------------------------------------------------------------------
// LTMemoryCategories.h
//
// Provides the integer identifiers for all the memory categories that are supported.
// One important thing to remember is that if a category ID is added here, it needs
// to also have its related category information added to the memory DLL in the
// category manager, otherwise it cannot have memory associated with it.
//
//------------------------------------------------------------------------------------

#ifndef __LTMEMORYCATEGORIES_H__
#define __LTMEMORYCATEGORIES_H__

enum
{
	LT_MEM_TYPE_ALL,
		LT_MEM_TYPE_UNKNOWN,
		LT_MEM_TYPE_ENGINE,
			LT_MEM_TYPE_MISC,
			LT_MEM_TYPE_TEXTURE,
			LT_MEM_TYPE_VISIBILITY,
			LT_MEM_TYPE_PHYSICS,
				LT_MEM_TYPE_HAVOK,
			LT_MEM_TYPE_MODEL,
			LT_MEM_TYPE_SOUND,
			LT_MEM_TYPE_OBJECT,
			LT_MEM_TYPE_WORLD,
			LT_MEM_TYPE_MUSIC,
			LT_MEM_TYPE_FILE,
			LT_MEM_TYPE_UI,
			LT_MEM_TYPE_HASHTABLE,
			LT_MEM_TYPE_NETWORKING,
			LT_MEM_TYPE_RENDERER,
				LT_MEM_TYPE_RENDER_VIDEO,
				LT_MEM_TYPE_RENDER_MATERIAL,
				LT_MEM_TYPE_RENDER_SHADER,
				LT_MEM_TYPE_RENDER_WORLD,
				LT_MEM_TYPE_RENDER_MESH,
				LT_MEM_TYPE_RENDER_TEXTURE,
			LT_MEM_TYPE_CONSOLE,
			LT_MEM_TYPE_INTERFACEDB,
			LT_MEM_TYPE_INPUT,
			LT_MEM_TYPE_GRAPH,
		LT_MEM_TYPE_GAMECODE,
			LT_MEM_TYPE_GAMEDATABASE,
			LT_MEM_TYPE_CLIENTSHELL,
			LT_MEM_TYPE_OBJECTSHELL,
			LT_MEM_TYPE_CLIENTFX,
};

#endif

