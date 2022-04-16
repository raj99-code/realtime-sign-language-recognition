#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <iostream>

class LetterRecog {
	public:
		LetterRecog(const std::string &path);
		cv::Point2f forward(cv::Mat &src);
	private:
		cv::dnn::Net net;
		void loadModel(const std::string &path);
		void Mat_Normalization(cv::Mat &matrix);
};


