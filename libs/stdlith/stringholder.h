//------------------------------------------------------------------
//
//  FILE      : StringHolder.h
//
//  PURPOSE   : Defines the CStringHolder class.  StringHolders
//              help you manage memory when dealing with strings
//              in particular.
//
//  CREATED   : 5/1/96
//
//  COPYRIGHT : Monolith 1996 All Rights Reserved
//
//------------------------------------------------------------------

#ifndef __STRINGHOLDER_H__
#define __STRINGHOLDER_H__

#ifndef __DYNARRAY_H__
#include "dynarray.h"
#endif

class SBank {
public:
    char    *m_pString;
    uint16  m_AllocSize;
    uint16  m_StringSize; // Current length of the string.
};


class CStringHolder {
public:
    
    // Constructors
    CStringHolder();
    CStringHolder(uint32 allocSize);
    
    // Destructor
    ~CStringHolder();

    // Member functions
    
    // Changes the allocation size.  You can only do this when the stringholder is empty.
    void SetAllocSize(uint32 size);
    
    // Adds the string in, and grows the internal array if necessary.
    // If bFindFirst=TRUE, it'll search the current array and 
    // if the string's already there, return a pointer to it.
    char *AddString(const char *pString, bool bFindFirst=true);

    // Same as above, but here it'll use len and ignore the
    // contents of the string.
    char *AddString(const char *pString, bool bFindFirst, unsigned int len);
    
    // Returns a pointer if it can find it.
    // Returns Null if it's not in there.
    char *FindString(const char *pString);
    
    // Clears out all strings.
    void ClearStrings(); 

private:
    // Member variables
    
    // Pointers to strings.
    CMoArray<SBank> m_Strings;
    
    // When it has to allocate a string,
    // this is how big the string is (at a minimum).
    uint32 m_AllocSize;
};


#endif

