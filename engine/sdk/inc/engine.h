/*!
Base engine header file
*/

#ifndef __ENGINE_H__
#define __ENGINE_H__

#pragma warning (disable:4786) // No truncation warnings, please..

/*!
Engine define
*/
#ifndef _ENGINE_
    #define _ENGINE_
#endif


/*!
Platform configuration
*/

#ifndef __PLATFORM_H__
#include "platform.h"
#endif

/*!
Main includes
*/

#ifndef __LTBASETYPES_H__
#include "ltbasetypes.h"
#endif

#ifndef __LTBASEDEFS_H__
#include "ltbasedefs.h"
#endif

/*!
Interfaces
*/
#ifndef __ILTINSTREAM_H__
#include "iltinstream.h"
#endif

#ifndef __ILTOUTSTREAM_H__
#include "iltoutstream.h"
#endif


#endif //!__ENGINE_H__
