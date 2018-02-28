
# !/usr/bin/env python

import cv2
import numpy as np

import MTLabFace
import math




config=MTLabFace.MTFaceConfig()
config.MeituFA=True
config.MeituCrop=True
config.FD_FASCORE=1.0
config.CROP_SIZE=256
#config.MeituFacial=True
#config.MeituFR=True
config.EXTERN_LEFT=0.2
config.EXTERN_RIGHT=0.2
config.EXTERN_BOTTOM=0.1
config.EXTERN_UP=0.3


# Checks if a matrix is a valid rotation matrix.
def isRotationMatrix(R):
    Rt = np.transpose(R)
    shouldBeIdentity = np.dot(Rt, R)
    I = np.identity(3, dtype=R.dtype)
    n = np.linalg.norm(I - shouldBeIdentity)
    return n < 1e-6


def rotationMatrixToEulerAngles(R):
    # assert (isRotationMatrix(R))
    #
    sy = math.sqrt(R[2, 1] * R[2, 1] + R[2, 2] * R[2, 2])
    #
    # singular = sy < 1e-6

    #if not singular:
    x = math.atan2(R[2, 1], R[2, 2])
    y = math.atan2(-R[2, 0], sy)
    z = math.atan2(R[1, 0], R[0, 0])

    x *=180.0/3.14159265
    y *= 180.0 / 3.14159265
    z *= 180.0 / 3.14159265


    return np.array([x, y, z])


# Read Image
image=cv2.imread('/home/mt08/10.jpeg')
size = image.shape

result=MTLabFace.FaceFeatureDetect(image,config)



for j in range(len(result)):
    point_o = result[j]["Points"]
    image_points = np.array(

    [point_o[33],    # Left Mouth corner
    point_o[37],
    point_o[51],
    point_o[55 ],


   # point_o[63],
    point_o[46 ],
   # point_o[67 ],
    point_o[42 ],

    point_o[65],
    point_o[61 ],

    point_o[77 ],
    point_o[80 ],

    point_o[83 ],
  #  point_o[89 ],
    point_o[74 ],
   # point_o[95 ],


     point_o[0 ],
     point_o[5 ],

    point_o[27 ],
    point_o[32] ]

, dtype="double")




