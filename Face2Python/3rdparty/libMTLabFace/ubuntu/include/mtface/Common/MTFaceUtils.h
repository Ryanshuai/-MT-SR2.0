#ifndef _mtface_COMMON_MTWARPUTILS_H_
#define _mtface_COMMON_MTWARPUTILS_H_
#include "mtface/Common/MTDefine.h"
#include "mtface/Common/MTTypes.h"
#include "mtface/Common/MTFaceFeature.h"
#include "mtface/Common/MTImage.h"

namespace mtface{

/// @struct WarpConfig
/// 裁图的配置参数
struct WarpConfig{
    ///< padding 比例, 相对于人脸的外包框, 如果是 17点, 相对于转正后的外包框
    float padding_left = 0.f;
    float padding_right = 0.f;
    float padding_top = 0.f;
    float padding_bottom = 0.f;
};

struct Color{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;
};

enum BorderTypes {
    BORDER_CONSTANT = 0,        //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
    BORDER_REPLICATE = 1,       //!< `aaaaaa|abcdefgh|hhhhhhh`
    BORDER_REFLECT = 2,         //!< `fedcba|abcdefgh|hgfedcb`
    BORDER_WRAP = 3,            //!< `cdefgh|abcdefgh|abcdefg`
    BORDER_REFLECT_101 = 4,     //!< `gfedcb|abcdefgh|gfedcba`
    BORDER_TRANSPARENT = 5,     //!< `uvwxyz|absdefgh|ijklmno`
    BORDER_REFLECT101 = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
    BORDER_DEFAULT = BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
    BORDER_ISOLATED = 16 //!< do not look outside of ROI
};

enum InterpolationFlags{
    INTER_NEAREST = 0,
    INTER_LINEAR = 1,
    INTER_CUBIC = 2,
    INTER_AREA = 3,
    INTER_LANCZOS4 = 4,
    INTER_MAX = 7,
};
/// ===========================================================================
/// 一些有用的辅助函数
MEITU_EXPORT MTRect2f CalcBoundingBox(const MTVector<MTPoint2f>& points);
MEITU_EXPORT float    CalcFaceAngleDeg(const MTVector<MTPoint2f>& points);      // 度
MEITU_EXPORT float    CalcFaceAngleRad(const MTVector<MTPoint2f>& points);      // 弧度
MEITU_EXPORT void     TransformPoints(const MTVector<MTPoint2f>& src, MTVector<MTPoint2f>& dst, const float M[6]);
MEITU_EXPORT void     InvertAffineTransform(const float M[6], float iM[6]);
/// ===========================================================================
/// @brief 计算裁图的变换矩阵
/// @param M[6]       [out] 2x3 相似变换矩阵, row major
/// @param size       [in]  裁图最终的大小
/// @param points     [in] 人脸点
/// @param config     [in] 裁图的参数
MEITU_EXPORT void CalcWarpTransform(float M[6], MTSize2i size, const MTVector<MTPoint2f>& points, WarpConfig config = WarpConfig());
/// @param roll_angle [in] 人脸角度
MEITU_EXPORT void CalcWarpTransform(float M[6], MTSize2i size, const MTVector<MTPoint2f>& points, float roll_angle, WarpConfig config = WarpConfig());
/// @param bounds     [in] 人脸框
MEITU_EXPORT void CalcWarpTransform(float M[6], MTSize2i size, MTRect2f bounds, WarpConfig config = WarpConfig());

/// ===========================================================================
/// @brief 人脸裁图
/// @param image  [in] 输入人脸图
/// @param result [out] 输出摆正的人脸
/// @param size   [in] 输出图像大小
/// @param points [in] 人脸点, 默认支持 118 点, 77点, 17点
/// @param config [in] 裁图相关参数, 参见 @class WarpConfig
/// @param inter_flags [in] 插值方式
/// @param border_mode [in] 扩边方式
/// @param border_value [in] 扩边颜色 rgba
MEITU_EXPORT void WarpFace(const MTImage &image, MTImage &result,
    MTSize2i size, 
    const MTVector<MTPoint2f> points, 
    const WarpConfig& config = WarpConfig(), 
    int inter_flags = InterpolationFlags::INTER_LINEAR, 
    int border_mode = BorderTypes::BORDER_CONSTANT, 
    const Color border_value = Color());

/// @brief 指定人脸角度
/// @param roll_angle [in] 人脸角度, 见MTFace
MEITU_EXPORT void WarpFace(const MTImage &image, MTImage &result,
    MTSize2i size,
    const MTVector<MTPoint2f> points, float roll_angle, 
    const WarpConfig& config = WarpConfig(),
    int inter_flags = InterpolationFlags::INTER_LINEAR,
    int border_mode = BorderTypes::BORDER_CONSTANT,
    const Color border_value = Color());

/// @brief 通过人脸框裁图, 不旋转
/// @param bounds [in] 人脸框
MEITU_EXPORT void WarpFace(const MTImage &image, MTImage &result,
    MTSize2i size, const MTRect2f bounds,
    const WarpConfig& config = WarpConfig(),
    int inter_flags = InterpolationFlags::INTER_LINEAR,
    int border_mode = BorderTypes::BORDER_CONSTANT,
    const Color border_value= Color());

/// ===========================================================================
/// @brief OpenCV的仿射变换
/// @param image  [in] 输入人脸图
/// @param result [out] 输出摆正的人脸
/// @param size   [in] 输出图像大小
/// @param M      [in] 仿射变换矩阵, 行优先排列 2x3
/// @param inter_flags [in] 插值方式
/// @param border_mode [in] 扩边方式
/// @param border_value [in] 扩边颜色 rgba
MEITU_EXPORT void WarpAffine(const MTImage &image, MTImage &result, MTSize2i size, const float M[6],
    int inter_flags = InterpolationFlags::INTER_LINEAR, int border_mode = BorderTypes::BORDER_CONSTANT, 
    const Color border_value = Color());



}


#endif
