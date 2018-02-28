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
//#include <mtcvlite/Mat/types.h>
#include <mtcvlite/Core/MTCVLiteDefine.h>

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

	enum WarpAndCropType{ WarpAdaptive = 0, CropAdaptive, CropCenter };
public:
	FaceWarper(const int nInterplation = 1/*MT_INTER_LINEAR*/,
		const int nBorderMode = 1/*MT_BORDER_REPLICATE*/,
		const int* paBorderConst = 0);
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

	/*
	* @param 
	* @param 
	* @param [afFaceRect] input: size >= 8，人脸框计算方式请调用函数ComputeFaceRectByFAPoints计算
	*                     人脸裁剪矩形外框左下(0, 1), 右下(2, 3), 右上(4, 5), 左上(6, 7), 逆时针存储
	* @param [fRotateRad] input: 人脸矩形框的角度
	* @param [pdRotateMat] output: size >= 6 计算2x3旋转变换矩阵，0-5代表2x3变换矩阵
	*                              
	* @return true  on success
	*		  false on fail
	*/
	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能4通道，BGRA(或者ios: ARGB)
		int nWidth, int nHeight,    //输入，图像长宽
		Mat& matFaceImage,          //输出，裁切的人脸
		int& nFaceImageSize,        //输入，裁切的人脸图像长宽，长宽大小一致, -1表示维持原始大小
		const int nFaceImageChannel,//输入，裁切的人脸图像通道数，只能是1或4
		const float* afFaceRect,    //输入，人脸框，外框
		const float fRotateRad,     //输入，人脸框的角度，单位为弧度
		float* pfRotateMat = 0,     //输出，旋转变换矩阵
		int* pnFaceValidRect = 0    //输出，裁切的人脸的有效矩形框区域
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
		int* pnFaceValidRect = 0    //输出，裁切的人脸的有效矩形框区域
		);


	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能1-4通道，若为4通道，alpha通道在内部会被改变
		int nWidth, int nHeight,    //输入，图像长宽
		int nChannel,               //输入，通道数，只能是1-4范围内的通道数
		const double* pdRotateMat,  //输入，裁剪人脸的2x3变换矩阵，size>=6
		Mat& matFaceImage,          //输出，裁切的人脸
		const int nFaceWidth,       //输入，裁切的人脸宽
		const int nFaceHeight,      //输入，裁切的人脸高
		const int nFaceChannel = -1 //输入，当pImage为4通道时，该参数才有意义
		);

	bool RunWithWarp_fast(
		unsigned char* pImage,      //输入，图像指针，只能1-4通道，若为4通道，alpha通道在内部会被改变
		int nWidth, int nHeight,    //输入，图像长宽
		int nChannel,               //输入，通道数，只能是1-4范围内的通道数
		const double* pdRotateMat,  //输入，裁剪人脸的2x3变换矩阵，size>=6
		unsigned char* pFaceImage,  //输出，裁切的人脸，需要在函数外声明好内存，大小为nFaceWidth * nFaceHeight * nFaceChannel
		const int nFaceWidth,       //输入，裁切的人脸宽
		const int nFaceHeight,      //输入，裁切的人脸高
		const int nFaceChannel = -1 //输入，当pImage为4通道时，该参数才有意义
		);

	bool Crop(
		unsigned char* pImage,      //输入，图像指针，只能1-4通道，若为4通道，alpha通道在内部会被改变
		int nWidth, int nHeight,    //输入，图像长宽
		int nChannel,               //输入，通道数，只能是1-4范围内的通道数
		const int* pCropRect,       //输入，裁剪人脸的矩形框,存储规则x0,y0,w,h，size>=4
		Mat& matFaceImage,          //输出，裁切的人脸
		const int nFaceWidth,       //输入，裁切的人脸宽
		const int nFaceHeight,      //输入，裁切的人脸高
		const int nFaceChannel = -1 //输入，当pImage为4通道时，该参数才有意义
		);

	bool Crop(
		unsigned char* pImage,      //输入，图像指针，只能1-4通道，若为4通道，alpha通道在内部会被改变
		int nWidth, int nHeight,    //输入，图像长宽
		int nChannel,               //输入，通道数，只能是1-4范围内的通道数
		const int* pCropRect,       //输入，裁剪人脸的矩形框,存储规则x0,y0,w,h，size>=4
		unsigned char* pFaceImage,  //输出，裁切的人脸
		const int nFaceWidth,       //输入，裁切的人脸宽
		const int nFaceHeight       //输入，裁切的人脸高
		);

	/*
	* @param [pfFAPoints] input: size = nFAPoints * 2, 按x,y,x,y...依次存储
	* @param [pfWarpFAPpints] output: size = nFAPoints * 2, 按x,y,x,y...依次存储
	* @param [pfRotateMat/pdRotateMat] input: size >= 6 计算旋转变换矩阵，0-5代表2x3变换矩阵
	*                                                          
	* @return void
	*/
	void ComputeWarpFAPoints(
		const float* pfFAPoints,  //输入，原始图像上的人脸点，按x,y,x,y...依次存储
		float* pfWarpFAPpints,    //输出，经旋转变换矩阵以及图像裁剪得到的人脸点
		const int nFAPoints,      //输入，人脸点数
		const double* pdRotateMat
		);

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
	* @param [pTranMat] input: size >= 6 计算旋转变换矩阵，0-5代表2x3变换矩阵，6-7代表crop起始位置
	*                              
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
	* @param [nFacePoint] input: face point num, must be equal to 17, 33, 45 77, or 106, 118
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
	bool ComputeFaceTransParam(
		const int nWidth, const int nHeight,           //输入，图像长宽
		const int nWarpWidth, const int nWarpHeight,   //输入，人脸图像Warp尺寸，可为-1
		const float* pfFacePoints, int nFacePoint,     //输入，人脸点及点数，必须为17, 33, 45 77, or 106, 118
		float* afInnerOuterRect,                       //输出，裁剪人脸的内框和外框
		float& fRotateRad,                             //输出，人脸需要warp的角度
		double* pdRotateMat,                           //输出，旋转2x3变换矩阵
		int& nRealWarpWidth,                           //输出，人脸裁剪实际宽度
		int& nRealWarpHeight,                          //输出，人脸裁剪实际高度
		const float* afExtentParam = 0,                //输入，人脸4个方向的延拓参数
		const float fExtentRect = 0.0f,                //输入，人脸在上述延拓基础上，整体延拓的参数
		int* pnFaceValidRect = 0                       //输出，裁切的人脸的有效矩形框区域
		);
	
	/*
	* 若人脸点数不是17, 33, 45 77, or 106, 118，必须指定下述2个参数，即必须满足
	* fForeheadDirX * fForeheadDirX + fForeheadDirY * fForeheadDirY > 0.000001
	* 否则，不满足上述条件，将返回false
	* @param [fForeheadDirX] input: 下巴指向额头的方向，x方向
	* @param [fForeheadDirY] input: 下巴指向额头的方向，y方向
	*/
	bool ComputeFaceTransParam(
		const int nWidth, const int nHeight,           //输入，图像长宽
		const int nWarpWidth, const int nWarpHeight,   //输入，人脸图像Warp尺寸，可为-1
		const float* pfFacePoints, int nFacePoint,     //输入，人脸点及点数
		const float fForeheadDirX,                     //输入，下巴指向额头的方向，x方向
		const float fForeheadDirY,                     //输入，下巴指向额头的方向，y方向
		float* afInnerOuterRect,
		float& fRotateRad,
		double* pdRotateMat,          
		int& nRealWarpWidth,
		int& nRealWarpHeight,
		const float* afExtentParam = 0,
		const float fExtentRect = 0.0f,
		int* pnFaceValidRect = 0  
		);

	bool ComputeFaceRectByFAPoints(
		const int nWarpWidth, const int nWarpHeight,
		const float* pfFacePoints, int nFacePoint,
		float* afInnerOuterRect, float& fRotateRad,
		const float* afExtentParam = 0, const float fExtentRect = 0.0f);

	bool ComputeFaceRectByFAPoints(
		const int nWarpWidth, const int nWarpHeight,
		const float* pfFacePoints, int nFacePoint,
		const float fForeheadDirX, const float fForeheadDirY,
		float* afInnerOuterRect, float& fRotateRad,
		const float* afExtentParam = 0, const float fExtentRect = 0.0f);

	bool ComputeFaceCropParam(
		const int nWidth, const int nHeight,           //输入，图像长宽
		const int nCropWidth, const int nCropHeight,   //输入，人脸图像Crop尺寸，可为-1
		const float* pfFacePoints, int nFacePoint,     //输入，人脸点及点数
		int* anCropRect,
		float& fRotateRad,
		double* pdCropMat,
		int& nRealCropWidth,
		int& nRealCropHeight,
		const float* afExtentParam = 0,
		const float fExtentRect = 0.0f,
		int* pnFaceValidRect = 0,
		const int nCropMode = CropCenter);

	bool ComputeFaceCropParam(
		const int nWidth, const int nHeight,           //输入，图像长宽
		const int nCropWidth, const int nCropHeight,   //输入，人脸图像Crop尺寸，可为-1
		const float* pfFacePoints, int nFacePoint,     //输入，人脸点及点数
		const float fForeheadDirX,                     //输入，下巴指向额头的方向，x方向
		const float fForeheadDirY,                     //输入，下巴指向额头的方向，y方向
		int* anCropRect,
		float& fRotateRad,
		double* pdCropMat,
		int& nRealCropWidth,
		int& nRealCropHeight,
		const float* afExtentParam = 0,
		const float fExtentRect = 0.0f,
		int* pnFaceValidRect = 0,
		const int nCropMode = CropCenter);

	bool ComputeFaceCropCenterParam(
		const int nWidth, const int nHeight,           //输入，图像长宽
		const int nCropWidth, const int nCropHeight,   //输入，人脸图像Crop尺寸，可为-1
		const float* pfFacePoints, int nFacePoint,     //输入，人脸点及点数
		const float fForeheadDirX,                     //输入，下巴指向额头的方向，x方向
		const float fForeheadDirY,                     //输入，下巴指向额头的方向，y方向
		int* anCropRect,
		float& fRotateRad,
		double* pdCropMat,
		int& nRealCropWidth,
		int& nRealCropHeight,
		const float* afExtentParam = 0,
		const float fExtentRect = 0.0f,
		int* pnFaceValidRect = 0);

	bool ComputeFaceRectByFAPoints(
		const float* pfFacePoints, int nFacePoint,
		float* afInnerOuterRect, float& fRotateRad,
		const float* afExtentParam = 0, const float fExtentRect = 0.0f);

	bool ComputeFaceRectByFAPoints(
		const float* pfFacePoints, int nFacePoint,
		const float fForeheadDirX, const float fForeheadDirY,
		float* afInnerOuterRect, float& fRotateRad,
		const float* afExtentParam = 0, const float fExtentRect = 0.0f);

