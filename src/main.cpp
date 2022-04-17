

/* #include "ArduiPi_OLED_lib.h"
#include "Adafruit_GFX.h"
#include "ArduiPi_OLED.h"
 */
#include "FrameGen.hpp"
#include "roi.hpp"
//#include "InterHand.hpp"
#include "main.hpp"
#include "RecogModel.h"
#include "handDetect.h"
//#include "led_display.hpp"

/**
  @brief Function to initialize live camera stream and trackbars windows
*/

void initWindows(FrameGen m){
    //namedWindow("trackbars",CV_WINDOW_KEEPRATIO); ///< trackbars to adjust the threshold manually for hand segmentation
    cv::namedWindow("img1",CV_WINDOW_FULLSCREEN);
}


/**
  @brief Function to show the the live camera stream and the thresholding result in smaller window 
*/

void showWindows(FrameGen m){
	cv::pyrDown(m.bw,m.bw); ///< redcuing the shown thresholded image size using Gaussian pyramids 
	cv::pyrDown(m.bw,m.bw);
	cv::Rect roi( cv::Point( 3*m.src.cols/4,0 ), m.bw.size()); ///< defining the location of the thresholded image in the original window
	std::vector<cv::Mat> channels;
	cv::Mat result;
	for(int i=0;i<3;i++)
		channels.push_back(m.bw);
	merge(channels,result);
	result.copyTo( m.src(roi));
	cv::imshow("img1",m.src);	
}



int main(){
	
	FrameGen m(0);		
	InterHand hg;
	handDetect hd;
	hd.init(&m);		
	m.cap >>m.src;
    cv::namedWindow("img1",CV_WINDOW_KEEPRATIO);
	std::vector<char> classes = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '<', '_', ' '};
	LetterRecog model("../model/asl_alphabet.onnx");
	
	hd.waitForPalmCover(&m);
	hd.average(&m);
	cv::destroyWindow("img1");
	initWindows(m);
	hd.initTrackbars();
	// LedDisplay display;
	
	for(;;){
		
		m.cap >> m.src;
		cv::flip(m.src,m.src,1);
		cv::pyrDown(m.src,m.srcLR);
		cv::blur(m.srcLR,m.srcLR,cv::Size(3,3));
		cv::cvtColor(m.srcLR,m.srcLR,ORIGCOL2COL);
		hd.produceBinaries(&m);
		cv::cvtColor(m.srcLR,m.srcLR,COL2ORIGCOL);
		hd.makeContours(&m, &hg);
		
		cv::Mat l=hd.crop(&m,&hg);
		cv::Point2f forw = model.forward(l);
		std::cout << classes[forw.x] << std::endl;
		std::cout << "\n" << std::endl;
		//display.addChar(classes[forw.x]);
			
		
		showWindows(m);
		
    	if(cv::waitKey(30) == char('q')) break;
	}
	cv::destroyAllWindows();
	
	m.cap.release();
    return 0;
}
