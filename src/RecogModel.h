#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <iostream>

   /**
   * @brief class for sign language recognition. An ONNX model is loaded to be used for inference to recognize sign language alphabet
   * 
   *
   */

class LetterRecog {
	public:
		/**
   * @brief initialize the required vectors
   * 
   *
   */
		LetterRecog(const std::string &path);
		cv::Point2f forward(cv::Mat &src);
	private:
		cv::dnn::Net net;
		void loadModel(const std::string &path);
		void Mat_Normalization(cv::Mat &matrix);
};


