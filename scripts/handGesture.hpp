#ifndef _HAND_GESTURE_
#define _HAND_GESTURE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "main.hpp"
#include "myImage.hpp"

using namespace cv;
using namespace std;

class HandGesture{
	public:
		MyImage m;
		HandGesture();
		vector<vector<Point> > contours;
		vector<vector<int> >hullI;
		vector<vector<Point> >hullP;

		Rect rect;
	
		int cIdx;
		int frameNumber;
	
		Rect bRect;
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
