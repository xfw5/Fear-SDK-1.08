// ----------------------------------------------------------------------- //
//
// MODULE  : ltbuildversion.h
//
// PURPOSE : This file contains the current build version of the game
//           project
//
// CREATED : 11/02/05
//
// (c) 2005 Monolith Productions, Inc.  All Rights Reserved
//
// ----------------------------------------------------------------------- //

#ifndef __LTBUILDVERSION_H__
#define __LTBUILDVERSION_H__

/*
 *	Internal Build Numbers
 */ 
// the product id, this should increment with every product
#define LT_VERSION_PRODUCT			0
// the product sku, sp demo, mp demo, etc
#define LT_VERSION_SKU				0
// the build number
#define LT_VERSION_BUILD			282
// the bug fix between builds
#define LT_VERSION_BUGFIX			0

/*
 *	External Build Numbers
 */

// the products major version
#define LT_PRODUCT_MAJOR			1
// the products minor version
#define LT_PRODUCT_MINOR			08
// the build number
#define LT_PRODUCT_BUILD			LT_VERSION_BUILD
// the hot fix number
#define LT_PRODUCT_HOTFIX			LT_VERSION_BUGFIX


// the description of the executable
#ifndef LT_VERSION_DESCRIPTION
#	define LT_VERSION_DESCRIPTION	""
#endif

#define __LT_VERSION_PRODUCT_STRING_INTERNAL2(p, s, b, f) \
	#p "." #s "." #b "." #f

#define __LT_VERSION_PRODUCT_STRING_INTERNAL(p, s, b, f) \
	__LT_VERSION_PRODUCT_STRING_INTERNAL2(p, s, b, f)

// the internal version string
#define LT_VERSION_INTERNAL_STRING	__LT_VERSION_PRODUCT_STRING_INTERNAL(LT_VERSION_PRODUCT, LT_VERSION_SKU, LT_VERSION_BUILD, LT_VERSION_BUGFIX)
// the product version string
#define LT_VERSION_PRODUCT_STRING	__LT_VERSION_PRODUCT_STRING_INTERNAL(LT_PRODUCT_MAJOR, LT_PRODUCT_MINOR, LT_PRODUCT_BUILD, LT_PRODUCT_HOTFIX)

#define __LT_VERSION_BUILD_STRING_INTERNAL2(b) \
	"Build " #b

#define __LT_VERSION_BUILD_STRING_INTERNAL(b) \
	__LT_VERSION_BUILD_STRING_INTERNAL2(b)

// the build string
#define LT_VERSION_BUILD_STRING		__LT_VERSION_BUILD_STRING_INTERNAL(LT_VERSION_BUILD)

// the company name
#define LT_VERSION_COMPANYNAME		"Monolith Productions, Inc."

// the copyright notice
#ifndef LT_VERSION_COPYRIGHT
#	define LT_VERSION_COPYRIGHT		"Copyright © 2005 Monolith Productions, Inc."
#endif

#define LT_VERSION_RESOURCE_BUILD_STRING	"Monolith Build String"
#define LT_VERSION_RESOURCE_BUILD_VERSION	"Monolith Build Version"

// a list of email alias that get emails when a crash occurs
#define LT_DUMP_EMAIL_TOOLS			"Tools"
#define LT_DUMP_EMAIL_GAME			"Game"

#endif  // __LTBUILDVERSION_H__

