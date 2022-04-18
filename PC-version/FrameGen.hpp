#ifndef _MYIMAGE_
#define _MYIMAGE_ 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

	/**
   * @brief Class for video capturing from camera module. It takes the frames from the camera and passes it to other classes for further processing.
   * 
   *
   */

class FrameGen{
	public:
		FrameGen(int Camera);
		FrameGen(cv::Mat &src);
		FrameGen();
		cv::Mat srcLR;
		
		cv::Mat src;
		cv::Mat bw;
		std::vector<cv::Mat> bwList;
		cv::VideoCapture cap;		
		int cameraSrc; 
		void Camera(int i);
};



#endif
