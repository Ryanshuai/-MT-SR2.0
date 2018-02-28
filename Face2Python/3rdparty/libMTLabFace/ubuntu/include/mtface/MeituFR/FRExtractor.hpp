//
//  FRExtractor.hpp
//  MeituFRDemo
//
//  Created by meitu on 2017/6/9.
//  Copyright © 2017年 com.meitu.mlab. All rights reserved.
//

#ifndef mtface_MeituFR_FRExtractor_hpp
#define mtface_MeituFR_FRExtractor_hpp

#include "mtface/Common/MTDefine.h"
#include "mtface/Common/MTFeatureDetector.h"

#undef major
#undef minor

namespace mtface
{

    /// \class FRExtractor 提取 faceCode， 在调用FRExtractor 之前必须先进行 FD, FA
    ///        **对于新版FD输出人脸17点, 可以不用FA**
	///        如果希望一步提取 faceCode, 请使用组合检测器 FRComposite
    class MEITU_EXPORT FRExtractor : public MTFeatureDetector{
    public:
        struct Config{
            unsigned int major;     /// 主版本号
            unsigned int minor;     /// 副版本号
            bool saveFaceThumbnail; /// 是否存储人脸预览图, 默认 false
            Config() :major(0), minor(1), saveFaceThumbnail(false){}
        };
    public:
        FRExtractor();
        ~FRExtractor();
        FRExtractor(const FRExtractor& rhs) = delete;
        FRExtractor& operator = (const FRExtractor &) = delete;
        MTRET LoadModels(MTModels *models = NULL) override;
        MTRET Detect(const MTImage& image, MTVector<MTFaceFeature> &features) override;
        MTRET SetConfig(void* config) override;
        MTRET GetConfig(void* config) override;
    private:
        struct FRExtractor_Impl;
        FRExtractor_Impl* pImpl;
    };
}

#endif /* mtface_MeituFR_FRExtractor_hpp */
