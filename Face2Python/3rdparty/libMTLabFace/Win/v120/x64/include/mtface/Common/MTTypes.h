#pragma once


#include "mtface/Common/MTDefine.h"
#include <vector>
#include <stdio.h>

namespace mtface
{

// ERROR < 0
enum MTRET
{
    MTRET_SUCCESS          = 0,    // 成功
    MTRET_ERR_FAIL         = -1,   // 失败
    MTRET_ERR_NOINIT       = -2,   // 尚未初始化
    MTRET_ERR_PARAMS       = -3,   // 参数错误
    MTRET_ERR_UNSUPPORT    = -4,   // 不支持的操作或参数
    MTRET_ERR_FILENONE     = -5,   // 文件未找到
    MTRET_ERR_FILEERR      = -6,   // 文件错误
    MTRET_ERR_PROCESS      = -7,   // 处理中无法进行其他操作
    MTRET_ERR_UNKNOWN      = -8,   // 未知错误
};

template <typename Dtype>
struct MEITU_EXPORT MTRect_
{
    MTRect_()
    {

    }

    MTRect_(Dtype xx, Dtype yy, Dtype ww, Dtype hh) : x(xx), y(yy), width(ww), height(hh)
    {

    }

    Dtype area()
    {
        return width * height;
    }

    Dtype x;
    Dtype y;
    Dtype width;
    Dtype height;
};

typedef MTRect_<float> MTRect2f;
typedef	MTRect_<int> MTRect2i;

template <typename Dtype>
struct MEITU_EXPORT MTSize_
{
    MTSize_() { }
    
    MTSize_(Dtype w, Dtype h) : width(w), height(h) { }
    
    Dtype width;
    Dtype height;
};

typedef MTSize_<float> MTSize2f;
typedef MTSize_<int> MTSize2i;

template <typename Dtype>
struct MEITU_EXPORT MTPoint_
{
    MTPoint_() {}
    MTPoint_(Dtype xx, Dtype yy) : x(xx), y(yy) {}
    
    Dtype x;
    Dtype y;
};

typedef MTPoint_<float> MTPoint2f;
typedef MTPoint_<int>	MTPoint2i;

template <typename Dtype>
struct MEITU_EXPORT MTPoint3_
{
	MTPoint3_() {}
	MTPoint3_(Dtype xx, Dtype yy) : x(xx), y(yy) {}

	Dtype x;
	Dtype y;
	Dtype z;
};

typedef MTPoint3_<float> MTPoint3f;
typedef MTPoint3_<int>	MTPoint3i;

template <typename Dtype>
class MTData
{
public:
    MTData();
    MTData(int w, int h);
    
    // 分配空间, 拷贝外部传入的数据
    MTData(int w, int h, Dtype *p);
    ~MTData();
    
    // 分配空间并全部初始化为0
    void initWithZero(int w, int h);
    
    // 不重分配空间, 而是使用外部传入的数据
    void initWithData(int w, int h, Dtype *p);
    
    int width;
    int height;
    Dtype *pData;
private:
    Dtype *pDataReal;
};

} // namespace mtface
