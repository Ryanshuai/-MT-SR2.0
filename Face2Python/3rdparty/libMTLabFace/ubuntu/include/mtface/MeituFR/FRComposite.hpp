//
//  FaceInfoPipline.hpp
//  MeituFRDemo
//
//  Created by meitu on 2017/6/8.
//  Copyright © 2017年 com.meitu.mlab. All rights reserved.
//

#ifndef mtface_MeituFR_FRComposite_hpp
#define mtface_MeituFR_FRComposite_hpp

#include "mtface/Common/MTDefine.h"
#include "mtface/Common/MTFeatureDetector.h"

namespace mtface
{ 
    /// \brief 组合多个检测器， 一步完成 faceCode 提取
    class MEITU_EXPORT FRComposite : MTFeatureDetector
    {
    public:
        FRComposite();
        ~FRComposite();
        
        MTRET LoadModels(MTModels *models /* = NULL */) override;
        MTRET Detect(const MTImage& image, MTVector<MTFaceFeature> &features) override;

        /// simply forward config to FRExtractor
        MTRET SetConfig(void *config) override;
        MTRET GetConfig(void *config) override;
        
    private:
        MTFeatureDetector* _fd;
        MTFeatureDetector* _fr;
    };
}

#endif /* mtface_MeituFR_FRComposite_hpp */
