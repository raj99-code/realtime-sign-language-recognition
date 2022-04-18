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
#include <chrono>

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"

#include "FrameGen.hpp"
#include "roi.hpp"
#include "main.hpp"
#include "RecogModel.h"
#include "handDetect.h"

#include "ThreadSafeFramesQueue.hpp"


int main(){
	bool stopRunning = false;
	FrameGen m(0);
	std::cout << "Loaded" << std::endl;
	InterHand hg;
	handDetect hd;
	hd.init(&m);
	m.cap >> m.src;
    cv::namedWindow("img1", CV_WINDOW_KEEPRATIO);
	std::vector<char> classes = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '<', '_', ' '};
	LetterRecog model("../model/asl_alphabet.onnx");

	hd.waitForPalmCover(&m);
	hd.average(&m);
	hd.initTrackbars();
	int i = 0;
	int j = 0;
	int k = 0;

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
			cv::imshow("img1", m.src);
			std::this_thread::sleep_for(std::chrono::milliseconds(700));
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
			FrameGen preprocess(inp);
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
			std::cout << curr_letter << std::endl;
		}
	}


	captureThread.join();
	inferenceThread.join();
	//out.release();
	m.cap.release();
	cv::destroyAllWindows();
	return 0;
}
