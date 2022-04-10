#include "hand_detect.h"

int main() {
	std::string hand_detection_model_path = "../model/hand_detection.tflite";
	HandDetect hand_detection_model(hand_detection_model_path);
	
	std::string path = "../imgs/B_test.jpg";
	cv::Mat in_img = cv::imread(path);
	
	hand_detection_model.getInference(in_img);
	
}