# image_points = np.array([
#     (359, 391),     # Nose tip
#     (399, 561),     # Chin
#     (337, 297),     # Left eye left corner
#     (513, 301),     # Right eye right corne
#     (345, 465),     # Left Mouth corner
#     (453, 469)      # Right mouth corner
# ], dtype="double")

    poinsts_all = np.array([

        (-62.991, -59.881, 16.357),
        (-44.598, -66.440, -2.466),
        (-9.505, -53.395, -5.619),
        (-43.153, -58.929, 0.509),

        (-52.239, -45.040, 15.425),  # 4
        (-33.906, -48.283, 6.463),
        (-20.721, -39.754, 12.819),
        (-35.727, -37.322, 7.518),
        (-41.155, -44.806, 16.798),

        (62.378, -59.881, 16.357),

        (43.985, -66.440, -2.466),
        (8.891, -53.395, -5.619),
        (42.540, -58.929, 0.509),

        (51.626, -45.040, 15.425),  # 13
        (33.293, -48.283, 6.463),
        (20.108, -39.754, 12.819),
        (35.113, -37.322, 7.518),
        (40.542, -44.806, 16.798),

        (-19.060, 0.399, -4.938),  # 18
        (-0.307, 11.581, -11.801),
        (18.446, 0.399, -4.938),
        (-0.307, -9.122, -25.421),

        (-28.040, 32.593, 11.042),  # 22
        (-0.307, 26.717, -12.994),
        (27.427, 32.593, 11.042),
        (-0.307, 47.513, -6.076),

        (-78.149, -55.432, 56.154),  # 26
        (-71.851, 15.568, 54.652),
        (-58.259, 38.488, 36.910),
        (-0.307, 75.399, 2.238),
        (57.646, 38.488, 36.910),
        (71.237, 15.568, 54.652),
        (77.535, -55.432, 56.154),

        (-18.573, 26.408, -5.221),  # 33
        (-8.858, 23.916, -11.577),
        (8.245, 23.916, -11.577),
        (17.959, 26.408, -5.221),

        (9.588, 46.857, -4.308),  # 37
        (-10.201, 46.857, -4.308),

        (-15.919, 33.269, 0.893),  # 39
        (-0.307, 34.783, -3.994),
        (15.306, 33.269, 0.893),
        (-15.919, 33.269, 0.893),
        (-0.307, 34.783, -3.994),
        (15.306, 33.269, 0.893),

        (-15.178, -49.704, 2.448),  # 45
        (14.565, -49.704, 2.448),
        (-75.954, -13.403, 63.895),
        (-67.21, 26.596, 46.09),
        (-34.49, 61.691, 19.238),
        (34.49, 61.691, 19.238),
        (67.21, 26.596, 46.09),
        (75.954, -13.403, 63.895)  # 52

    ])

    poinsts_all2=np.array([

      (-62.991, -59.881, 16.357),
  (-44.598, -66.440, -2.466),
   (-9.505, -53.395, -5.619
    ),
   (-43.153, -58.929, 0.509),

    (-52.239
    , -45.040
    , 15.425
    ), # 4
    (-33.906
    , -48.283
    , 6.463
    ),
   (-20.721
    , -39.754
    , 12.819
    ),
    (-35.727
    , -37.322
    , 7.518
    ),
    (-41.155
    , -44.806
    , 16.798
    ),

    (62.378, -59.881, 16.357),


    (43.985, -66.440, -2.466),
    (8.891, -53.395, -5.619),
    (42.540, -58.929, 0.509),

    (51.626, -45.040, 15.425), # 13
    (33.293, -48.283, 6.463),
    (20.108, -39.754, 12.819
    ),
    (35.113
    , -37.322
    , 7.518
    ),
    (40.542
    , -44.806
    , 16.798
    ),

    (-19.060
    , 0.399
    , -4.938
    ), # 18
    (-0.307
    , 11.581
    , -11.801
    ),
    (18.446
    , 0.399
    , -4.938
    ),
    (-0.307
    , -9.122
    , -25.421
    ),

    (-28.040
    , 32.593
    , 11.042
    ), # 22
    (-0.307
    , 26.717
    , -12.994
    ),
    (27.427
    , 32.593
    , 11.042
    ),
    (-0.307
    , 47.513
    , -6.076
    ),

    (-78.149
    , -55.432
    , 56.154
    ), # 26
    (-71.851
    , 15.568
    , 54.652
    ),
    (-58.259
    , 38.488
    , 36.910
    ),
    (-0.307
    , 75.399
    , 2.238
    ),
    (57.646
    , 38.488
    , 36.910
    ),
    (71.237
    , 15.568
    , 54.652
    ),
    (77.535
    , -55.432
    , 56.154
    ),

    (-18.573
    , 26.408
    , -5.221
    ), # 33
    (-8.858
    , 23.916
    , -11.577
    ),
    (8.245
    , 23.916
    , -11.577
    ),
    (17.959
    , 26.408
    , -5.221
    ),

    (9.588
    , 46.857
    , -4.308
    ), # 37
    (-10.201
    , 46.857
    , -4.308
    ),

    (-15.919
    , 33.269
    , 0.893
    ), # 39
    (-0.307
    , 34.783
    , -3.994
    ),
    (15.306
    , 33.269
    , 0.893
    ),
    (-15.919
    , 33.269
    , 0.893
    ),
    (-0.307
    , 34.783
    , -3.994
    ),
    (15.306
    , 33.269
    , 0.893
    ),

    (-15.178
    , -49.704
    , 2.448
    ), # 45
    (14.565
    , -49.704
    , 2.448
    ),
    (-75.954
    , -13.403
    , 63.895
    ),
    (-67.21
    , 26.596
    , 46.09
    ),
    (-34.49
    , 61.691
    , 19.238
    ),
    (34.49
    , 61.691
    , 19.238
    ),
    (67.21
    , 26.596
    , 46.09
    ),
    (75.954
    , -13.403
    , 63.895
    ) # 52

])
# 3D model points.
    model_points = np.array([
                             poinsts_all[0],#
                             poinsts_all[2],
                             poinsts_all[4],
                             poinsts_all[6],
                             poinsts_all[9],
                             poinsts_all[11],
                             poinsts_all[13],
                             poinsts_all[15],

                             poinsts_all[18],
                             poinsts_all[19],
                             poinsts_all[20],
                             poinsts_all[21],

                             poinsts_all[26],
                             poinsts_all[27],
                             poinsts_all[31],
                             poinsts_all[32]

])

    focal_length = size[1]
    center = (size[1] / 2, size[0] / 2)
    camera_matrix = np.array(
        [[focal_length, 0, center[0]],
         [0, focal_length, center[1]],
         [0, 0, 1]], dtype="double"
    )

   # print "Camera Matrix :\n {0}".format(camera_matrix)

    dist_coeffs = np.zeros((4, 1))  # Assuming no lens distortion
    (success, rotation_vector, translation_vector) = cv2.solvePnP(model_points, image_points, camera_matrix,
                                                                  dist_coeffs, flags=cv2.CV_ITERATIVE)


    rotatematrix=cv2.Rodrigues(rotation_vector)[0]
    measured_eulers = rotationMatrixToEulerAngles(rotatematrix)

    print measured_eulers


    for p in image_points:
        cv2.circle(image, (int(p[0]), int(p[1])), 3, (0 ,0 ,255), -1)

    cv2.imshow("Output", image)
    cv2.waitKey(0)





