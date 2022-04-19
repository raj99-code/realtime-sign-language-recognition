#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <iostream>

   /**
   * @brief Class for sign language recognition.
    An ONNX model is loaded to be used for inference to recognize sign language alphabet and then the incoming frame is normalized. Next, the frame is forwarded
    to the model to extract features and output the inference result.
   * 
   *
   */

class LetterRecog {
	public:
		/**
   * @brief Model loading and setting the parameters
   * 
   *
   */
		LetterRecog(const std::string &path);
      /**
   * @brief Pass the frame to the model for inference
   * 
   *
   */
		cv::Point2f forward(cv::Mat &src);
	private:
		cv::dnn::Net net;
      	/**
   * @brief Load the ONNX model
   * 
   *
   */
		void loadModel(const std::string &path);

      	/**
   * @brief Normalize the incoming frame.
   * 
   *
   */
		void Mat_Normalization(cv::Mat &matrix);
};


