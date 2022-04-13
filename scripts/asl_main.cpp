#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include "BackgroundRemover.h"
#include "SkinDetector.h"
#include "FaceDetector.h"
#include "FingerCount.h"
#include "RecogModel.h"

int main(int, char**) {
	cv::VideoCapture videoCapture(0);
	videoCapture.set(cv::CAP_PROP_SETTINGS, 1);

	if (!videoCapture.isOpened()) {
		std::cout << "Can't find camera!" << std::endl;
		return -1;
	}

	cv::Mat frame, frameOut, handMask, foreground, fingerCountDebug,cropped_im;

	BackgroundRemover backgroundRemover;
	SkinDetector skinDetector;
	FaceDetector faceDetector;
	FingerCount fingerCount;
	cv::Rect boundRect;
	
    std::vector<std::string> classes = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "del", "nothing", "space"};
	LetterRecog model("../model/asl_alphabet.onnx");

	while (true) {
		videoCapture >> frame;
		frameOut = frame.clone();

		skinDetector.drawSkinColorSampler(frameOut);

		foreground = backgroundRemover.getForeground(frame);
		
		faceDetector.removeFaces(frame, foreground);
		handMask = skinDetector.getSkinMask(foreground);
		fingerCountDebug = fingerCount.findFingersCount(handMask, frameOut);

	
		cv::imshow("output", frameOut);
		cv::imshow("foreground", foreground);
		//cv::imshow("handMask", handMask);
		//cv::imshow("handDetection", fingerCountDebug);
		
		
		if(fingerCount.getBoundingRectArea() > 0) {
			boundRect = fingerCount.getBoundingRect();
			cv::Mat cropped = frameOut(boundRect);
			cv::Point2f forw = model.forward(cropped);
			std::cout << classes[forw.x] << std::endl;
			std::cout << "\n" << std::endl;
			cv::imshow("cropped", cropped);
		} else {
			cv::imshow("cropped", frameOut);
		}
		

		int key = cv::waitKey(1);

		if (key == 27) // esc
			break;
		else if (key == 98) // b
			backgroundRemover.calibrate(frame);
		else if (key == 115) // s
			skinDetector.calibrate(frame);
	}

	return 0;
}
