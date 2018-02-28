#pragma once

#include <mtface/Common/MTFeatureDetector.h>
#include <mtface/Common/MTTypes.h>
#include <mtface/Common/MTDefine.h>
#include <mtface/MTFaceConfig.hpp>

// compatibility GNU compiler
#undef major
#undef minor

namespace mtface
{

class MEITU_EXPORT CMeituFDDetectorV2 : public MTFeatureDetector
{
public:
    struct Config
    {
        Config() : minsize(192),rpn_score(0.6), fa_score(0.01f),fa_box_maxnum(10),multi_thread(true)
        {
            
        }
        bool multi_thread;
        int minsize;
        int fa_box_maxnum;
        float rpn_score;
        float fa_score;
    };
    
public:
    CMeituFDDetectorV2();
    ~CMeituFDDetectorV2();
    
    //Load FD model
    /*
     * @models [ModelData]
     * if models path is NULL,use built-in model(!!!EXCEPT IOS!!!)
     * @return true on success
     *		  false on fail
     */
    MTRET LoadModels(MTModels *models = NULL);
    
    MTRET SetConfig(void *config);
    MTRET GetConfig(void *config);
    
    //Run FD
    /*
     * @param [image]    input image
     * @param [feature]  output feature
     * @return >=0 on success
     *		  < 0 on fail
     */
    MTRET Detect(const MTImage& image, MTVector<MTFaceFeature> &features);
    
private:
    MTFeatureDetector *m_handle;
};

} // namespace mtface
