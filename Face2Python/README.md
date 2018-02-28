
# ubuntu


## update 2018-2-05
update  crop_size param
old crop: CROP_SIZE=128
new crop : CROP_SIZE_H=128  CROP_SIZE_W=128



## update 2018-1-17
old crop param:
`
config.EXTERN_RATE[0]=0.2  #left
config.EXTERN_RATE[1]=0.2   #right
config.EXTERN_RATE[2]=0.1   #bottom
config.EXTERN_RATE[3]=0.3   #up
`
new crop param
`
config.EXTERN_LEFF=0.2  #left
config.EXTERN_RIGHT=0.2   #right
config.EXTERN_BOTTOM=0.1   #bottom
config.EXTERN_UP=0.3   #up
`
## update 2018-1-04
* change the precision of return results from float64 to float32.

## update 2017-12-28
* Set the default  `CROP_SIZE = max(inner_rect_size)*2`  
* Add two items into  the return results : ` ["WarpM"] 	, ["InvWarpM"] `. For map the warped_face to the origin image position.
* Remove the `/3rdparty/Mat`

```
import cv2

config=MTLabFace.MTFaceConfig()
config.MeituCrop=True

config.EXTERN_RATE[0]=0.2  #left
config.EXTERN_RATE[1]=0.2   #right
config.EXTERN_RATE[2]=0.1   #bottom
config.EXTERN_RATE[3]=0.3   #up
image=cv2.imread("")
result=MTLabFace.FaceFeatureDetect(image,config)
    if result is not None:
        for i in range(len(result)):
		crop_img=result[i]["CropImage"]

           	 WarpM=result[i]["WarpM"]
            	InvWarpM=result[i]["InvWarpM"]
            	img_crop = cv2.warpAffine(crop_img, InvWarpM, (int(image.shape[1]), int(image.shape[0])))
	
```

## run test_Demo


* `cd demo`
* `source prov_evn `
* `python test.py `



## using MTLabFace in project

* Copy the  **libMTLabFace.so, MTLabFace.py , _MTLabFace.so** from `/build/lib/ubuntu/`   into your project dir

* Add the libs dir(where MTLabFace.so.. located) into the **LD_LIBRARY_PATH** 

* import the MTLabFace in your .py file
`
    import MTLabFace
`


## FD 

```
import MTLabFace
import cv2


image=cv2.imread('./test.jpg')

result=MTLabFace.FaceFeatureDetect(image)

for i in range(len(result)):
    face=result[i]
    Rect=face["Rect"]         # [x,y,w,h]   (x,y)is the left_up point of the rect ;w,h is the width and the height respectively
    Points=face["Points"]     # fd will return 17 points

```


## FD and FA


```
import MTLabFace
import cv2


image=cv2.imread('./test.jpg')

config=MTLabFace.MTFaceConfig()
config.MeituFA=True

result=MTLabFace.FaceFeatureDetect(image,config)

for i in range(len(result)):
    face=result[i]
    Rect=face["Rect"]
    Points=face["Points"]   # fa will return 118 points 
```


## FD and CropFace

```
import MTLabFace
import cv2


image=cv2.imread('./test.jpg')

config=MTLabFace.MTFaceConfig()
config.MeituCrop=True; #Crop the face
config. EXTERN_RATE[0]=0.5   #left pad
config. EXTERN_RATE[1]=0.5  # right pad
config. EXTERN_RATE[2]=0.5  #bottom pad
config. EXTERN_RATE[3]=0.5  #up pad 


result=MTLabFace.FaceFeatureDetect(image,config)

for i in range(len(result)):
    face=result[i]
    Rect=face["Rect"]
    Points=face["Points"]   # fa will return 118 points 

    CropImage=face["CropImage"]   # crop the warped face
    CropRect=face["CropRect"]   #  rect in the croped face
    CropPoints=face["CropPoints"]  #  points in the croped face
```


## Gender (Race,AGE)


