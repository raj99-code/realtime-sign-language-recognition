#ifndef _MYIMAGE_
#define _MYIMAGE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>


class MyImage{
	public:
		MyImage(int webCamera);
		MyImage(cv::Mat &src);
		MyImage();
		cv::Mat srcLR;
		
		cv::Mat src;
		cv::Mat bw;
		std::vector<cv::Mat> bwList;
		cv::VideoCapture cap;		
		int cameraSrc; 
		void initWebCamera(int i);
};



#endif
