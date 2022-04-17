#ifndef _HAND_GESTURE_
#define _HAND_GESTURE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "myImage.hpp"
#include "main.hpp"


class HandGesture{
	public:
		MyImage m;
		HandGesture();
		std::vector<std::vector<cv::Point> > contours;
		std::vector<std::vector<int> >hullI;
		std::vector<std::vector<cv::Point> >hullP;

		cv::Rect rect;
	
		int cIdx;
		int frameNumber;
	
		cv::Rect bRect;
		double bRect_width;
		double bRect_height;
		bool isHand;
		bool detectIfHand();
		void initVectors();
		
	
		
	private:
		
		int fontFace;
	
		void analyzeContours();

		

};




#endif
