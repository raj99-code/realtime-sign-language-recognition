#ifndef _MYIMAGE_
#define _MYIMAGE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <vector>


using namespace std;

class MyImage{
	public:
		MyImage(int webCamera);
		MyImage();
		cv::Mat srcLR;
		
		cv::Mat src;
		cv::Mat bw;
		vector<cv::Mat> bwList;
		cv::VideoCapture cap;		
		int cameraSrc; 
		void initWebCamera(int i);
};



#endif
