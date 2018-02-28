/*****************************************************************
* FaceWarper ,截取摆正后的人脸图像，已进行优化加速
* Copyright (c) 2016 MEITU. All rights reserved.
*
* @version: 1.1
*
* @author:  dqplum
*
* @email:  lqd@meitu.com
*
* @date: 2016-06-06
*
* @note:
*
* @usage:
*
* @change: dqplum 2016-07-31 更改warp计算方式，缩放和摆正人脸同时做，最后才进行裁剪，提高计算效率
* @change: dqplum 2016-10-12 添加矩形框延拓的参数调用函数，开放延拓参数
* @change: dqplum 2017-05-07 允许输入的裁剪人脸尺寸为-1，-1表示内部保留人脸裁剪图在原图中的大小
******************************************************************/
#pragma once
//#include <vector>
//#include <mtcvlite/Mat/mat.h>
#include <mtface/Common/MTDefine.h>

namespace mtcvlite
{	
class Mat;

class MEITU_EXPORT FaceWarper
{
public:
	enum {
		WARP_CONVEX, //通过计算凸包的方法实现warp
		WARP_LINE    //通过鼻子、嘴唇、下巴尖点实现warp
	};
public:
	FaceWarper();
	~FaceWarper();

	//! 通过人脸点33、45、106，裁切人脸图像并进行warp，使其摆正，每次只能输入一张人脸点
	bool RunWithWarp(
		unsigned char* pImage,      //输入，图像指针，只能4通道，BGRA(或者ios: ARGB)
		int nWidth, int nHeight,    //输入，图像长宽
		unsigned char* pFaceImage,  //输出，裁切的人脸，需要在函数外声明好内存，大小为nFaceImageSize * nFaceImageSize * nFaceImageChannel
		const int nFaceImageSize,   //输入，裁切的人脸图像长宽，长宽大小一致
		const int nFaceImageChannel,//输入，裁切的人脸图像通道数，只能是1或4
		const float* pFacePoints,   //输入，人脸对齐点指针
		const int nFacePoint,       //输入，人脸点数
		float* pFaceRect = 0);

	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能4通道，BGRA(或者ios: ARGB)
		int nWidth, int nHeight,    //输入，图像长宽
		Mat* pMatFaceImage,          //输出，裁切的人脸
		int& nFaceImageSize,        //输入和输出，裁切的人脸图像长宽，长宽大小一致，必须赋初值
		const int nFaceImageChannel,//输入，裁切的人脸图像通道数，只能是1或4
		const float* pFacePoints,   //输入，人脸对齐点指针
		const int nFacePoint,       //输入，人脸点数
		float* pFaceRect = 0,
		const int nBorderMode = 1
		);

	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能4通道，BGRA(或者ios: ARGB)
		int nWidth, int nHeight,    //输入，图像长宽
		unsigned char* pFaceImage,  //输出，裁切的人脸,需要在函数外声明好内存，大小为nFaceImageSize * nFaceImageSize * nFaceImageChannel
		const int nFaceImageSize,   //输入和输出，裁切的人脸图像长宽，长宽大小一致，必须赋初值
		const int nFaceImageChannel,//输入，裁切的人脸图像通道数，只能是1或4
		const float* pFacePoints,   //输入，人脸对齐点指针
		const int nFacePoint,       //输入，人脸点数
		float* pFaceRect = 0,
		const int nBorderMode = 1
		);

	/*
	* @param 
	* @param 
	* @param [afFaceRect] input: size >= 8，人脸框计算方式请调用函数ComputeFaceRectByFAPoints计算
	*                     人脸裁剪矩形外框左下(0, 1), 右下(2, 3), 右上(4, 5), 左上(6, 7), 逆时针存储
	* @param [fRotateRad] input: 人脸矩形框的角度
	* @param [pfRotateMat] output: size >= 8 计算旋转变换矩阵，0-5代表2x3变换矩阵，6-7代表crop起始位置
	*                              在内部计算时，为提高计算效率，会对人脸区域进行裁框再进行warp，其中
	*                              pfRotateMat[6]和pfRotateMat[7]分别代表裁框的起始x和y
	* @return true  on success
	*		  false on fail
	*/
	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能4通道，BGRA(或者ios: ARGB)
		int nWidth, int nHeight,    //输入，图像长宽
		Mat* pMatFaceImage,          //输出，裁切的人脸
		int& nFaceImageSize,        //输入，裁切的人脸图像长宽，长宽大小一致, -1表示维持原始大小
		const int nFaceImageChannel,//输入，裁切的人脸图像通道数，只能是1或4
		const float* afFaceRect,    //输入，人脸框，外框
		const float fRotateRad,     //输入，人脸框的角度，单位为弧度
		float* pfRotateMat = 0,     //输出，旋转变换矩阵
		int* pnFaceValidRect = 0,   //输出，裁切的人脸的有效矩形框区域
		const int nBorderMode = 1
		);

	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能4通道，BGRA(或者ios: ARGB)
		int nWidth, int nHeight,    //输入，图像长宽
		unsigned char* pFaceImage,  //输出，裁切的人脸, 需要在函数外声明好内存，大小为nFaceImageSize * nFaceImageSize * nFaceImageChannel
		const int nFaceImageSize,   //输入，裁切的人脸图像长宽，长宽大小一致
		const int nFaceImageChannel,//输入，裁切的人脸图像通道数，只能是1或4
		const float* afFaceRect,    //输入，人脸框，外框
		const float fRotateRad,     //输入，人脸框的角度，单位为弧度
		float* pfRotateMat = 0,     //输出，旋转变换矩阵
		int* pnFaceValidRect = 0,   //输出，裁切的人脸的有效矩形框区域
		const int nBorderMode = 1
		);

	/*
	* @param [pfFAPoints] input: size = nFAPoints * 2, 按x,y,x,y...依次存储
	* @param [pfWarpFAPpints] output: size = nFAPoints * 2, 按x,y,x,y...依次存储
	* @param [pfRotateMat] input: size >= 8 计算旋转变换矩阵，0-5代表2x3变换矩阵，6-7代表crop起始位置
	*                              在内部计算时，为提高计算效率，会对人脸区域进行裁框再进行warp，其中
	*                              pfRotateMat[6]和pfRotateMat[7]分别代表裁框的起始x和y
	* @return void
	*/
	void ComputeWarpFAPoints(
		const float* pfFAPoints,  //输入，原始图像上的人脸点，按x,y,x,y...依次存储
		float* pfWarpFAPpints,    //输出，经旋转变换矩阵以及图像裁剪得到的人脸点
		const int nFAPoints,      //输入，人脸点数
		const float* pfRotateMat
		);

	/*
	* @param [pWarpImage] input: 4通道，则alpha通道存mask
	*                            2通道，则pImage必须存成2通道数据，比如RGRG...，R通道存灰度，G通道存mask
	*                            1或3通道，灰度图或BGR图
	* @param [] output: 
	* @param [pTranMat] input: size >= 8 计算旋转变换矩阵，0-5代表2x3变换矩阵，6-7代表crop起始位置
	*                              在内部计算时，为提高计算效率，会对人脸区域进行裁框再进行warp，其中
	*                              pfRotateMat[6]和pfRotateMat[7]分别代表裁框的起始x和y
	* @param [pImage] output: 4通道，则alpha通道存mask
	*                         2通道，则pImage输出2通道数据，比如RGRG...，R通道存灰度，G通道存mask
	*                         1或3通道，灰度图或BGR图
	* @return void
	*/
	void InverseWarpImage(
		unsigned char* pWarpImage,      //输入, warp后的图像(人脸图)
		const int nWarpWidth,           //输入,
		const int nWarpHeight,          //输入,
		const int nChannel,             //输入,通道数, 只能为1,2,3或4
		const float fWarpScaleWidth,    //输入,warp图像宽度方向放大的倍数
		const float fWarpScaleHeight,   //输入,warp图像高度方向放大的倍数
		const float* pTranMat,          //输入,warp变换矩阵
		unsigned char* pImage,          //输出，图像指针，大小为nWidth * nHeight * nChannel
		int nWidth, int nHeight         //输入，原始图像长宽
		);

	//! 根据人脸对齐点计算矩形框
	/*
	* @param [pfFacePoints] input: face point, x,y,x,y,x,y......
	* @param [nFacePoint] input: face point num, must be equal to 33, 45 or 106
	* @param [afInnerOuterRect] output: size >= 16
	*                     裁剪矩形内框左下(0, 1), 右下(2, 3), 右上(4, 5), 左上(6, 7), 逆时针存储
	*                     裁剪矩形外框左下(8, 9), 右下(10, 11), 右上(12, 13), 左上(14, 15), 逆时针存储
	* @param [fRotateRad] output: 人脸矩形框的角度，单位为弧度
	* @param [afExtentParam] input: 人脸框内框向外延拓的参数, 延拓参数从头部，逆时针存储
	*                     afExtentParam[ExtentHead]   沿额头方向延拓的参数
	*                     afExtentParam[ExtentLeft]   沿脸庞左边延拓的参数
	*                     afExtentParam[ExtentJaw]    沿下巴方向延拓的参数
	*                     afExtentParam[ExtentRight]  沿脸庞右边延拓的参数
	* @param [fExtentRect] input: 在上述延拓的基础上，4个方向统一延拓的参数
	*                      即长宽放大的倍数为 1+fExtentRect
	* @return true  on success
	*		  false on fail
	*/
	enum ExtentDirection{ExtentHead = 0, ExtentLeft = 1, ExtentJaw = 2, ExtentRight = 3};
	bool ComputeFaceRectByFAPoints(const float* pfFacePoints, int nFacePoint,
		float* afInnerOuterRect, float& fRotateRad,
		const float* afExtentParam = 0, const float fExtentRect = 0.0f);

	bool ComputeFaceRectByFAPoints(const float* pfFacePoints, int nFacePoint,
		const float fForeheadDirX, const float fForeheadDirY,
		float* afInnerOuterRect, float& fRotateRad,
		const float* afExtentParam = 0, const float fExtentRect = 0.0f);
protected:
	bool ComputeRotateParam(const float* pFacePoints, int nFacePoint, 
		float* aRotatedRectPts8, double& RotateRad, float* aInnerRectPts8 = 0);
	
};

} // namespace mtcvlite
