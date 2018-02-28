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
            // 内部使用人脸检测的情况下有效，每隔fd_gap_frames做一次FD，或者每隔fd_gap_frames*30ms做一次检测。
            fd_gap_frames = 15;                 // [NO]
            // （逻辑上）最大跟踪的人脸数目，可动态调整
            max_track_faces = 5;                // [NO]
            // （实现上）人脸缓冲区的大小，改变需要StopTracking才能起效
            reserve_track_faces = 5;            // [YES]
            // 内部参数
            optical_flow_wsize = 11;            // [NO]
            // 内部参数
            fa_score_threshold = 0.0f;          // [NO]
            // 用于设置内部FD的分数
            fd_score_threshold = 0.0f;          // [NO]
            // 内部参数
            clamp_param = 1.0f;                 // [NO]
            // 内部参数
            optical_flow_fusion_factor = 15.f;  // [NO]
            // 是否使用内部FD线程
            faceDetection_interal = true;       // [YES]
            // 是否使用光流法：使用点不抖；不使用点抖；
            run_opticalFlow = true;             // [NO]
            // 内部坐标使用百分比
            innerSave_percent = true;           // [Yes]
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
    FaceTracker();
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
