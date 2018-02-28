#ifndef _H_MEITUFDDETECTOR_H_
#define _H_MEITUFDDETECTOR_H_

#include <mtface/Common/MTFeatureDetector.h>
#include <mtface/Common/MTTypes.h>
#include <mtface/Common/MTDefine.h>
#include <mtface/MTFaceConfig.hpp>

// compatibility GNU compiler
#undef major
#undef minor

#define MTFD_MODE_NORMAL	1
#define MTFD_MODE_FAST		2

namespace mtface
{

class MEITU_EXPORT CMeituFDDetector : public MTFeatureDetector
{
public:
	struct Config
	{
		Config() : score(0.f), maxFaceNum(10), mode(MTFD_MODE_NORMAL), major(0), minor(0) { }

		// input && output
		float score;       // Set the minimum value of face score in range(-1,160)
		int maxFaceNum;    // Set the maximum face detect number
		int mode;          // Set detect mode MTFD_MODE_NORMAL/MTFD_MODE_FAST

		// output
		unsigned int major; // major version number
		unsigned int minor; // minor version number
	};

public:
	CMeituFDDetector();
    ~CMeituFDDetector();
    
    //Load FD model
    /*
     * @models [ModelData]
     * if models path is NULL,use built-in model(!!!EXCEPT IOS!!!)
     * @return true on success
     *		  false on fail
     */
    MTRET LoadModels(MTModels *models = NULL);

	/*
	 * Set and get Config, need to run after LoadModel
	 */
	MTRET SetConfig(void *config);
	MTRET GetConfig(void *config);

	//Run face detect
	/*
	* @param [pData] RGBA image data
	* @param [nWidth,nHeight] image size
	* @return face count. >=0 on success 
	*		  < 0 on fail
	*/
	MTRET Detect(const MTImage& image, MTVector<MTFaceFeature> &features);
private:
	MTFeatureDetector *m_handle;
};

} // namespace mtface

#endif
