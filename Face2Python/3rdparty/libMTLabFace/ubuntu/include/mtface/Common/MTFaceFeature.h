#pragma once

#include <mtface/Common/MTDefine.h>
#include <mtface/Common/MTTypes.h>
#include <mtface/Common/MTVector.h>
#include <mtface/Common/MTImage.h>

#include <stdio.h>
#include <iostream>

namespace mtface
{

// 所有人脸特征
class MEITU_EXPORT MTFaceFeature
{
public:
    enum GENDER{
        FEMALE = 0,
        MALE = 1
    };
    
    enum RACE{
        WHITE = 0,
        YELLOW = 1,
        BLACK=2
    };
    
    enum CHEEKSHAPE{
        HIGH = 0, FLAT = 1
    };
    enum JAWSHAPE{
        SQUARE = 0, SHARP = 1, ROUND = 2
    };

public:

	MTFaceFeature();

	bool                            hasFaceBounds;
	MTRect_<float>                  faceBounds;             // 人脸框

	bool                            hasFacePoints;
	MTVector<MTPoint_<float> >      facePoints;             // 人脸点

	bool                            hasTrackingId;
	int                             trackingId;             // 人脸TrackingId
    
	/*******************************************************
						face angle illustration
										270°
										|
						 180° --------(-_-)---------> x     0°
										|
										|
										\/
										y
										90°
						人脸方向为双眼连线与X轴的夹角
	********************************************************/
    bool                            hasRollAngle;
    float                           rollAngle;              // 人脸角度

    bool							hasGender;				// 性别分类算法直接填入性别
    GENDER							gender;
    
    bool                            hasRace;                // 性别分类算法直接填入性别
    RACE                            race;
    
    bool                            hasAge;
    int                             age;

    bool                            hasFaceCode;
    MTVector<float>                 faceCode;
    bool                            hasClusterID;
    int                             clusterID;
    bool                            hasFaceThumbnail;       // 人脸预览图
    MTImage                         faceThumbnail;      

    bool                            hasCheekShape;
    CHEEKSHAPE                      cheekShape;
    bool                            hasJawShape;
    JAWSHAPE                        jawShape;

	//bool                            hasGenderScores;
	//MTVector<float>					genderScores;           // 性别分数
	//bool                            hasGenderLabels;
	//MTVector<GENDER_LABELS>			genderLabels;

};

// 辅助函数

// @brief 将FaceFeature的坐标系转换到 target_orientation 的坐标系中
// 使用场景说明： 进行FD的时候， 根据orientation对图像数据进行“转正”，
// 得到的人脸框是基于转正后的方向， 输出时保持框与数据坐标一致， 需要
// 将人脸框转回原来的坐标席中

// @brief 将当前orientation的 feature 坐标转正成 orientation = 1 的坐标
// 使用场景说明： feature的坐标是基于图像数据的排布的， 该函数是将图像根据
// orientation转正后的feature 坐标
// @param [in|out] feature 输入的MTFaceFeature, 及转正后的结果
// @param [in] size 输入feature的基准宽高
// @param [in] orientation 输入feature 的基准orientaion
MEITU_EXPORT int RotateFaceFeature(const MTVector<MTFaceFeature>& src, MTVector<MTFaceFeature>& dst,
    MTSize_<int> size, int src_orientation, int dst_orientation);

MEITU_EXPORT int ResizeFaceFeature(const MTVector<MTFaceFeature>& src, MTVector<MTFaceFeature>& dst,
    MTSize_<int> src_size, MTSize_<int> dst_size);

// 镜像人脸点
// @param [in] src 人脸点, 
// @param [out] dst 镜像后的人脸点
MEITU_EXPORT int MirrorFacePoints(const MTVector<MTPoint2f>& src, MTVector<MTPoint2f>& dst);

} // namespace mtface

MEITU_EXPORT std::ostream& operator<<(std::ostream& o, mtface::MTFaceFeature src);
