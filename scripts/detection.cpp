#include "recog_model.h"

int main() {
   cv::Mat in_img = cv::imread("/home/pi/American-Sign-Language-Recognition-using-Deep-Neural-Network/images/sample_prediction (5).png");
   std::vector<std::string> classes = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "del", "nothing", "space"};
   LetterRecog model("/home/pi/realtime-sign-language-recognition/model/asl_alphabet.onnx");
   cv::Point2f forw = model.forward(in_img);
   std::cout << "Detected class is: " << classes[forw.x] << std::endl;
   return 0;
}
