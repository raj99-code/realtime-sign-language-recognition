#include "myImage.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "roi.hpp"


My_ROI::My_ROI(){
	upper_corner=cv::Point(0,0);
	lower_corner=cv::Point(0,0);
}

My_ROI::My_ROI(cv::Point u_corner, cv::Point l_corner, cv::Mat src){
	upper_corner=u_corner;
	lower_corner=l_corner;
	color=cv::Scalar(0,255,0);
	border_thickness=2;
	roi_ptr=src(cv::Rect(u_corner.x, u_corner.y, l_corner.x-u_corner.x,l_corner.y-u_corner.y));
}

void My_ROI::draw_rectangle(cv::Mat src){
	cv::rectangle(src,upper_corner,lower_corner,color,border_thickness);
}

