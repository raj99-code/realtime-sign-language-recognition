#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "recog_model.h"
#include <iostream>
#include <stdio.h>

using namespace cv;

using namespace std;

int main() {
    

Mat image, HSV;

namedWindow("Display window");

VideoCapture cap(0);

if (!cap.isOpened()) {

cout << "cannot open camera";

}

while (true) {

cap >> image;
LetterRecog model("asl_alphabet.onnx");

Point2f forw = model.forward(image);
imshow("Live", image);
cout << "Detected class is: " << forw.x << endl;
cout << "\n" << endl;


waitKey(25);

}

return 0;

}
