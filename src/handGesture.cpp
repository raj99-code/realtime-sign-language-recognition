#include "handGesture.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


using namespace std;

HandGesture::HandGesture(){
	frameNumber=0;
	
	fontFace = cv::FONT_HERSHEY_PLAIN;
}

void HandGesture::initVectors(){
	hullI=vector<vector<int> >(contours.size());
	hullP=vector<vector<cv::Point> >(contours.size());
	
}

void HandGesture::analyzeContours(){
	bRect_height=bRect.height;
	bRect_width=bRect.width;
}



bool HandGesture::detectIfHand(){
	analyzeContours();
	double h = bRect_height; 
	double w = bRect_width;
	isHand=true;
	if(h==0 || w == 0){
		isHand=false;
	}else if(h/w > 4 || w/h >4){
		isHand=false;	
	}else if(bRect.x<20){
		isHand=false;	
	}	
	return isHand;
}







