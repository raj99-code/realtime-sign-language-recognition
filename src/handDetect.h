#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "FrameGen.hpp"
#include "roi.hpp"
#include "InterHand.hpp"
#include <vector>
#include <cmath>


/**
  @brief Class to detect the hand from a video stream using thresholding and contours

*/



class handDetect{

    public:

    handDetect();


 /**
  @brief Function to take the initial video stream and specify the area of calibrartion squares
*/


    void init(FrameGen *m);


   /**
   * @brief specify the location of the calibration squares on the incoming frame and draw them 
   * 
   *
   */

    void waitForPalmCover(FrameGen* m);

   /**
   * @brief calculate the final average color of the hand in hue lightness saturation space based on 
   * average skin color from the calibration squares to find the threshold and detect the hand.
   *
   */

    void average(FrameGen *m);

   /**
   * @brief create trackbars to be able to manually adjust the threshold if needed. To use the trackbars, 
   * umcomment the function in the main file.
   *
   */   

    void initTrackbars();

   /**
   * @brief getting the lower and upper boundries of the hand color. the function calculate these boundries
   * based on the average color from the calibration squares after normalization and then perform median Blur to remove the noise
   *
   */
    void produceBinaries(FrameGen *m);

   /**
   * @brief crop the detected hand area of each frame and store it in Mat object to be fed to the inference model
   * 
   *
   */
    cv::Mat crop(FrameGen *m, InterHand *hg);

   /**
   * @brief create contours for the hand from the incoming frame to get the hand location and detect the hand
   * 
   *
   */
    void makeContours(FrameGen *m, InterHand* hg);

    private:

   /**
   * @brief convert the frame color to the original color from two input color spaces 
   * 
   *
   */
   
    void col2origCol(int hsv[3], int bgr[3], cv::Mat src);

   /**
   * @brief function to print text on the frame when needed
   * 
   *
   */
    
    void printText(cv::Mat src, std::string text);

   /**
   * @brief calculate the median value of the color averaging results 
   * 
   *
   */
    int getMedian(std::vector<int> val);

   /**
   * @brief aquire the average hand skin color from the calibration squares
   * 
   *
   */
    void getAvgColor(FrameGen *m,My_ROI roi,int avg[3]);

    /**
   * @brief get the color boundries and normalize them to 0-255
   * 
   *
   */
    void normalizeColors(FrameGen * myImage);

    /**
   * @brief get the biggest contours and remove the cotours that do not represent hands
   * 
   *
   */
    int findBiggestContour(std::vector<std::vector<cv::Point>> contours);
   
   

};
