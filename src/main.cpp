

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

/*! \mainpage kNOw SIGN Source Code Documentation
 *
 * \section intro_sec Introduction
 *
 * kNOw SIGN is a sign language (ASL) recognition project that utilizes real-time and efficient programming to help 
 * mute and deaf by establishing two-way communication channel with normal people who have never studied sign language. 
 * 
 * Source Code documentation is explained here, for more information about the project please visit <a href="https://github.com/raj99-code/realtime-sign-language-recognition"> kNOw SIGN</a>

 * \section struct_sec Project Structure
 * The project has 3 main parts to achieve the recognition process:

 1. The hand detection part where all the frame pre-processing needed for recognition is done.
 2. The recognition part which is responsible for using the deep learning model to inference the letters recognized by hand gestures.
 3. The display section (Raspberry Pi version only) which handels the output of the recognition resuls to the OLED dispaly
 *

 \section stage_sec Processing Stages

 The project involves several stages to produce the recognition results and each stage is done but one or more functions of the program. These stages are done by:

 1. FrameGen
 2. handDetect
 3. InterHand
 4. My_ROI
 5. LetterRecog
 6. LedDisplay

 The description of these stages is explained in detailes in the individual class documentation.

 \section res_sec Resources
 
 - <a href="https://github.com/raj99-code/realtime-sign-language-recognition"> Github</a>
 - <a href="https://raj99-code.github.io/realtime-sign-language-recognition"> kNOw SIGN Page</a>
 


 * 
 *
 * 
 */


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
