
#ifndef ROI 
#define ROI

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

class My_ROI{
	public:
		My_ROI();
		My_ROI(cv::Point upper_corner, cv::Point lower_corner,cv::Mat src);
		cv::Point upper_corner, lower_corner;
		cv::Mat roi_ptr;
		cv::Scalar color;
		int border_thickness;
		void draw_rectangle(cv::Mat src);
};



#endif
