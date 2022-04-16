

#PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:${PKG_CONFIG_PATH}
#export PKG_CONFIG_PATH

all:
	g++  -std=c++11 -pthread -o opencv main.cpp handDetect.cpp RecogModel.cpp myImage.cpp handGesture.cpp roi.cpp  `pkg-config --cflags --libs opencv4` 

