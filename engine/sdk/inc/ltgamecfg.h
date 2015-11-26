//-----------------------------------------------------------------------------------------
// ltgamecfg.h
//
// Some configuration data for the game must be data driven rather than hardcoded.  Also
// this data must not exist in existing data stores because it must be shielded from 
// other systems like CRC checking, localization, or console system.  This class
// provides access to the data layer for any system that needs it.  Direct
// access to the data outside of this class is discouraged.  
// 
// The data was originally hardcoded, but needed to be moved to a data file to support the
// MPFree effort.
//
//-----------------------------------------------------------------------------------------
#ifndef __LTGAMECFG_H__
#define __LTGAMECFG_H__

#include "ltprofileutils.h"
#include "ltinteger.h"

// Data filename.
#define GAMECFG_FILE	".\\gamecfg.txt"

// Profile section name.
#define GAMECFG_SECTION	"General"

class LTGameCfg
{
public:
	
	// Check if this is the MPFree product.
	static bool IsMPFreeProduct( )
	{
		return ( LTProfileUtils::ReadUint32( GAMECFG_SECTION, "PRODUCT_MPFREE", 0, GAMECFG_FILE ) == 1 );
	}

};

#endif // __LTGAMECFG_H__
