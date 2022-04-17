#include "FrameGen.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
	/**
   * @brief capture frames from camera module
   * 
   *
   */

FrameGen::FrameGen(){
}

FrameGen::FrameGen(int Camera){
	cameraSrc=Camera;
	cap=cv::VideoCapture(Camera);
}
