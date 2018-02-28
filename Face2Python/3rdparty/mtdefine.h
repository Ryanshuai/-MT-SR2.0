/*
 * mtdefine.h
 *
 * brief: 各平台的公共宏定义
 */
#ifndef __MTDEFINE_H__
#define __MTDEFINE_H__

// plaform detection
#if defined(_WIN32) || defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #if _MSC_VER < 1600 // before Visual Studio 2010 have not <stdint.h>
    #define STDINT_MISSING   1
    #endif
#elif defined(ANDROID) || defined(_ANDROID_)
    #define PLATFORM_ANDROID 1
    #if !defined(__clang__) && (GCC_VERSION < 40500) // test if GCC > 4.5.0 https://gcc.gnu.org/c99status.html
    #define STDINT_MISSING   1
    #endif
#elif defined(_APPLE_)
    #define PLATFORM_IOS	 1
    // macro define of TARGET_OS_IPHONE, TARGET_OS_SIMULATOR, TARGET_CPU_ARM, TARGET_CPU_ARM64 etc
    #include <TargetConditionals.h>
#elif defined(__linux__) || defined(__unix__) || defined(linux) || defined(__linux)
    #define PLATFORM_LINUX   1
#else
    #define PLATFORM_UNKNOWN 1
    #define STDINT_MISSING   1
#endif

// iOS Neon support, it's also can defined in xcode "Other C Flags" by separate arch
#define ENABLE_NEON_SUPPORT 1
#if (TARGET_OS_IPHONE != 0) && defined(ENABLE_NEON_SUPPORT)
    #if !defined(HAVE_NEON)
        #define HAVE_NEON
    #endif
    #if TARGET_CPU_ARM != 0
    #if !defined(HAVE_NEON32)
        #define HAVE_NEON32
    #endif
    #elif TARGET_CPU_ARM64 != 0
    #if !defined(HAVE_NEON64)
        #define HAVE_NEON64
    #endif
    #endif
#endif

// common includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <float.h>      // for FLT_EPSILON etc

using std::max;
using std::min;

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <unistd.h>
#include <pthread.h>
#elif defined(PLATFORM_WINDOWS)
#else
#endif

#if defined(STDINT_MISSING)
#else
#include <stdint.h>
#endif

// BGRA sequence
#if defined(PLATFORM_IOS)
#define MT_ALPHA    0
#define MT_RED      1
#define MT_GREEN    2
#define MT_BLUE     3
#else
#define MT_ALPHA    3
#define MT_RED      2
#define MT_GREEN    1
#define MT_BLUE     0
#endif

// OpenGL color space
#define GL_CHANNLE_R 0
#define GL_CHANNLE_G 1
#define GL_CHANNLE_B 2
#define GL_CHANNLE_A 3

// type define
typedef unsigned char       byte;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        UINT;
typedef unsigned int        uint32;
typedef long long           int64;
typedef unsigned long long  uint64;
typedef long                MTCOLORREF;
typedef float               REAL;

#undef TRUE
#define	TRUE                true
#undef FALSE
#define	FALSE               false

// safe delete
#undef SALFDELETE
#define SALFDELETE(x) if((x)!=NULL){ delete (x); (x)=NULL; }

#undef SAFE_DELETE
#define SAFE_DELETE(x) if((x)!=NULL){ delete (x); (x)=NULL; }

#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if((x)!=NULL){ delete[] (x); (x)=NULL; }


////////////////////////////////////////////////////////////////////////////////////////////////////

#define MT_FRAME_MIN 1
#define MT_IMAGESIZE_MIN_LENGTH		4

//旋转的类型
#define	MT_ROTATION_CLOCKLEFT		0		//逆时针翻转90度
#define	MT_ROTATION_CLOCKRIGHT		1		//顺时针翻转90度
#define	MT_ROTATION_LEFTANDRIGHT	2		//左右翻转
#define	MT_ROTATION_UPANDDOWN		3		//上下翻转
#define	MT_ROTATION_OTHER			4		//-45到45的旋转

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

#define FMax(x, y) ((x) < (y) ? (y) : (x))
#define FMin(x, y) ((x) < (y) ? (x) : (y))

