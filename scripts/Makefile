

#PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:${PKG_CONFIG_PATH}
#export PKG_CONFIG_PATH

all:
	g++ -pthread -std=c++11 main_thread.cpp handDetect.cpp RecogModel.cpp myImage.cpp handGesture.cpp roi.cpp led_display.cpp -lArduiPi_OLED -o opencv `pkg-config --cflags --libs opencv4`
