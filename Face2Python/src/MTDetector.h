/*****************************************************************
* FDFA Detector ,Detect face ,and align the face ,crop the face 
* Copyright (c) 2017 MEITU. All rights reserved.
*
* @version: 1.0
*
* @author:  lzh
*
* @update :2017-12-07 lzh1@meitu.com
*
* @note:
*
* @usage：
*
******************************************************************/

#pragma once

#include <vector>
#include <mtface/Common/Common.hpp>
#include <mtface/Common/MTImage.h>
#include <mtface/Common/MTFaceUtils.h>
#include <mtface/MeituFDV2/MeituFDDetectorV2.h>
#include <mtface/MeituFA/MeituFADetector.h>
#include <mtface/MeituGender/GenderDetector.h>
#include <mtface/MeituRace/RaceDetector.h>
#include <mtface/MeituAge/AgeDetector.h>
#include <mtface/FaceTracker/FaceTracker.hpp>
#include <mtface/MeituFR/FRExtractor.hpp>       // face recognition feature extractor
#include <mtface/MeituFR/FRClusterer.hpp>           // face cluster
#include <mtface/FacialAnalysis/CheekDetector.h>    // cheek classifier
#include <mtface/FacialAnalysis/JawDetector.h> 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/version.hpp>



namespace mtface
{

struct FDRect {
	float x,y, width,height;
};

struct FDPoints {
  std::vector<cv::Point2f> Points;
};

/*This class is for recieving image data from input source*/
class  FDImage{
public:
  FDImage();
  FDImage(const int cols,const int rows, const int chans, void* data);
  ~FDImage();
  int height;
  int width;
  int channels;
  unsigned char* data;
};


struct MTFaceConfig
{

     MTFaceConfig():MeituFD(true),MeituFA(false),MeituFT(false),MeituAge(false),MeituRace(false),MeituGender(false), MeituCrop(false),FD_MINSIZE(256),FD_FASCORE(0.1f),CROP_SIZE_H(100),CROP_SIZE_W(100),FA_MAX_NUM(20),MeituFacial(false),MeituFR(false)
     {
	    // EXTERN_RATE.resize(4);
     //  	EXTERN_RATE[0]=0.2f;
     //  	EXTERN_RATE[1]=0.2f;
     //   	EXTERN_RATE[2]=0.2f;
	    // EXTERN_RATE[3]=0.2f;
	    EXTERN_LEFT=0.0f;
	    EXTERN_RIGHT=0.0f;
	    EXTERN_BOTTOM=0.0f;
	    EXTERN_UP=0.0f;

}

    bool MeituFD;
    bool MeituFA;
    bool MeituFT;
    bool MeituAge;
    bool MeituRace;
    bool MeituGender;
    bool MeituCrop;
    bool MeituFR;
    bool MeituFacial;

    int CROP_SIZE_H;
	int CROP_SIZE_W;
    int FD_MINSIZE;
    float FD_FASCORE;
 	
 	float EXTERN_LEFT;
 	float EXTERN_RIGHT;
 	float EXTERN_BOTTOM;
 	float EXTERN_UP;
    //std::vector<float> EXTERN_RATE;
    int  FA_MAX_NUM;   
};



struct FDFAResult
{

		FDFAResult():hasTrackingId(false),hasCrop(false),hasRollAngle(false),hasGender(false),hasRace(false),hasAge(false),hasFaceCode(false),hasClusterID(false),hasCheekShape(false),hasJawShape(false){
       
}
   		cv::Mat mCropImage; // crop_warped face image
		std::vector<float>   WarpMatrix;
		std::vector<float> InversWarpMatrix;
             
		FDRect mFaceRect;	// face rect in origin image	
		
		
		bool    hasCrop;
		FDRect mCropFaceRect;   // face rect in croped image
                std::vector<cv::Point2f> mFacePoints;  //face points in origin image
       		std::vector<cv::Point2f> mCropFacePoints;  //face points in warped image

		bool    hasTrackingId;
                int     trackingId;             
   
                bool    hasRollAngle;
                float   rollAngle;              // 人脸角度

                bool	hasGender;				// 性别分类算法直接填入性别
                int	gender;
    
   	 	bool     hasRace;                // 性别分类算法直接填入性别
    		int     race;
    
    		bool     hasAge;
    		int      age;	


		bool                            hasFaceCode;
    		std::vector<float>              faceCode;
    		bool                            hasClusterID;
    		int                             clusterID;
   
    		bool                            hasCheekShape;
    		int                             cheekShape;
    		bool                            hasJawShape;
    		int                             jawShape;
	};


class MTDetector
{

public:
	MTDetector();
	~MTDetector();
	void LoadFDModel(const char* pFDPModelName, const char* pFDAModelName);
	void LoadFAModel(const char* pFAModelName);
	void LoadFDModel();
	void LoadFAModel();
	void LoadFTModel();
	void LoadAgeModel();
  void LoadRaceModel();
	void LoadGenderModel(); 
  void LoadJawModel(); 
	void LoadCheekModel(); 
	void LoadFRModel(); 

  std::vector<FDFAResult>RunMeituFace(const cv::Mat &ImageIn,const MTFaceConfig &FaceConfig);

  std::vector<FDFAResult> RunMeituFaceRects(const cv::Mat &ImageIn,std::vector<FDRect> Rects,const MTFaceConfig &FaceConfig);
  std::vector<FDFAResult> RunMeituFacePoints(const cv::Mat &ImageIn,std::vector<FDPoints> Points,const MTFaceConfig &FaceConfig);
 
  int RunCluster(std::vector<FDFAResult> &FDFAFeatures);


protected:

	mtface::CMeituFDDetectorV2 mtFD;
	mtface::CMeituFADetector mtFA;
        mtface::CGenderDetector mtGender;
	mtface::CAgeDetector mtAge;
	mtface::CRaceDetector mtRace;
	mtface::FaceTracker mtFT;
	mtface::CJawDetector mtJaw;
    	mtface::CCheekDetector mtCheek;
   	mtface::FRExtractor mtFR;
};


}