#define FLOAT_TO_BYTE(x) (BYTE)(MAX(0, MIN(255, (x))))
#define INT_TO_BYTE(x) (MAX(0, MIN(255, (x))))

#define ZEROPLUS(a) ((a) == 0 ? 1 : (a))
//#define FLT__EPSILON     1.192092896e-07f
#define FLT__EPSILON FLT_EPSILON // is in float.h


#define DIV_255     0.00392156
#define DIV_510     0.00196470
#define DIV_255_6   0.023529    //6 / 255
#define DIV_12_255  21.25       //6 / 255
#define DIV_6_255   42.5        //6 / 255
#define DIV_3_255   85          //6 / 255
#define DIV_3_510   170         //6 / 255
#define DIV_2_255   127.5       //6 / 255

#define HLSMAX   255   /* H,L, and S vary over 0-HLSMAX */
#define RGBMAX   255   /* R,G, and B vary over 0-RGBMAX */
#define UNDEFINED (HLSMAX*2/3)
#define HSLMAX   255	/* H,L, and S vary over 0-HSLMAX */
#define HSLUNDEFINED (HSLMAX*2/3)

#ifndef CLAMP2BYTE
#define CLAMP2BYTE(x) (unsigned char)((((unsigned short)(x) | ((short)(255 - (x)) >> 15)) & ~(x) >> 15));
#endif

#define FSquare(x) ((x)*(x))
#define FClamp(t, tLow, tHigh) (FMax(tLow, FMin(tHigh, t)))
#define FClamp0255(n) FClamp(n, 0, 0xFF)

#define LIB_PI 3.1415926535897932384626433832795
#define LIB_2PI (2.0*LIB_PI)

#define radians(degrees) ((degrees) * M_PI / 180.0f) // M_PI is defined in math.h
#define AngleToRadian(nAngle) (LIB_PI*(nAngle)/180.0)

#define FHypot(x,y) sqrt((double)(x)*(x)+(double)(y)*(y))
#define FRound(dX) ((dX>0.0) ? ((int)(dX+0.5)) : ((int)(dX-0.5)))
#define GRAYSCALE(r,g,b) ((30*(r)+59*(g)+11*(b))/100)
#define IsInside(w,h,x,y) ((x>=0) && (x<w) && (y>=0) && (y<h))
#define IsRGBEqual(a1,a2) ((a1.rgbRed == a2.rgbRed) && (a1.rgbGreen == a2.rgbGreen) && (a1.rgbBlue == a2.rgbBlue))

//#define _cimg_Labf(x) ((x)>=0.008856f?(pow(x,(double)1/3)):(7.787f*(x)+16.0f/116))
#define _cimg_Labf(x) ((x)>=0.008856f?(pow(x,0.333333f)):(7.787f*(x)+0.137931f))
//#define _cimg_Labfi(x) ((x)>=0.206893f?((x)*(x)*(x)):(((x)-16.0f/116)/7.787f))
#define _cimg_Labfi(x) ((x)>=0.206893f?((x)*(x)*(x)):(((x)-0.137931f)*0.128419f))

#undef LOBYTE
#define LOBYTE(rgb)         ((rgb)&0xff)
#undef ToBYTE
#define ToBYTE(a)           (0xff & a)

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

//判断是否与2^N对齐((a%(2^n))==0)
#define IS_ALIGNED(p, a) (!((unsigned int)(p) & ((a) - 1)))
//作用:http://linux.chinaunix.net/techdoc/net/2008/07/23/1020120.shtml
#define OMITFP __attribute__((optimize("omit-frame-pointer")))
//一个宏计算
#define BLENDER(a, b, f) ((int)(a) + ((f) * ((int)(b) - (int)(a)) >> 16))

#define YG 74  /* static_cast<int8>(1.164 * 64 + 0.5) */

#define UB 127 /* min(63,static_cast<int8>(2.018 * 64)) */
#define UG -25 /* static_cast<int8>(-0.391 * 64 - 0.5) */
#define UR 0

#define VB 0
#define VG -52 /* static_cast<int8>(-0.813 * 64 - 0.5) */
#define VR 102 /* static_cast<int8>(1.596 * 64 + 0.5) */

