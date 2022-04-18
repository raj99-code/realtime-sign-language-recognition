#include "RecogModel.h"

LetterRecog::LetterRecog(const std::string &path) {
	this->loadModel(path);
	this->net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	this->net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

void LetterRecog::loadModel(const std::string &path) {
	this->net = cv::dnn::readNetFromONNX(path);
	// std::cout << "Model loaded successfully" << std::endl;
}

void LetterRecog::Mat_Normalization(cv::Mat &matrix) {
	cv::normalize(matrix, matrix, 0, 255, cv::NORM_MINMAX);
	//std::cout << "Image Normalized" << std::endl;
}

cv::Point2f LetterRecog::forward(cv::Mat &src) {
	cv::Mat blob;
	cv::resize(src, blob, cv::Size(200, 200));
	blob.convertTo(blob, CV_32F, 1./255, -0.5);
	blob = blob.reshape(1, {1, 200, 200, 3});
	this->net.setInput(blob);
	//std::cout << "Input set successfully" << std::endl;
	
	cv::Mat prob = this->net.forward();
	//std::cout << "Forward Successful" << std::endl;

	this->Mat_Normalization(prob);
	cv::Point classIdPoint;
	double confidence;
	
	cv::minMaxLoc(prob.reshape(1, 1), 0, &confidence, 0, &classIdPoint);
	int classId = classIdPoint.x;
	//std::cout << "Normalization Successful" << std::endl;
	return cv::Point2f(classId, confidence);
}
