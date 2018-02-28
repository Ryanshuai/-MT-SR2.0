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
        Config() : major(0), minor(0) { }
        
        // output model version
        unsigned int major;
        unsigned int minor;
    };

public:
#ifdef ENABLE_PLATFORM_CHECK
    CMeituFDDetectorV2(JNIEnv *env, jobject obj,jobject mContext);
#else
    CMeituFDDetectorV2();
#endif
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