// Bias
#define BB (16256)  /*(UB << 7) + (VB << 7)*/
#define BG (-9856)  /*(UG << 7) + (VG << 7)*/
#define BR (13056)  /*(UR << 7) + (VR << 7)*/

//static_cast
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

#undef MT_PI
#define MT_PI 3.1415926


typedef enum _TONE_REGION
{
    /// shadow region of image.
    TONE_SHADOWS = 0,
    /// midtone region of image.
    TONE_MIDTONES = 1,
    /// highlight region of image.
    TONE_HIGHLIGHTS = 2,
}TONE_REGION;

typedef enum _IMAGE_CHANNEL
{
    CHANNEL_RED   = 1 << 0,// 0x01,
    CHANNEL_GREEN = 1 << 1,// 0x02,
    CHANNEL_BLUE  = 1 << 2,// 0x04,
    CHANNEL_ALPHA = 1 << 3,// 0x08,
    CHANNEL_RGB   = CHANNEL_RED|CHANNEL_GREEN|CHANNEL_BLUE,// 0x07,
    CHANNEL_RGBA  = CHANNEL_RGB|CHANNEL_ALPHA,// 0x0F,
    CHANNEL_GRAY  = 1 << 4,// 0x10
}IMAGE_CHANNEL;

typedef struct tagMT2DPOINT
{
    int x;
    int y;
	tagMT2DPOINT(){}
	tagMT2DPOINT(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
}MT2DPOINT;
//POINT POINTMake(int _x,int _y);

typedef struct MT2DPoint
{
    int x;
    int y;
    MT2DPoint(int nx,int ny):x(nx),y(ny)
    {
    }
    MT2DPoint()
    {
        x =0;
        y = 0;
    }
}MT2DPoint;

typedef struct
{
    int left;
    int top;
    int right;
    int bottom;
    
    int Width() const
    {
        return (right - left + 1);
    }
    
    int Height() const
    {
        return (bottom - top + 1);
    }
    
}MT2DRect;

typedef struct tagMT2DRECT
{
    long    left;
    long    top;
    long    right;
    long    bottom;
    long Width() const
    {
        return right - left + 1;
    }
    
    long Height() const
    {
        return bottom - top + 1;
    }
	tagMT2DRECT(long nleft, long ntop, long nright, long nbottom) :left(nleft), top(ntop), right(nright), bottom(nbottom){}
	tagMT2DRECT(){}
}MT2DRECT, MT2DCRect;

typedef struct tagMT2DRectFF
{
    int X;
    int Y;
    int Width;
    int Height;
    
	tagMT2DRectFF(int nX, int nY, int nW, int nH) :X(nX), Y(nY), Width(nW), Height(nH){}
	tagMT2DRectFF(){}
}MT2DRectFF;

//直线ax+by+c=0的参数a,b,c
typedef struct tagMT2DLine
{
    float a;
    float b;
    float c;
}MT2DLine;

typedef struct tagMT2DSize
{
    int Width;
    int Height;
    
	tagMT2DSize(int nW, int nH) :Width(nW), Height(nH){}
	tagMT2DSize(){}
}MT2DSize;

typedef struct tagMTRGBQUAD
{
    BYTE    rgbBlue;
    BYTE    rgbGreen;
    BYTE    rgbRed;
    BYTE    rgbReserved;
} MTRGBQUAD;


typedef struct _MT_IMAGE_DATA
{
    int width;
    int height;
    BYTE* data;
}MT_IMAGE_DATA;


// FDFA

//Float rect struct
typedef struct MtRectF
{
    float left, top, width, height;
    MtRectF(){}
    MtRectF(float l, float t, float w, float h) : left(l), top(t), width(w), height(h){}
}MtRectF;

//Float rect struct
typedef struct MtRectI
{
    int left, top, width, height;
    MtRectI(){}
    MtRectI(int l, int t, int w, int h) : left(l), top(t), width(w), height(h){}
}MtRectI;



//return
//错误列表
typedef enum _MT_ERROR_CODE
{
	MT_OK = 0,				//没有错误
	MT_CHANNEL_ERROR,		//通道参数错误，即不支持该类型的通道
	MT_PARAMETER_ERROR,		//传递的参数错误
}MT_ERROR_CODE;


#endif