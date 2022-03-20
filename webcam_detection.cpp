#include "recog_model.h"

int main() {
	cv::Mat curr_image;
	cv::namedWindow("Display Window");
	cv::VideoCapture cap(0);
    std::vector<std::string> classes = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "del", "nothing", "space"};
	LetterRecog model("../model/asl_alphabet.onnx");
	
	if (!cap.isOpened()) { std::cout << "Unable to open camera" << std::endl; }
	
	while (true) {
		cap >> curr_image;
		cv::imshow("Display Window", curr_image);
		cv::Point2f forw = model.forward(curr_image);
	    std::cout << classes[forw.x] << std::endl;
	    std::cout << "\n" << std::endl;
		cv::waitKey(25);
	}

	return 0;
}
