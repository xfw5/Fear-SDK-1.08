#include "iltmemory.h"

//The intent of this file is to allow projects that link with libraries that use the
//memory interface but do not use the memory system to link properly. This just stubs out the
//calls made to the ILTMemory interface and provides symbols to resolve linking.

#ifndef DISABLE_MEMORY_TRACKING

	void LTMemPushMemoryContext(const char* /*sFileName*/, uint32 /*nLineNum*/, uint32 /*nCategory*/)
	{
	}

	void LTMemPopMemoryContext()
	{
	}

	void LTMemExternalAlloc(uint32 /*nMemCategory*/, uint32 /*nSize*/)
	{
	}

	void LTMemExternalFree(uint32 /*nMemCategory*/, uint32 /*nSize*/)
	{
	}

	ILTMemory*	LTMemGetILTMemory()
	{
		return NULL;
	}

#endif



