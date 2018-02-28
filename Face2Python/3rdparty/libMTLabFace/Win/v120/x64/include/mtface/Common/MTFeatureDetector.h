#pragma once

#include <mtface/Common/MTModels.h>
#include <mtface/Common/MTImage.h>
#include <mtface/Common/MTFaceFeature.h>

namespace mtface
{

// 检测接口通用基类
class MTFeatureDetector
{
public:
    MTFeatureDetector() {}
    virtual ~MTFeatureDetector() {}
    
	/*
	 * LoadModel
	 */
    virtual MTRET LoadModels(MTModels *models = NULL) = 0;

	/*
	 * Config
	 */
	virtual MTRET SetConfig(void *config) = 0;
	virtual MTRET GetConfig(void *config) = 0;

	/*
     * @param input  [image]   待检测图像
     * @param output [feature] 检测结果信息
	 */
	virtual MTRET Detect(const MTImage& image, MTVector<MTFaceFeature> &features) = 0;
};

} // namespace mtface
