#pragma once

#include "mtface/Common/MTDefine.h"
#include "mtface/Common/MTTypes.h"

namespace mtface
{

////////////////////////////////////////////////////////////////////////////////////////
//								EXIF方向示例
//     1        2       3      4         5            6           7          8
//
//    888888  888888      88  88      8888888888  88                  88  8888888888
//    88          88      88  88      88  88      88  88          88  88      88  88
//    8888      8888    8888  8888    88          8888888888  8888888888          88
//    88          88      88  88
//    88          88  888888  888888
////////////////////////////////////////////////////////////////////////////////////////


struct MEITU_EXPORT MTImage
{
	enum PIXEL_FORMAT
	{
		GRAY,
		RGBA,
		NV12,
		NV21,
		I420
	};

	MTImage();
	MTImage(int _width, int _height, PIXEL_FORMAT _format, int _orientation = 1);
	MTImage(const MTImage& rhs);
	MTImage& operator=(const MTImage& rhs);
	~MTImage();

	
	union{
		unsigned char *y;       //y通道数据、RGB类型数据、GRAY
		unsigned char* data;
	};
	unsigned char *u;			//u通道数据
	unsigned char *v;			//v通道数据
	union{
		int ystride;            //y通道stride
		int stride;
	};
	int ustride;				//u通道的stride
    int vstride;				//v通道的stride

	int width;					//图片宽度
	int height;					//图片高度
	int orientation;			//exif方向
	PIXEL_FORMAT format;
	int* refcount;

	size_t total() const;
	void release();
	bool empty() const;
	void create(int _width, int _height, PIXEL_FORMAT _format, int _oriention = 1, int _ystride = 0, int _ustride = 0, int _vstride = 0);
	MTImage clone() const;
	void copyTo(MTImage& dst) const;
    
	static MTImage from_gray(int _width, int _height, uint8_t* _data, int _orientation = 1, int _stride = 0);
	static MTImage from_rgba(int _width, int _height, uint8_t* _data, int _orientation = 1, int _stride = 0);
	static MTImage from_nv12(int _width, int _height, uint8_t* _py, uint8_t* _puv, int _orientation = 1, int _ystride = 0, int _uvstride = 0);
	static MTImage from_nv21(int _width, int _height, uint8_t* _py, uint8_t* _pvu, int _orientation = 1, int _ystride = 0, int _vustride = 0);
	static MTImage from_i420(int _width, int _height, uint8_t* _py, uint8_t* _pu, uint8_t* _pv, int _orientation = 1, int _ystride = 0, int _ustride = 0, int _vstride = 0);
};
// copy src image to prevent call functions with the same images
MEITU_EXPORT int resize(MTImage src, MTImage& dst, MTSize_<int> size);
MEITU_EXPORT int convert(MTImage src, MTImage& dst, MTImage::PIXEL_FORMAT format);
MEITU_EXPORT int rotate(MTImage src, MTImage& dst, int orientation = 1);
MEITU_EXPORT int convertOnToO1(MTImage src, MTImage& dst, MTImage::PIXEL_FORMAT format);

} // namespace mtface


