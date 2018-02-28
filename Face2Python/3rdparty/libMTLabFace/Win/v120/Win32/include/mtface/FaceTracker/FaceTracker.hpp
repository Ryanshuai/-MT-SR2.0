#ifndef FACE_TRACKER_HPP_
#define FACE_TRACKER_HPP_

#include <mtface/Common/MTDefine.h>
#include <mtface/Common/MTFeatureDetector.h>
#include <mtface/MTFaceConfig.hpp>

// compatibility GNU compiler
#undef major
#undef minor

namespace sdm
{
    class FaceTrackerImp;
}

namespace mtface
{
    
class MEITU_EXPORT FaceTracker : public MTFeatureDetector
{
public:
    struct Config
    {
        Config() : major(0), minor(0), patch(0), isTracking(false)
        {   // Need Restart?
            fd_gap_frames = 15;                 // [NO]
            max_track_faces = 5;                // [NO]
            reserve_track_faces = 5;            // [YES]
            optical_flow_wsize = 21;            // [NO]
            fa_score_threshold = 0.0f;          // [NO]
            fd_score_threshold = 0.0f;          // [NO]
            clamp_param = 1.0f;                 // [NO]
            optical_flow_fusion_factor = 15.f;  // [NO]
            faceDetection_interal = true;       // [YES]
            run_opticalFlow = true;             // [NO]
            innerSave_percent = false;          // [NO]
        }
        
        int fd_gap_frames;
        int max_track_faces;
        int reserve_track_faces;
        int optical_flow_wsize;
        float fa_score_threshold;
        float fd_score_threshold;
        float clamp_param;
        float optical_flow_fusion_factor;
        bool faceDetection_interal;
        bool run_opticalFlow;
        bool innerSave_percent;
        
        // version info
        int major;
        int minor;
        int patch;
        
        int num_points;
        
        // tracking control
        bool isTracking;
    };

public:
#ifdef ENABLE_PLATFORM_CHECK
    FaceTracker(JNIEnv *env, jobject obj,jobject mContext);
#else
    FaceTracker();
#endif
    ~FaceTracker();
    
    /*
     * LoadModel
     */
    MTRET LoadModels(MTModels *models = NULL);
    
    /*
     * Config
     */
    MTRET SetConfig(void *config);
    MTRET GetConfig(void *config);
    
    Config& GetConfig();
    MTRET FlushConfig();
    
    
    /*
     * Detect
     */
    MTRET Detect(const MTImage& image, MTVector<MTFaceFeature> &features);
    MTRET StopTracking();
    
private:
    sdm::FaceTrackerImp *imp;
    Config* config_cache;
    bool config_cache_dirty;
};

} // namespace mtface

#endif
