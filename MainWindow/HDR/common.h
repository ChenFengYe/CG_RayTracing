#ifndef _LW_COMMON_H_
#define _LW_COMMON_H_
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <typeinfo>
#include <type_traits>

// ----------------------------------------------------------------------------
// Parameter constants
// ----------------------------------------------------------------------------
static const double PI = 4.0 * atan(1.0);
static const double INV_PI = 1.0 / PI;
static const double INFTY = 1.0e32;
static const double EPS = 1.0e-6;

#if (defined(WIN32) || defined(_WIN32) || defined(WINCE) || defined(__CYGWIN__))
#   define PACKED(__declare__) __pragma(pack(push,1)) __declare__ __pragma(pack(pop)) 
#else
#   define PACKED(__declare__) __declare__ __attribute__((__packed__))
#endif


// -----------------------------------------------------------------------------
// Assertion with message
// -----------------------------------------------------------------------------
#ifndef __FUNCTION_NAME__
#if defined(_WIN32) || defined(__WIN32__)
#define __FUNCTION_NAME__ __FUNCTION__
#else
#define __FUNCTION_NAME__ __func__
#endif
#endif

#define Assertion(PREDICATE, MSG) \
do {\
if (!(PREDICATE)) {	\
	std::cerr << "Asssertion \"" \
	<< #PREDICATE << "\" failed in " << __FILE__ \
	<< " line " << __LINE__ \
	<< " in function \"" << (__FUNCTION_NAME__) << "\"" \
	<< " : " << (MSG) << std::endl; \
	std::abort(); \
} \
} while (false)

#endif