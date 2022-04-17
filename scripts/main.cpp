#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <termios.h>
#include <netinet/in.h>
#include <getopt.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"

#include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"

#include "myImage.hpp"
#include "roi.hpp"
#include "handGesture.hpp"
#include "main.hpp"
#include "RecogModel.h"
#include "handDetect.h"
#include "led_display.hpp"


void initWindows(MyImage m){
    //namedWindow("trackbars",CV_WINDOW_KEEPRATIO);
    cv::namedWindow("img1",CV_WINDOW_FULLSCREEN);
}

void showWindows(MyImage m){
	cv::pyrDown(m.bw,m.bw);
	cv::pyrDown(m.bw,m.bw);
	cv::Rect roi( cv::Point( 3*m.src.cols/4,0 ), m.bw.size());
	std::vector<cv::Mat> channels;
	cv::Mat result;
	for(int i=0;i<3;i++)
		channels.push_back(m.bw);
	merge(channels,result);
	result.copyTo( m.src(roi));
	cv::imshow("img1",m.src);	
}



int main(){
	pthread_t thread1, thread2;
	// Mat iret1 = pthread_create( &thread1, NULL,function<void(Mat)> m,NULL);
	MyImage m(0);		
	HandGesture hg;
	handDetect hd;
	hd.init(&m);		
	m.cap >>m.src;
    cv::namedWindow("img1",CV_WINDOW_KEEPRATIO);
	std::vector<char> classes = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '<', '_', ' '};
	LetterRecog model("../model/asl_alphabet.onnx");
	
	hd.waitForPalmCover(&m);
	hd.average(&m);
	cv::destroyWindow("img1");
	initWindows(m);
	hd.initTrackbars();
	LedDisplay display;
	
	for(;;){
		
		m.cap >> m.src;
		cv::flip(m.src,m.src,1);
		cv::pyrDown(m.src,m.srcLR);
		cv::blur(m.srcLR,m.srcLR,cv::Size(3,3));
		cv::cvtColor(m.srcLR,m.srcLR,ORIGCOL2COL);
		hd.produceBinaries(&m);
		cv::cvtColor(m.srcLR,m.srcLR,COL2ORIGCOL);
		hd.makeContours(&m, &hg);
		
		cv::Mat l=hd.crop(&m,&hg);
		cv::Point2f forw = model.forward(l);
		std::cout << classes[forw.x] << std::endl;
		std::cout << "\n" << std::endl;
		display.addChar(classes[forw.x]);
			
		
		showWindows(m);
		//out << m.src;
		//imwrite("./images/final_result.jpg",m.src);
    	if(cv::waitKey(30) == char('q')) break;
	}
	cv::destroyAllWindows();
	//out.release();
	m.cap.release();
    return 0;
}
