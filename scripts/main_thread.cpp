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
#include <thread>

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

#include "ThreadSafeFramesQueue.hpp"


void initWindows(MyImage m){
    //namedWindow("trackbars",CV_WINDOW_KEEPRATIO);
    cv::namedWindow("img1", CV_WINDOW_KEEPRATIO); //CV_WINDOW_FULLSCREEN);
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
	cv::imshow("img1", m.src);	
}



int main(){
	bool stopRunning = false;
	MyImage m(0);
	std::cout << "Loaded" << std::endl;		
	HandGesture hg;
	handDetect hd;
	hd.init(&m);
	m.cap >> m.src;
    cv::namedWindow("img1", CV_WINDOW_KEEPRATIO);
	std::vector<char> classes = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '<', '_', ' '};
	LetterRecog model("../model/asl_alphabet.onnx");

	hd.waitForPalmCover(&m);
	hd.average(&m);
	cv::destroyWindow("img1");
	initWindows(m);
	hd.initTrackbars();
	LedDisplay display;

	/** THREAD 1: Frames-capturing thread
	 * UPDATES: raw frames queue
	 */
	ThreadSafeFramesQueue<cv::Mat> rawFrames;
	std::thread captureThread([&](){
		while (1) {
			m.cap >> m.src;
			char key = cv::waitKey(1);
			if (key == 27 || m.src.empty()) {
				//! If ESC is pressed OR stream is exhausted, Send the kill-signal to other threads
				stopRunning = true;
				break;
			}
			/** @note need to use clone here to pass a copy, otherwise input could be corrupted */
			rawFrames.push(m.src.clone());  
		}
	});
	

	/** THREAD 2: Inference thread
	 * UPDATES: inference frames queue - inferred chars
	 */
	ThreadSafeFramesQueue<char> inferenceQueue;
	std::thread inferenceThread([&](){
		while (!stopRunning || !rawFrames.empty()) {
		  cv::Mat inp;
		  if (rawFrames.Pop(inp)) {
			//! inp is valid, run inference on it
			MyImage preprocess(inp);
			hd.produceBinaries(&preprocess);
			cv::cvtColor(preprocess.srcLR, preprocess.srcLR, COL2ORIGCOL);
			hd.makeContours(&preprocess, &hg);

			cv::Mat cropped = hd.crop(&preprocess, &hg);
			cv::Point2f forw = model.forward(cropped);
			inferenceQueue.Push(classes[forw.x]);
		  }
		}
	});
	


	while(!stopRunning || !inferenceQueue.empty()) {
		char curr_letter;
		if (inferenceQueue.Pop(curr_letter)) {
			display.addChar(curr_letter);
		}
	}


	captureThread.join();
	inferenceThread.join();
	//out.release();
	m.cap.release();
	cv::destroyAllWindows();
    return 0;
}