protected:
	bool ComputeRotateParam(const float* pFacePoints, int nFacePoint, 
		float* aRotatedRectPts8, double& RotateRad, float* aInnerRectPts8 = 0);

	void ComputeFaceTransMat(
		const int nWidth, const int nHeight,    //输入，图像长宽
		const int nWarpWidth, const int nWarpHeight,
		const float* afFaceRect,    //输入，人脸框，外框
		const float fRotateRad,     //输入，人脸框的角度，单位为弧度
		double* pdRotateMat,          //输出，旋转变换矩阵
		int& nRealWarpWidth,
		int& nRealWarpHeight,
		int* pnFaceValidRect = 0   //输出，裁切的人脸的有效矩形框区域
		);

	int m_aBorderConst[4];
	int m_nBorderMode;
	int m_nInterplation;
	
};

/*example示例调用过程：
FaceWarper fw(MT_INTER_LINEAR, MT_BORDER_CONSTAN, 0);
float afInnerOuterRect[16], fRotRad;
double adRotMat[6];

fw.ComputeFaceTransParam(nWidth, nHeight, imgFace.face_width, imgFace.face_height,
imgFace.face_points_ptr(), imgFace.face_points_num,
afInnerOuterRect, fRotRad, adRotMat,
imgFace.face_width, imgFace.face_height,
afExtentParam, fExtentRect, imgFace.face_valid_rect);

fw.RunWithWarp_fast(pImage, nWidth, nHeight, 4, adRotMat, imgFace.face_aligned_image,
imgFace.face_width, imgFace.face_height,imgFace.face_channel);

for (int i = 0; i < 6; i++)
{
imgFace.tran_mat[i] = float(adRotMat[i]);
}

fw.ComputeWarpFAPoints(imgFace.face_points_ptr(), imgFace.face_aligned_points_ptr(),
imgFace.face_points_num, adRotMat);
*/

}