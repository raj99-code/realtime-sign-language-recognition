#include "RecogModel.h"

int main() {
   std::vector<std::string> classes = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "del", "nothing", "space"};
   for(std::string class_name : classes) {
	   if (class_name == "del") { continue; }
	   std::string path = "../imgs/" + class_name + "_test.jpg";
	   cv::Mat in_img = cv::imread(path);
	   LetterRecog model("../model/asl_alphabet.onnx");
	   cv::Point2f forw = model.forward(in_img);
	   std::cout << "Original class is: " << class_name << std::endl;
	   std::cout << "Detected class is: " << classes[forw.x] << std::endl;
	   std::cout << "\n" << std::endl;
   }
   return 0;
}
