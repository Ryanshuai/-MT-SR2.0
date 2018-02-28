#ifndef MTLAB_COMMON_MEITUDEFINE_H_
#define MTLAB_COMMON_MEITUDEFINE_H_

#include <algorithm>
#include <stdio.h>
#include <string.h>

//plaform
#if defined(_WIN32) || defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64) || defined(_WIN64)
	#define PLATFORM_WINDOWS 1
#elif defined(ANDROID) || defined(_ANDROID_)
	#define PLATFORM_ANDROID 1
    #include <jni.h>
#elif defined(__APPLE__)
	// macro define of TARGET_OS_IPHONE, TARGET_OS_SIMULATOR, TARGET_CPU_ARM, TARGET_CPU_ARM64 etc
    #include <TargetConditionals.h>
	#define PLATFORM_IOS	 1
#elif defined(__linux__) || defined(__unix__) || defined(linux) || defined(__linux)
	#define PLATFORM_LINUX	 1
#else
	#define PLATFORM_UNKNOWN 1
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1600) // before Visual Studio 2010 have not <stdint.h>
	#define STDINT_MISSING   1
#elif defined(__GNUC__)
	#if !defined(GCC_VERSION)
	#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#endif
    #if (GCC_VERSION < 40500) // test if GCC > 4.5.0 https://gcc.gnu.org/c99status.html
	#define STDINT_MISSING   1
	#endif
#else
	#define STDINT_MISSING   0
#endif

#if !defined(STDINT_MISSING) || (STDINT_MISSING == 0)
#include <stdint.h>
#else
#endif

// EXPORT
#if defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64)
	#ifdef MEITUFDFA_LIB
	#define MEITU_EXPORT __declspec(dllexport)
	#define MEITU_TEMPLATE
	#else
	#ifndef MEITUFDFA_SOURCE
	#define MEITU_EXPORT __declspec(dllimport)
	#define MEITU_TEMPLATE extern
	#else
	#define MEITU_EXPORT
	#define MEITU_TEMPLATE 
	#endif
	#endif
#elif defined(_ADNROID_) || defined(ANDROID) || defined(__APPLE__) || defined(__linux__)
	#ifdef MEITUFDFA_LIB
	#define MEITU_EXPORT __attribute__((visibility("default")))
	#define MEITU_TEMPLATE
	#else
	#define MEITU_EXPORT
	#define MEITU_TEMPLATE extern
	#endif
#else
	#define MEITU_EXPORT
	#define MEITU_TEMPLATE 
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) { if (x) delete (x); (x) = NULL; }	
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) delete [] (x); (x) = NULL; }	
#endif

typedef unsigned char BYTE;

#ifndef SC
#define SC(t,x) static_cast<t>(x)
#endif

#ifndef SC_INT
#define SC_INT(x) static_cast<int>(x)
#endif
#ifndef SC_FLOAT
#define SC_FLOAT(x) static_cast<float>(x)
#endif
#ifndef SC_BYTE
#define SC_BYTE(x) static_cast<BYTE>(x)
#endif
#ifndef SC_SHORT
#define SC_SHORT(x) static_cast<short>(x)
#endif

#ifndef CLAMP2BYTE
#define CLAMP2BYTE(x) (unsigned char)((((unsigned short)x | ((short)(255 - x) >> 15)) & ~x >> 15));
#endif

namespace mtface
{

const int MT_RED   = 0;
const int MT_GREEN = 1;
const int MT_BLUE  = 2;
const int MT_ALPHA = 3;

} // namespace mtface

#endif // MTLAB_COMMON_MEITUDEFINE_H_
