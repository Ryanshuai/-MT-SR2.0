#ifndef MTLAB_COMMON_MEITULOG_H_
#define MTLAB_COMMON_MEITULOG_H_

#include <stdio.h>
#include <ctime>

#if defined(_MSC_VER)
#include <windows.h>

// undef unnecessary macros
#undef  min
#undef  max
#endif

#if defined(ANDROID) || defined(__ANDROID__)
#include <android/log.h>
#endif

namespace mtface
{

////////////////////////Android platform//////////////////////////////////////////////
#if defined(ANDROID) || defined(__ANDROID__)
#ifndef NDEBUG
//Jni log tag
#define LOG_TAG "FDFALog"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);
#else
#define LOGI(...)
#define LOGD(...)
#define LOGE(...)
#endif //NDEBUG

#elif defined(__linux__)
///////////////////////////////////Linux///////////////////////////////////////

#ifndef NDEBUG
#define LOGI(...) fprintf(stderr,"%08d [INFO]: ",(int)clock());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n");
#define LOGD(...) fprintf(stderr,"%08d [DEBUG]: ",(int)clock());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n");
#define LOGE(...) fprintf(stderr,"%08d [ERROR]: ",(int)clock());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n");
#else
#define LOGI(...)
#define LOGD(...)
#define LOGE(...)
#endif//NDEBUG

#elif defined(__APPLE__)

#ifndef NDEBUG
#define LOGI(format, ...) printf (format, ##__VA_ARGS__);printf ("\n");
#define LOGD(format, ...) printf (format, ##__VA_ARGS__);printf ("\n");
#define LOGE(format, ...) printf (format, ##__VA_ARGS__);printf ("\n");
#else
#define LOGI(...) ;
#define LOGD(...) ;
#define LOGE(...) ;
#endif//NDEBUG

#else
////////////////////////////////Windows//////////////////////////////////////
#ifndef NDEBUG

#define LOGI(...) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY); \
											fprintf(stderr,"%08d : ",clock());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n");

#define LOGD(...) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN | FOREGROUND_BLUE |  FOREGROUND_INTENSITY); \
											fprintf(stderr,"%08d : ",clock());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n");

#define LOGE(...) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED |  FOREGROUND_INTENSITY); \
										 fprintf(stderr,"%08d : ",clock());fprintf(stderr,__VA_ARGS__);fprintf(stderr,"\r\n%s(%d)\r\n",__FILE__,__LINE__);
#else
#define LOGI(...)
#define LOGD(...)
#define LOGE(...)
#endif//NDEBUG
#endif//defined(ANDROID) || defined(__ANDROID__)

// #define LOGI(format, ...) printf (format, ##__VA_ARGS__);printf ("\n");
// #define LOGD(format, ...) printf (format, ##__VA_ARGS__);printf ("\n");
// #define LOGE(format, ...) printf (format, ##__VA_ARGS__);printf ("\n");

#define LOG_START_FUNC() LOGD("++(%s)++",__FUNCTION__)

#define LOG_END_FUNC() LOGD("--(%s)--",__FUNCTION__)

} // namespace mtface

#endif // MTLAB_COMMON_MEITULOG_H_
