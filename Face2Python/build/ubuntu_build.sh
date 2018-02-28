#!/bin/bash


if [ ! -n "$CXX" ]; then
    CXX=g++
fi

current_dir=`pwd`

export OPENCV_PATH=../3rdparty/libopencv/ubuntu/include/
#export LIB_PATH=/usr/local/include/
OUT_PATH=lib/ubuntu/

swig -I$PYTHONPATH -I$OPENCV_PATH -python -c++ MTLabFace.i



gcc -std=c++11 -fPIC -I$PYTHONPATH  -I$OPENCV_PATH  \
../src/MTDetector.cpp \
MTLabFace_wrap.cxx  \
-o $OUT_PATH/_MTLabFace.so -shared  \
-I ../3rdparty/libMTLabFace/ubuntu/include  \
-I ../3rdparty/libyuv/include  \
-I ../3rdparty  \
-I ..      \
-Wl,-Bstatic ../3rdparty/libopencv/ubuntu/lib/libopencv.a   ../3rdparty/libyuv/ubuntu/lib/libyuv.a \
-Wl,-Bdynamic -L ../3rdparty/libMTLabFace/ubuntu/lib \
-L ../3rdparty/libyuv/ubuntu/lib \
-L.  -lMTLabFace   

cp  ../3rdparty/libMTLabFace/ubuntu/lib/libMTLabFace.so $OUT_PATH
cp  MTLabFace.py $OUT_PATH

cp $OUT_PATH/* ../demo/

