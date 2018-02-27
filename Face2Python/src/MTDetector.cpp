#include "MTDetector.h"


#include <libyuv/rotate_argb.h>
using namespace libyuv;
using namespace mtface;
using namespace cv;


#include <iostream>
#include <fstream>
#include <time.h>

#include <mtface/Common/MTFaceUtils.h>
#include <mtface/Common/MTDefine.h>

namespace mtface
{



static inline void fastFree(void* ptr)
{
	if (ptr)
	{
		unsigned char* udata = ((unsigned char**)ptr)[-1];
		free(udata);
	}
}


FDImage::FDImage()
{
   width=0;
   height=0;
   channels=0;
   data=NULL;
}


 FDImage::FDImage( const int cols, const int rows,const int chans ,void* udata)
 {
      height=rows;
	  width=cols;
      channels=chans;
      data=new unsigned char[rows*cols*chans];
	  memcpy(data, udata,rows*cols*chans*sizeof(unsigned char) );
 }


FDImage::~FDImage()
{
  width = height=channels= 0;
  fastFree(data);
}




void  MTDetector::LoadFDModel(const char* pFDPModelName, const char* pFDAModelName)
	{  
			mtface::MTModels modelFD;
		if (modelFD.PushModelFromFile(pFDPModelName, mtface::MTModels::Type::MEITU_FDV2_RPN_MODEL_TYPE) &&
			modelFD.PushModelFromFile(pFDAModelName, mtface::MTModels::Type::MEITU_FDV2_FA_MODEL_TYPE))
		{
			mtFD.LoadModels(&modelFD);
		}
	}

void MTDetector::LoadFDModel()
	{
		mtFD.LoadModels(NULL);
	}	

void MTDetector::LoadFAModel(const char* pFAModelName)
	{ 
		mtface::MTModels modelFA;
		if (modelFA.PushModelFromFile(pFAModelName, mtface::MTModels::Type::MEITU_FA_MODEL_TYPE))
		{
		mtFA.LoadModels(&modelFA);
		}
		
	}


void MTDetector::LoadFAModel()
{
	mtFA.LoadModels(NULL);
}

  
void MTDetector::LoadFTModel()
{
	mtFT.LoadModels(NULL);
}

void MTDetector::LoadAgeModel()
{
	mtAge.LoadModels(NULL);
}

void MTDetector::LoadRaceModel()
{
	mtRace.LoadModels(NULL);
}

void MTDetector::LoadGenderModel()
{
	mtGender.LoadModels(NULL);
}

void MTDetector::LoadJawModel()
{
	mtJaw.LoadModels(NULL);
}

void MTDetector::LoadCheekModel()
{
	mtCheek.LoadModels(NULL);
}

void MTDetector::LoadFRModel()
{
	mtFR.LoadModels(NULL);
}


MTDetector::MTDetector()
{
	 LoadFDModel();
     LoadFAModel();
	 LoadFTModel();
	 LoadAgeModel();
     LoadRaceModel();
	 LoadGenderModel();
	 LoadJawModel(); 
	 LoadCheekModel(); 
     LoadFRModel();  
}


MTDetector::~MTDetector()
{
     
}



std::vector<FDFAResult> MTDetector::RunMeituFace(const cv::Mat &ImageIn,const MTFaceConfig &FaceConfig)
{

    std::vector<FDFAResult> Result;
     Result.clear();
	 cv::Mat cvrgba;

     cv::cvtColor(ImageIn, cvrgba, CV_BGR2RGBA);

	 mtface::MTImage image = mtface::MTImage::from_rgba(cvrgba.cols, cvrgba.rows, cvrgba.data);
    
     mtface::MTVector<mtface::MTFaceFeature> features;

	 mtface::CMeituFDDetectorV2::Config cfg;
	 cfg.minsize=FaceConfig.FD_MINSIZE;

	// cfg.rpn_score=FaceConfig.RPN_SCORE;
	 cfg.fa_score=FaceConfig.FD_FASCORE;
	 
	 cfg.fa_box_maxnum=FaceConfig.FA_MAX_NUM;

	 mtFD.SetConfig(&cfg);
	 mtFD.Detect(image, features);
    
	if(FaceConfig.MeituFA|| FaceConfig.MeituAge || FaceConfig.MeituRace|| FaceConfig.MeituGender)
	{
		mtFA.Detect(image,features);		   
	}
    
	if(FaceConfig.MeituFT)
	{
         
         mtFT.Detect(image,features);
	}

	if(FaceConfig.MeituAge)
	{

         mtAge.Detect(image,features);
	}

	if(FaceConfig.MeituRace)
	{

         mtRace.Detect(image,features);
	}

	if(FaceConfig.MeituGender)
	{

         mtGender.Detect(image,features);
	}

	if(FaceConfig.MeituFacial)
	{
	   mtJaw.Detect(image, features);
       mtCheek.Detect(image, features);
	}
	if(FaceConfig.MeituFR)
	{

         mtFR.Detect(image,features);
	}


	if (features.size()>0)
    {
		
   		mtface::WarpConfig config;
   		config.padding_left = FaceConfig.EXTERN_LEFT;
    	config.padding_right = FaceConfig.EXTERN_RIGHT;
    	config.padding_bottom =FaceConfig.EXTERN_BOTTOM;
    	config.padding_top =FaceConfig.EXTERN_UP;
		
	   
        
		// save crop image
       for(auto i=0;i<features.size();i++)
	   {


		    FDFAResult temp_result;
			//float iM[6];
		    temp_result.mFaceRect.x=features[i].faceBounds.x;
			temp_result.mFaceRect.y=features[i].faceBounds.y;
			temp_result.mFaceRect.width=features[i].faceBounds.width;
			temp_result.mFaceRect.height=features[i].faceBounds.height;
			int crop_h,crop_w;
			if((FaceConfig.CROP_SIZE_H>0)&&(FaceConfig.CROP_SIZE_W>0))
				{
						crop_h=FaceConfig.CROP_SIZE_H;
						crop_w=FaceConfig.CROP_SIZE_W;
				}
				else
				{
					crop_h=features[i].faceBounds.height*2.0f;
					crop_w=features[i].faceBounds.width*2.0f;
				}

			if(FaceConfig.MeituCrop)
			{
				temp_result.hasCrop=true;
				MTImage face;
				float M[6];
				float iM[6];
				
				mtface::MTVector<mtface::MTPoint2f> Warpfacepoints;
				//std::cout<<crop_w<<","<<crop_h<<std::endl;
				mtface::CalcWarpTransform(M,mtface::MTSize2i(crop_w, crop_h),features[i].facePoints, config);
				mtface::WarpAffine(image, face,mtface::MTSize2i(crop_w, crop_h), M);
				//mtface::InvertAffineTransform(M,iM);
				TransformPoints(features[i].facePoints,Warpfacepoints,M);

				//mtface::WarpAffine(image,face, mtface::MTSize2i(100, 100), M);
				InvertAffineTransform(M, iM);
				
				//mtface::WarpAffine(image,face, mtface::MTSize2i(100, 100), M);
				temp_result.WarpMatrix.resize(6);
				temp_result.InversWarpMatrix.resize(6);
				
				for(int j=0;j<6;++j)
				{
					temp_result.WarpMatrix[j]=M[j];
					temp_result.InversWarpMatrix[j]=iM[j];
				}
		
				cv::Mat CropImage(face.height,face.width,CV_8UC4, face.data);
				cv::Mat bgr;
				cv::cvtColor(CropImage, bgr, CV_RGBA2BGR);

				temp_result.mCropImage=bgr;
				
				

				

				auto inner_points=Warpfacepoints.size();
				std::vector<float> point_x(inner_points);//
				std::vector<float> point_y(inner_points);
				
				
				for(int k = 0; k < inner_points; k++)
				{
					point_x[k] = Warpfacepoints[k].x;
					point_y[k] = Warpfacepoints[k].y;
					// std::cout<<point_x[k]<<std::endl;
				}
            // 根据前26个点（内五官点），最大最小值 包出一个框
				std::vector<float>::iterator biggest_x=std::max_element(std::begin(point_x),std::end(point_x));
				std::vector<float>::iterator biggest_y=std::max_element(std::begin(point_y),std::end(point_y));
				std::vector<float>::iterator smallest_x=std::min_element(std::begin(point_x),std::end(point_x));
				std::vector<float>::iterator smallest_y=std::min_element(std::begin(point_y),std::end(point_y));
				float max_x=(*biggest_x);
				float max_y=(*biggest_y);
				float min_x=(*smallest_x);
				float min_y=(*smallest_y);

		
				temp_result.mCropFaceRect.x=static_cast<int>(min_x);
				temp_result.mCropFaceRect.y=static_cast<int>(min_y);
				temp_result.mCropFaceRect.width=static_cast<int>(max_x-min_x);
				temp_result.mCropFaceRect.height=static_cast<int>(max_y-min_y);


				temp_result.mCropFacePoints.resize(Warpfacepoints.size());
				for(auto j=0;j<Warpfacepoints.size();++j)
				{
						temp_result.mCropFacePoints[j]=cv::Point2f(Warpfacepoints[j].x,Warpfacepoints[j].y);
				}

			}
			
			temp_result.mFacePoints.resize(features[i].facePoints.size());
			for(auto j=0;j<features[i].facePoints.size();++j)
			{
					temp_result.mFacePoints[j]=cv::Point2f(features[i].facePoints[j].x,features[i].facePoints[j].y);
			}

			if(features[i].hasRollAngle)
			{
				temp_result.hasRollAngle=true;
				temp_result.rollAngle=features[i].rollAngle;
			}
		

			if(features[i].hasTrackingId)
			{
				temp_result.hasTrackingId=true;
				temp_result.trackingId=features[i].trackingId;
			}
               
			if(features[i].hasGender)
			{
				temp_result.hasGender=true;
				temp_result.gender=features[i].gender;
			}
                       
			if(features[i].hasRace)
			{
				temp_result.hasRace=true;
				temp_result.race=features[i].race;
			}
			
			if(features[i].hasAge)
			{
				temp_result.hasAge=true;
				temp_result.age=features[i].age;
			}

			if(features[i].hasJawShape)
			{
				temp_result.hasJawShape=true;
				temp_result.jawShape=features[i].jawShape;
			}
			if(features[i].hasCheekShape)
			{
				temp_result.hasCheekShape=true;
				temp_result.cheekShape=features[i].cheekShape;
			}
			if(features[i].hasFaceCode)
			{
				temp_result.hasFaceCode=true;
				std::vector<float> facecode;
				facecode.resize(static_cast<int>(features[i].faceCode.size()));
				for (auto j=0;j<features[i].faceCode.size();++j)
					facecode[j]=features[i].faceCode[j];
				temp_result.faceCode=facecode;
			}
		Result.push_back(temp_result);
	   }
		
	}
	 return Result; 


}



std::vector<FDFAResult> MTDetector::RunMeituFaceRects(const cv::Mat &ImageIn,std::vector<FDRect> Rects,const MTFaceConfig &FaceConfig)
{

	 std::vector<FDFAResult> Result;
     Result.clear();
	 cv::Mat cvrgba;

     cv::cvtColor(ImageIn, cvrgba, CV_BGR2RGBA);

	 mtface::MTImage image = mtface::MTImage::from_rgba(cvrgba.cols, cvrgba.rows, cvrgba.data);
    
     mtface::MTVector<mtface::MTFaceFeature> features;
     features.resize(Rects.size());
	 for(auto i =0;i<Rects.size();++i)
	 {
	 	mtface::MTFaceFeature TempFeature;
	 	TempFeature.faceBounds.x=Rects[i].x;
		TempFeature.faceBounds.y=Rects[i].y;
		TempFeature.faceBounds.width=Rects[i].width;
		TempFeature.faceBounds.height=Rects[i].height;
		TempFeature.hasFaceBounds=true;
		features[i]=TempFeature;

	 }

	mtFA.Detect(image,features);		   
    
	if(FaceConfig.MeituFT)
	{
         
         mtFT.Detect(image,features);
	}

	if(FaceConfig.MeituAge)
	{

         mtAge.Detect(image,features);
	}

	if(FaceConfig.MeituRace)
	{

         mtRace.Detect(image,features);
	}

	if(FaceConfig.MeituGender)
	{

         mtGender.Detect(image,features);
	}

	if(FaceConfig.MeituFacial)
	{
	   mtJaw.Detect(image, features);
       mtCheek.Detect(image, features);
	}
	if(FaceConfig.MeituFR)
	{

         mtFR.Detect(image,features);
	}


	if (features.size()>0)
    {
		
   		mtface::WarpConfig config;
   		config.padding_left = FaceConfig.EXTERN_LEFT;
    	config.padding_right = FaceConfig.EXTERN_RIGHT;
    	config.padding_bottom =FaceConfig.EXTERN_BOTTOM;
    	config.padding_top =FaceConfig.EXTERN_UP;
		
	 
        
		// save crop image
       for(auto i=0;i<features.size();i++)
	   {
		    FDFAResult temp_result;
			//float iM[6];
		    temp_result.mFaceRect.x=features[i].faceBounds.x;
			temp_result.mFaceRect.y=features[i].faceBounds.y;
			temp_result.mFaceRect.width=features[i].faceBounds.width;
			temp_result.mFaceRect.height=features[i].faceBounds.height;
			if(FaceConfig.MeituCrop)
			{
				int crop_h,crop_w;
			if((FaceConfig.CROP_SIZE_H>0)&&(FaceConfig.CROP_SIZE_W>0))
				{
						crop_h=FaceConfig.CROP_SIZE_H;
						crop_w=FaceConfig.CROP_SIZE_W;
				}
				else
				{
					crop_h=features[i].faceBounds.height*2.0f;
						crop_w=features[i].faceBounds.width*2.0f;
				}
				temp_result.hasCrop=true;
				MTImage face;
				float M[6];
				float iM[6];
				mtface::MTVector<mtface::MTPoint2f> Warpfacepoints;

				mtface::CalcWarpTransform(M,mtface::MTSize2i(crop_w, crop_h),features[i].facePoints, config);
				mtface::WarpAffine(image, face,mtface::MTSize2i(crop_w, crop_h), M);
				//mtface::InvertAffineTransform(M,iM);
				TransformPoints(features[i].facePoints,Warpfacepoints,M);

				//mtface::WarpAffine(image,face, mtface::MTSize2i(100, 100), M);
				InvertAffineTransform(M, iM);
				//mtface::WarpAffine(image,face, mtface::MTSize2i(100, 100), M);
				temp_result.WarpMatrix.resize(6);
				temp_result.InversWarpMatrix.resize(6);
				for(int j=0;j<6;++j)
				{
					temp_result.WarpMatrix[j]=M[j];
					temp_result.InversWarpMatrix[j]=iM[j];
				}
				cv::Mat CropImage(face.height,face.width,CV_8UC4, face.data);
				cv::Mat bgr;
				cv::cvtColor(CropImage, bgr, CV_RGBA2BGR);

				temp_result.mCropImage=bgr;
				
				

				

				auto inner_points=Warpfacepoints.size();
				std::vector<float> point_x(inner_points);//
				std::vector<float> point_y(inner_points);
				
				
				for(int k = 0; k < inner_points; k++)
				{
					point_x[k] = Warpfacepoints[k].x;
					point_y[k] = Warpfacepoints[k].y;
					// std::cout<<point_x[k]<<std::endl;
				}
            // 根据前26个点（内五官点），最大最小值 包出一个框
				std::vector<float>::iterator biggest_x=std::max_element(std::begin(point_x),std::end(point_x));
				std::vector<float>::iterator biggest_y=std::max_element(std::begin(point_y),std::end(point_y));
				std::vector<float>::iterator smallest_x=std::min_element(std::begin(point_x),std::end(point_x));
				std::vector<float>::iterator smallest_y=std::min_element(std::begin(point_y),std::end(point_y));
				float max_x=(*biggest_x);
				float max_y=(*biggest_y);
				float min_x=(*smallest_x);
				float min_y=(*smallest_y);

		
				temp_result.mCropFaceRect.x=static_cast<int>(min_x);
				temp_result.mCropFaceRect.y=static_cast<int>(min_y);
				temp_result.mCropFaceRect.width=static_cast<int>(max_x-min_x);
				temp_result.mCropFaceRect.height=static_cast<int>(max_y-min_y);


				temp_result.mCropFacePoints.resize(Warpfacepoints.size());
				for(auto j=0;j<Warpfacepoints.size();++j)
				{
						temp_result.mCropFacePoints[j]=cv::Point2f(Warpfacepoints[j].x,Warpfacepoints[j].y);
				}

			}
			
			temp_result.mFacePoints.resize(features[i].facePoints.size());
			for(auto j=0;j<features[i].facePoints.size();++j)
			{
					temp_result.mFacePoints[j]=cv::Point2f(features[i].facePoints[j].x,features[i].facePoints[j].y);
			}

			if(features[i].hasRollAngle)
			{
				temp_result.hasRollAngle=true;
				temp_result.rollAngle=features[i].rollAngle;
			}
		

			if(features[i].hasTrackingId)
			{
				temp_result.hasTrackingId=true;
				temp_result.trackingId=features[i].trackingId;
			}
               
			if(features[i].hasGender)
			{
				temp_result.hasGender=true;
				temp_result.gender=features[i].gender;
			}
                       
			if(features[i].hasRace)
			{
				temp_result.hasRace=true;
				temp_result.race=features[i].race;
			}
			
			if(features[i].hasAge)
			{
				temp_result.hasAge=true;
				temp_result.age=features[i].age;
			}

			if(features[i].hasJawShape)
			{
				temp_result.hasJawShape=true;
				temp_result.jawShape=features[i].jawShape;
			}
			if(features[i].hasCheekShape)
			{
				temp_result.hasCheekShape=true;
				temp_result.cheekShape=features[i].cheekShape;
			}
			if(features[i].hasFaceCode)
			{
				temp_result.hasFaceCode=true;
				std::vector<float> facecode;
				facecode.resize(static_cast<int>(features[i].faceCode.size()));
				for (auto j=0;j<features[i].faceCode.size();++j)
					facecode[j]=features[i].faceCode[j];
				temp_result.faceCode=facecode;
			}
		Result.push_back(temp_result);
	   }
		
	}
	 return Result; 

}


std::vector<FDFAResult> MTDetector::RunMeituFacePoints(const cv::Mat &ImageIn,std::vector<FDPoints> Points,const MTFaceConfig &FaceConfig)
{
	 std::vector<FDFAResult> Result;
     Result.clear();
	 cv::Mat cvrgba;

     cv::cvtColor(ImageIn, cvrgba, CV_BGR2RGBA);

	 mtface::MTImage image = mtface::MTImage::from_rgba(cvrgba.cols, cvrgba.rows, cvrgba.data);
    
     mtface::MTVector<mtface::MTFaceFeature> features;


     features.resize(Points.size());

	 for(auto i =0;i<Points.size();++i)
	 {

	 	mtface::MTFaceFeature TempFeature;
	 	std::vector<cv::Point2f> TempPoints=Points[i].Points;
        TempFeature.hasFacePoints=true;

	    TempFeature.facePoints.resize(TempPoints.size());
	    for(auto j=0;j<TempPoints.size();++j)
		{
			TempFeature.facePoints[j].x=TempPoints[j].x;
			TempFeature.facePoints[j].y=TempPoints[j].y;
		}   


		mtface::MTRect_<float>  bbx;
        auto perface_point_num=TempFeature.facePoints.size();
        int point_start = 0;
        int point_end = static_cast<int>(perface_point_num);
        if (perface_point_num==17)
        {
            point_start = 0;
            point_end = 17;
        }
        else if (perface_point_num == 33)
        {
            point_start = 0;
            point_end = 26;
        }
        else if (perface_point_num == 45)
        {
            point_start = 0;
            point_end = 38;
        }
        else if (perface_point_num == 106)
        {
            point_start = 33;
            point_end = 106;
        }
        else if (perface_point_num == 118)
        {
            point_start = 33;
            point_end = 118;
        }
        else
        {
            LOGE("the fa points have change the number,and the inner points should be change");
        }
        float minx =TempFeature.facePoints[0].x, miny = TempFeature.facePoints[0].y, maxx = TempFeature.facePoints[0].x, maxy = TempFeature.facePoints[0].y;
        for (int i = point_start; i < point_end; i++)
        {
            if (TempFeature.facePoints[i].x<minx) minx = TempFeature.facePoints[i].x;
            if (TempFeature.facePoints[i].y<miny) miny = TempFeature.facePoints[i].y;
            if (TempFeature.facePoints[i].x>maxx) maxx = TempFeature.facePoints[i].x;
            if (TempFeature.facePoints[i].y>maxy) maxy = TempFeature.facePoints[i].y;
        }

        float height = maxy - miny;
        float width = maxx - minx;
        float side=std::max(height,width);
        TempFeature.faceBounds.x=(minx+maxx)/2 - side/2;
        TempFeature.faceBounds.y =  (miny+maxy)/2 - side/2;
        TempFeature.faceBounds.height = side;
        TempFeature.faceBounds.width = side;

		TempFeature.hasFaceBounds=true;
		features[i]=TempFeature;
     }
	 

	bool needFa=false;
    for(auto i =0;i<Points.size();++i)
    {
    	std::vector<cv::Point2f> TempPoints=Points[i].Points;
    	if (TempPoints.size()<118)
    		{
    			needFa=true;
    		    break;
    		}

    }

	if(needFa)
	{
		mtFA.Detect(image,features);		   
	}

	if(FaceConfig.MeituFT)
	{
         
         mtFT.Detect(image,features);
	}

	if(FaceConfig.MeituAge)
	{

         mtAge.Detect(image,features);
	}

	if(FaceConfig.MeituRace)
	{

         mtRace.Detect(image,features);
	}

	if(FaceConfig.MeituGender)
	{

         mtGender.Detect(image,features);
	}

	if(FaceConfig.MeituFacial)
	{
	   mtJaw.Detect(image, features);
       mtCheek.Detect(image, features);
	}
	if(FaceConfig.MeituFR)
	{

         mtFR.Detect(image,features);
	}


	if (features.size()>0)
    {
		
   		mtface::WarpConfig config;
   		config.padding_left = FaceConfig.EXTERN_LEFT;
    	config.padding_right = FaceConfig.EXTERN_RIGHT;
    	config.padding_bottom =FaceConfig.EXTERN_BOTTOM;
    	config.padding_top =FaceConfig.EXTERN_UP;
		
	
        
		// save crop image
       for(auto i=0;i<features.size();i++)
	   {
		    FDFAResult temp_result;
			//float iM[6];
		    temp_result.mFaceRect.x=features[i].faceBounds.x;
			temp_result.mFaceRect.y=features[i].faceBounds.y;
			temp_result.mFaceRect.width=features[i].faceBounds.width;
			temp_result.mFaceRect.height=features[i].faceBounds.height;
			if(FaceConfig.MeituCrop)
			{	
				int crop_h,crop_w;
			  if((FaceConfig.CROP_SIZE_H>0)&&(FaceConfig.CROP_SIZE_W>0))
				{
						crop_h=FaceConfig.CROP_SIZE_H;
						crop_w=FaceConfig.CROP_SIZE_W;
				}
				else
				{
					crop_h=features[i].faceBounds.height*2.0f;
						crop_w=features[i].faceBounds.width*2.0f;
				}
				temp_result.hasCrop=true;
				MTImage face;
				float M[6];
				float iM[6];
				mtface::MTVector<mtface::MTPoint2f> Warpfacepoints;

				mtface::CalcWarpTransform(M,mtface::MTSize2i(crop_w, crop_h),features[i].facePoints, config);
				mtface::WarpAffine(image, face,mtface::MTSize2i(crop_w, crop_h), M);
				//mtface::InvertAffineTransform(M,iM);
				TransformPoints(features[i].facePoints,Warpfacepoints,M);
				InvertAffineTransform(M, iM);
				//mtface::WarpAffine(image,face, mtface::MTSize2i(100, 100), M);
				temp_result.WarpMatrix.resize(6);
				temp_result.InversWarpMatrix.resize(6);
				for(int j=0;j<6;++j)
				{
					temp_result.WarpMatrix[j]=M[j];
					temp_result.InversWarpMatrix[j]=iM[j];
				}

				cv::Mat CropImage(face.height,face.width,CV_8UC4, face.data);
				cv::Mat bgr;
				cv::cvtColor(CropImage, bgr, CV_RGBA2BGR);

				temp_result.mCropImage=bgr;
				
				

				

				auto inner_points=Warpfacepoints.size();
				std::vector<float> point_x(inner_points);//
				std::vector<float> point_y(inner_points);
				
				
				for(int k = 0; k < inner_points; k++)
				{
					point_x[k] = Warpfacepoints[k].x;
					point_y[k] = Warpfacepoints[k].y;
					// std::cout<<point_x[k]<<std::endl;
				}
            // 根据前26个点（内五官点），最大最小值 包出一个框
				std::vector<float>::iterator biggest_x=std::max_element(std::begin(point_x),std::end(point_x));
				std::vector<float>::iterator biggest_y=std::max_element(std::begin(point_y),std::end(point_y));
				std::vector<float>::iterator smallest_x=std::min_element(std::begin(point_x),std::end(point_x));
				std::vector<float>::iterator smallest_y=std::min_element(std::begin(point_y),std::end(point_y));
				float max_x=(*biggest_x);
				float max_y=(*biggest_y);
				float min_x=(*smallest_x);
				float min_y=(*smallest_y);

		
				temp_result.mCropFaceRect.x=static_cast<int>(min_x);
				temp_result.mCropFaceRect.y=static_cast<int>(min_y);
				temp_result.mCropFaceRect.width=static_cast<int>(max_x-min_x);
				temp_result.mCropFaceRect.height=static_cast<int>(max_y-min_y);


				temp_result.mCropFacePoints.resize(Warpfacepoints.size());
				for(auto j=0;j<Warpfacepoints.size();++j)
				{
						temp_result.mCropFacePoints[j]=cv::Point2f(Warpfacepoints[j].x,Warpfacepoints[j].y);
				}

			}
			
			temp_result.mFacePoints.resize(features[i].facePoints.size());
			for(auto j=0;j<features[i].facePoints.size();++j)
			{
					temp_result.mFacePoints[j]=cv::Point2f(features[i].facePoints[j].x,features[i].facePoints[j].y);
			}

			if(features[i].hasRollAngle)
			{
				temp_result.hasRollAngle=true;
				temp_result.rollAngle=features[i].rollAngle;
			}
		

			if(features[i].hasTrackingId)
			{
				temp_result.hasTrackingId=true;
				temp_result.trackingId=features[i].trackingId;
			}
               
			if(features[i].hasGender)
			{
				temp_result.hasGender=true;
				temp_result.gender=features[i].gender;
			}
                       
			if(features[i].hasRace)
			{
				temp_result.hasRace=true;
				temp_result.race=features[i].race;
			}
			
			if(features[i].hasAge)
			{
				temp_result.hasAge=true;
				temp_result.age=features[i].age;
			}

			if(features[i].hasJawShape)
			{
				temp_result.hasJawShape=true;
				temp_result.jawShape=features[i].jawShape;
			}
			if(features[i].hasCheekShape)
			{
				temp_result.hasCheekShape=true;
				temp_result.cheekShape=features[i].cheekShape;
			}
			if(features[i].hasFaceCode)
			{
				temp_result.hasFaceCode=true;
				std::vector<float> facecode;
				facecode.resize(static_cast<int>(features[i].faceCode.size()));
				for (auto j=0;j<features[i].faceCode.size();++j)
					facecode[j]=features[i].faceCode[j];
				temp_result.faceCode=facecode;
			}
		Result.push_back(temp_result);
	   }
		
	}
	 return Result; 

}




/*
*  input: fr features
*	output :add cluster id into the features
*
*/

int  MTDetector::RunCluster(std::vector<FDFAResult> &FDFAFeatures)
{  
   
    mtface::MTVector<mtface::MTFaceFeature> features;
	if(FDFAFeatures.size()<1)
		return -1;
	features.resize(FDFAFeatures.size());
	for (auto i =0;i<FDFAFeatures.size();++i)
		{
				
				if(FDFAFeatures[i].hasFaceCode)
					{
						features[i].hasFaceCode=true;
						features[i].faceCode.resize(FDFAFeatures[i].faceCode.size());
						for (auto j=0;j<FDFAFeatures[i].faceCode.size();++j)
						   features[i].faceCode[j]=FDFAFeatures[i].faceCode[j];
					}
				if(FDFAFeatures[i].hasClusterID)
				{
					features[i].hasClusterID=true;
					features[i].clusterID=FDFAFeatures[i].clusterID;

				}

		}
	//std::cout<<	features.size()<<std::endl;
	FRClusterer frcluster;
    frcluster.Cluster(features);
    
	for (auto i =0;i<features.size();++i)
		{
				if(features[i].hasClusterID)
					{
						//std::cout<<	FDFAFeatures[i].clusterID<<std::endl;
						//std::cout<<	features[i].clusterID<<std::endl;
						FDFAFeatures[i].hasClusterID=true;
						FDFAFeatures[i].clusterID=features[i].clusterID;
					
					}

		}
  
	return 0;
}





}
