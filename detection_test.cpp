#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "recog_model.h"
#include <iostream>
#include <stdio.h>
#include "BackgroundRemover.h"
#include "SkinDetector.h"

using namespace cv;

using namespace std;


int main() {
    
Mat frame, frameOut, handMask, foreground, fingerCountDebug;    

Mat image, HSV;
BackgroundRemover backgroundRemover;
SkinDetector skinDetector;

namedWindow("Display window");

VideoCapture cap(0);

if (!cap.isOpened()) {

cout << "cannot open camera";

}

while (true) {

cap >> image;
frameOut=image.clone();
skinDetector.drawSkinColorSampler(frameOut);

foreground = backgroundRemover.getForeground(image);
handMask = skinDetector.getSkinMask(foreground);
imshow("foreground1", frameOut);
imshow("foreground2", foreground);
imshow("foreground3", handMask);

LetterRecog model("asl_alphabet.onnx");

Point2f forw = model.forward(image);
imshow("Live", image);
cout << "Detected class is: " << forw.x << endl;
cout << "\n" << endl;


int key = waitKey(1);

if (key == 27) // esc
	break;

else if (key == 98) // b
	backgroundRemover.calibrate(image);
else if (key == 115) // s
	skinDetector.calibrate(image);

}

return 0;

}
