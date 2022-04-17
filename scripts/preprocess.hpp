#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>


class Preprocess{
	public:
		Preprocess(cv::Mat &src);
		cv::Mat srcLR;
		cv::Mat src;
};
