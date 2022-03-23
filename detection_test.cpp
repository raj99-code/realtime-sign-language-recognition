#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "recog_model.h"
#include <iostream>
#include <stdio.h>
#include "BackgroundRemover.h"

using namespace cv;

using namespace std;


/*
 Author: Pierfrancesco Soffritti https://github.com/PierfrancescoSoffritti
*/

BackgroundRemover::BackgroundRemover(void) {
	background;
	calibrated = false;
}

void BackgroundRemover::calibrate(Mat input) {
	cvtColor(input, background, COLOR_BGR2GRAY);
	calibrated = true;
}

Mat BackgroundRemover::getForeground(Mat input) {
	Mat foregroundMask = getForegroundMask(input);

	//imshow("foregroundMask", foregroundMask);

	Mat foreground;
	input.copyTo(foreground, foregroundMask);

	return foreground;
}

Mat BackgroundRemover::getForegroundMask(Mat input) {
	Mat foregroundMask;

	if (!calibrated) {
		foregroundMask = Mat::zeros(input.size(), CV_8UC1);
		return foregroundMask;
	}

	cvtColor(input, foregroundMask, COLOR_BGR2GRAY);

	removeBackground(foregroundMask, background);
	
	return foregroundMask;
}

void BackgroundRemover::removeBackground(Mat input, Mat background) {
	int thresholdOffset = 10;

	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			uchar framePixel = input.at<uchar>(i, j);
			uchar bgPixel = background.at<uchar>(i, j);

			if (framePixel >= bgPixel - thresholdOffset && framePixel <= bgPixel + thresholdOffset)
				input.at<uchar>(i, j) = 0;
			else
				input.at<uchar>(i, j) = 255;
		}
	}
}

BackgroundRemover backgroundRemover;
int main() {
    
Mat frame, frameOut, handMask, foreground, fingerCountDebug;    

Mat image, HSV;

namedWindow("Display window");

VideoCapture cap(0);

if (!cap.isOpened()) {

cout << "cannot open camera";

}

while (true) {

cap >> image;
foreground = backgroundRemover.getForeground(image);
imshow("foreground", foreground);

LetterRecog model("asl_alphabet.onnx");

Point2f forw = model.forward(image);
//imshow("Live", image);
cout << "Detected class is: " << forw.x << endl;
cout << "\n" << endl;


int key = waitKey(1);

if (key == 27) // esc
	break;

else if (key == 98) // b
	backgroundRemover.calibrate(frame);

}

return 0;

}
