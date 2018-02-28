#
/* MTLabFace.i */
 %module MTLabFace
 %{
 /* Put header files here or function declarations like below */
 #include "src/MTDetector.h"
 %}

/* add arrary interface  */
%include "carrays.i"
%array_class(float, floatArray);
%array_class(int,intArray)

#define EXTERN_NUM 4


%include "../src/MTDetector.h"

/* add opencv class interface  */
%include "opencv.i"
%cv_point_instantiate_defaults
%cv_mat__instantiate_defaults

namespace std {
   %template(vectorp) vector<cv::Point2f>;
   %template(vectorfdfa) vector<mtface::FDFAResult>;
   %template(vectorRect) vector<mtface::FDRect>;
    %template(vectorPoints) vector<mtface::FDPoints>;
	  };

%extend mtface::FDImage
{
   FDImage(int cols, int rows, int chan , ptrdiff_t data)
    {
        return new mtface::FDImage(cols, rows, chan, (void*) data);
    }
   
};

%include typemaps.i 





%pythoncode %{
import numpy as np
Detector=MTDetector()

def FaceFeatureDetect(image=None,cfg=None,Rects=None,Points=None):
	if image is None:
		return
	new_FDImage=Mat.from_array(image)
	if cfg is None:
		config=MTFaceConfig()
	else:
		config=cfg
	
	if(Points is not None):
		Pp=vectorPoints(len(Points))
		for i in range(len(Points)):
			Pp[i].Points.resize(len(Points[i]))
			for j in range(len(Points[i])):
				Pp[i].Points[j].x=Points[i][j][0]
				Pp[i].Points[j].y=Points[i][j][1]

		c= Detector.RunMeituFacePoints(new_FDImage,Pp,config)

	elif(Rects is not None):
		R=vectorRect(len(Rects))
		for i in range(len(Rects)):
			R[i].x=Rects[i][0]
			R[i].y=Rects[i][1]
			R[i].width=Rects[i][2]
			R[i].height=Rects[i][3]

		c= Detector.RunMeituFaceRects(new_FDImage,R,config)
	else:
		c=Detector.RunMeituFace(new_FDImage,config)

	
    

	FdFaResult=list()
	for i in range(c.size()):		
		face=c[i]
		faceinfo={}
		faceinfo["Rect"]=[np.float32(face.mFaceRect.x),np.float32(face.mFaceRect.y),np.float32(face.mFaceRect.width),np.float32(face.mFaceRect.height)]
		faceinfo["Points"]=[[np.float32(p.x),np.float32(p.y)] for p in face.mFacePoints]
		if(face.hasCrop):
			faceinfo["CropImage"]=np.array(face.mCropImage,copy=True)
			faceinfo["CropRect"]=[np.float32(face.mCropFaceRect.x),np.float32(face.mCropFaceRect.y),np.float32(face.mCropFaceRect.width),np.float32(face.mCropFaceRect.height)]
			faceinfo["CropPoints"]=[[np.float32(p.x),np.float32(p.y)] for p in face.mCropFacePoints]
			faceinfo["WarpM"]=np.array([ np.float32(p) for p in face.WarpMatrix]).reshape(2,3)
			faceinfo["InvWarpM"]=np.array([ np.float32(p) for p in face.InversWarpMatrix]).reshape(2,3)
        
		if(face.hasTrackingId):
			faceinfo["TrackingId"]=face.trackingId
		if(face.hasRollAngle):
			faceinfo["RollAngle"]=face.rollAngle
		if(face.hasRace):
			faceinfo["Race"]=face.race
		if(face.hasAge):
			faceinfo["Age"]=face.age
		if(face.hasGender):
			faceinfo["Gender"]=face.gender
		if(face.hasFaceCode):
			facecode=list()
			for j in range(face.faceCode.size()):
				facecode.append(np.float32(face.faceCode[j]))
			faceinfo["FaceCode"]=facecode
		if(face.hasClusterID):
			faceinfo["ClusterID"]=face.clusterID
		if(face.hasCheekShape):
			faceinfo["CheekShape"]=face.cheekShape
		if(face.hasJawShape):
			faceinfo["JawShape"]=face.jawShape
		FdFaResult.append(faceinfo)
	return FdFaResult


def Cluster(FaceCodeList=None,FaceIdList=None):
	if FaceCodeList is None:
		return
	
	Feature=vectorfdfa(len(FaceCodeList))
	faceCodeLen=len(FaceCodeList[0])
	for i in range(len(FaceCodeList)):
		Feature[i].hasFaceCode=True
		Feature[i].faceCode.resize(faceCodeLen)
		for j in range(faceCodeLen):
			Feature[i].faceCode[j]=np.float64(FaceCodeList[i][j])

	if FaceIdList is not None:
		assert (len(FaceCodeList)==len(FaceIdList))
		for i in range(len(FaceCodeList)):
			if(FaceIdList[i]>=0):
				Feature[i].hasClusterID=True
				Feature[i].clusterID=FaceIdList[i]
			else:
				Feature[i].hasClusterID=False
				Feature[i].clusterID=-1


	ClusterList=list()
	Detector.RunCluster(Feature)
	for i in range(len(FaceCodeList)):
		if(Feature[i].hasClusterID):
			ClusterList.append(Feature[i].clusterID) 

	return ClusterList

%}
