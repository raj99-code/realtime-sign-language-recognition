

#PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:${PKG_CONFIG_PATH}
#export PKG_CONFIG_PATH

all:
	g++ -pthread main.cpp myImage.cpp handGesture.cpp roi.cpp -lArduiPi_OLED -o opencv `pkg-config --cflags --libs opencv4` 

