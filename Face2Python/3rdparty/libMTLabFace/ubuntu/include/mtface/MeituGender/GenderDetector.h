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

class MEITU_EXPORT CGenderDetector : public MTFeatureDetector
{
public:
    struct Config
    {
        Config() : major(0), minor(0),score(0.5)
        { }
        
        // output model version
        unsigned int major;
        unsigned int minor;
        float score; // 0-1 设定男女的分数阈值，大于这个分则为女，小于这个分数为男。越高则越容易判定为男
    };
    
public:
    CGenderDetector();
    ~CGenderDetector();
    
    //Load Gender model
    /*
     * @models [ModelData]
     * if models path is NULL,use built-in model(!!!EXCEPT IOS!!!)
     * @return true on success
     *		  false on fail
     */
    MTRET LoadModels(MTModels *models = NULL);
    
    MTRET SetConfig(void *config);
    MTRET GetConfig(void *config);
    
    //Run Gender
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
