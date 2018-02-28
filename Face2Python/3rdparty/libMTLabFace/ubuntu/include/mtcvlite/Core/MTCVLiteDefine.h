#pragma once

//#include <mtface/Common/MTDefine.h>

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

