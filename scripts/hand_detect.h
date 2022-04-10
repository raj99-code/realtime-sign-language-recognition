#include <opencv2/opencv.hpp>
#include <opencv2/dnn/dnn.hpp>
#include <iostream>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/model.h"
#include "tensorflow/lite/model_builder.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/delegates/flex/delegate_data.h"


class HandDetect {
	public:
		HandDetect(const std::string &path);
		void getInference(cv::Mat &src);
	private:
		std::unique_ptr<tflite::FlatBufferModel> model;
		std::unique_ptr<tflite::Interpreter> interpreter;
		void loadModel(const std::string &path);
		void buildInterpreter();
};

HandDetect::HandDetect(const std::string &path) {
	this->loadModel(path);
	this->buildInterpreter();
}

void HandDetect::loadModel(const std::string &path) {
	this->model = tflite::FlatBufferModel::BuildFromFile(path.c_str());
	// TFLITE_MINIMAL_CHECK(this->model != nullptr);
	std::cout << "Model loaded successfully" << std::endl;
}

void HandDetect::buildInterpreter() {
	tflite::ops::builtin::BuiltinOpResolver resolver;
	tflite::InterpreterBuilder(*this->model, resolver)(&this->interpreter);
	std::cout << "Interpreter built successfully" << std::endl;
}

void HandDetect::getInference(cv::Mat &src) {
	cv::Mat image;
	int cam_width = src.cols;
	int cam_height = src.rows;
	
	this->interpreter->ResizeInputTensor(0, {1, cam_width, cam_height, 3});
	this->interpreter->AllocateTensors();
	this->interpreter->Invoke();

	std::cout << "Inference run successfully" << std::endl;
}
