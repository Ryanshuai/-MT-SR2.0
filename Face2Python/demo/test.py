


#import caffe
import sys
import cv2
import time
import numpy as np

#libdir='/root/cpp2python/'
#sys.path.insert(0,libdir)


import MTLabFace

image=cv2.imread('/home/mt08/test4.jpg')



config=MTLabFace.MTFaceConfig()
config.MeituCrop=True
#config.MeituFA=True
config.EXTERN_LEFT=0.25
config.EXTERN_RIGHT=0.25
config.EXTERN_BOTTOM=-0.4
config.EXTERN_UP=0.2
# config.EXTERN_RATE[0]=0.25
# config.EXTERN_RATE[1]=0.25
# config.EXTERN_RATE[2]=0.3
# config.EXTERN_RATE[3]=0.0
config.CROP_SIZE_H=80
config.CROP_SIZE_W=150


cv2.imshow('t1',image)
cv2.waitKey()

facecodeList=list()
for i in range(1):
    fd_start=time.time()
    result=MTLabFace.FaceFeatureDetect(image,config)
    for j in range(len(result)):
        point_o = result[j]["Points"]
        if config.MeituCrop:
            point_W = result[j]["CropPoints"]
            rect=result[j]["Rect"]
            max_side=max(rect[2],rect[3])
            new_rect=[(rect[0]+rect[2]/2)-max_side,((rect[1]+rect[3]*0.3)-max_side),max_side*2,max_side*2]

            #crop_image=image(new_rect[0])
            crop_img = result[j]["CropImage"]
            cv2.rectangle(image, (int(new_rect[0]), int(new_rect[1])),
                          (int(new_rect[2] + new_rect[0]), int(new_rect[3] + new_rect[1])),
                          (0, 0, 255), 2)
            cv2.imshow('t',crop_img)
            cv2.waitKey()
        if(config.MeituFR):
            facecodeList.append(result[j]["FaceCode"])
        orin_rect = result[j]["Rect"]

        cv2.rectangle(image, (int(orin_rect[0]), int(orin_rect[1])),
                      (int(orin_rect[2] + orin_rect[0]), int(orin_rect[3] + orin_rect[1])),
                      (0, 0, 255), 2)
    fd_end=time.time()
    print('fd time :{0}'.format(fd_end-fd_start))
    cv2.imshow('t',image)
    cv2.waitKey()






if (config.MeituFR):
    Indlist = [2, -1, -1, 1, -1, -1]

    Indlist1 = MTLabFace.Cluster(facecodeList, Indlist)

    face_rect = []
    Points = []
    if result is not None:
        for i in range(len(result)):
            print(i)
            face_rect.append(result[i]["Rect"])
            Points.append(result[i]["Points"])
            if "CropImage" in result[i].keys():
                face_Crop = result[i]["CropImage"]
                face_croprect = result[i]["CropRect"]
                cv2.rectangle(face_Crop, (int(face_croprect[0]), int(face_croprect[1])),
                              (int(face_croprect[2] + face_croprect[0]), int(face_croprect[3] + face_croprect[1])),
                              (255, 0, 0), 2)
                cv2.imshow('t', face_Crop)
                cv2.waitKey()
            orin_rect = result[i]["Rect"]

            cv2.rectangle(image, (int(orin_rect[0]), int(orin_rect[1])),
                          (int(orin_rect[2] + orin_rect[0]), int(orin_rect[3] + orin_rect[1])),
                          (0, 0, 255), 2)
        cv2.imshow('to', image)
        cv2.waitKey()

    result1=MTLabFace.FaceFeatureDetect(image,config,Points=Points)

    if result1 is not None:
        for i in range(len(result1)):
            print(i)
            face_rect.append(result1[i]["Rect"])
            if "CropImage" in result1[i].keys():
                face_Crop = result1[i]["CropImage"]
                face_croprect = result1[i]["CropRect"]
                cv2.rectangle(face_Crop, (int(face_croprect[0]), int(face_croprect[1])),
                              (int(face_croprect[2] + face_croprect[0]), int(face_croprect[3] + face_croprect[1])),
                              (255, 0, 0), 2)
                cv2.imshow('t', face_Crop)
                cv2.waitKey()
            orin_rect = result[i]["Rect"]

            cv2.rectangle(image, (int(orin_rect[0]), int(orin_rect[1])),
                          (int(orin_rect[2] + orin_rect[0]), int(orin_rect[3] + orin_rect[1])),
                          (0, 0, 255), 2)
        cv2.imshow('to', image)
        cv2.waitKey()

print('finish')