```
import MTLabFace
import cv2


image=cv2.imread('./test.jpg')

config=MTLabFace.MTFaceConfig()
config.MeituFA=True     #  
config.MeituGender=True

result=MTLabFace.FaceFeatureDetect(image,config)

for i in range(len(result)):
    face=result[i]
    Rect=face["Rect"]
    Points=face["Points"]  
    Gender=face["Gender"] 




```



## FR  and  Cluster demo

with no prior info
```
config=MTLabFace.MTFaceConfig()
config.MeituFR=True

facecodeList=list()
for i in range(3):
    result=MTLabFace.FaceFeatureDetect(image,config)
    for j in range(len(result)):
        facecodeList.append(result[j]["FaceCode"])

Indlist=MTLabFace.Cluster(facecodeList)


```


with prior info
```

Indlist1=MTLabFace.Cluster(facecodeList,Indlist)

```



# Have rects as input params

Will Detect fa and other features
```
import MTLabFace
import cv2


image=cv2.imread('./test.jpg')

config=MTLabFace.MTFaceConfig()
config.MeituCrop=True; #Crop the face
config. EXTERN_RATE[0]=0.5   #left pad
config. EXTERN_RATE[1]=0.5  # right pad
config. EXTERN_RATE[2]=0.5  #bottom pad
config. EXTERN_RATE[3]=0.5  #up pad 

result=MTLabFace.FaceFeatureDetect(image,config,Rects=rects)

```

# Have pointss as input params

detect other features
```
import MTLabFace
import cv2


image=cv2.imread('./test.jpg')

config=MTLabFace.MTFaceConfig()
config.MeituCrop=True; #Crop the face
config. EXTERN_RATE[0]=0.5   #left pad
config. EXTERN_RATE[1]=0.5  # right pad
config. EXTERN_RATE[2]=0.5  #bottom pad
config. EXTERN_RATE[3]=0.5  #up pad 

result=MTLabFace.FaceFeatureDetect(image,config,Points=Points)

```

# Other parameters 



set config :
```
config info:
     bool MeituFD;   [default=true]
     bool MeituFA;    [default=false]
    bool MeituFT;		[default=false]
    bool MeituAge;		[default=false]
    bool MeituRace;		[default=false]
    bool MeituGender;		[default=false]
    bool MeituCrop;    [default=false]

    bool MeituFR;  [default=false]
    bool MeituFacial;  [default=false]

    int CROP_SIZE;   [default=100]
    int FD_MINSIZE;    [defullt=256]
    float RPN_SCORE;     [default=0.6]
    std::vector<float> EXTERN_RATE;   [default: 0.2,0.2,0.2,0.2  ]  
  
    float FA_SCORE;    [default=0.01]
    int  FA_MAX_NUM;    [default=20]
```






return result ：
```
	"Rect"      # face rect  in origin image
    "Points"        # face points in origin image 
    "CropImage"     # warped face
	"WarpM"     # transform matrix 
	 "InvWarpM"  #inverse  matrix
    "CropRect"
    "CropPoints"
    "TrackingId"
    "RollAngle"   # face orientation
    "Race"         
    "Age"     
    "Gender"
    "FaceCode"     # FR features
    "ClusterID"   
    "CheekShape"
    "JawShape"   
```







# Building  MTLabFace.so

## ubuntu 

[Document](http://www.swig.org/Doc1.3/Python.html#Python_nn36)

### install swig

`
sudo apt-get install swig 
`
Or `conda install swig`

Or you can  download source codes from [SWIG](http://www.swig.org/),and install it according to the tutorial.


### compile

* cd  to the` build/`

* Set  **PYTHON_PATH**  in the prov_evn,and `source prov_evn`

* run `sh ubuntu_build.sh` you will get _MTLabFace.so  and MTLabFace.py



### testing

```
cp  build/lib/ubuntu/*  demo/
source prov_evn
python test.py

```







