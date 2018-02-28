#ifndef _H_MEITUFADETECTOR_H_
#define _H_MEITUFADETECTOR_H_

#include <cstdio>

#include <mtface/Common/MTFeatureDetector.h>
#include <mtface/Common/MTTypes.h>
#include <mtface/Common/MTDefine.h>
#include <mtface/MTFaceConfig.hpp>

// compatibility GNU compiler
#undef major
#undef minor

namespace mtface
{

class MEITU_EXPORT CMeituFADetector : public MTFeatureDetector
{
public:
	struct Config
	{
		Config() : major(0), minor(0), numPoints(0) { }

		// output model version
		unsigned int major;
		unsigned int minor;
        
        unsigned int numPoints; // num points of landmarks per face
	};

public:
#ifdef ENABLE_PLATFORM_CHECK
	CMeituFADetector(JNIEnv *env, jobject obj,jobject mContext);
#else
	CMeituFADetector();
#endif
	~CMeituFADetector();

	//Load FA model
	/*
	* @models [ModelData]
	* if models path is NULL,use built-in model(!!!EXCEPT IOS!!!)
	* @return true on success
	*		  false on fail
	*/
    MTRET LoadModels(MTModels *models = NULL);

	MTRET SetConfig(void *config);
	MTRET GetConfig(void *config);

	//Run FA 
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

#endif
