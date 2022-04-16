#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "myImage.hpp"
#include "roi.hpp"
#include "handGesture.hpp"
#include <vector>
#include <cmath>




using namespace std;

/* Global Variables  */


class handDetect{

    public:

    handDetect();
    void init(MyImage *m);
    void waitForPalmCover(MyImage* m);
    void average(MyImage *m);
    void initTrackbars();
    void produceBinaries(MyImage *m);
    cv::Mat crop(MyImage *m, HandGesture *hg);
    void makeContours(MyImage *m, HandGesture* hg);

    private:
   
    void col2origCol(int hsv[3], int bgr[3], cv::Mat src);
    void printText(cv::Mat src, string text);
    int getMedian(vector<int> val);
    void getAvgColor(MyImage *m,My_ROI roi,int avg[3]);
    void normalizeColors(MyImage * myImage);
    int findBiggestContour(vector<vector<cv::Point> > contours);
   
   

};