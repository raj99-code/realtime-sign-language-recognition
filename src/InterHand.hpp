

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "FrameGen.hpp"
#include "main.hpp"

  /**
   * @brief Class to handle intermediate processing of the hand detection such as hand bounding rectangle and contour vectors
   * 
   *
   */

class InterHand{
	public:
		FrameGen m;
		InterHand();
		std::vector<std::vector<cv::Point> > contours;

		cv::Rect rect;
	
		int cIdx;
		int frameNumber;
	
		cv::Rect bRect;
		double bRect_width;
		double bRect_height;
		bool isHand;

   /**
   * @brief return the results of the hand detection
   * 
   *
   */
		bool detectIfHand();

	
  
		
	
		
	private:
		
		int fontFace;

	/**
   * @brief define the hand area for processing 
   * 
   *
   */
	
		void analyzeContours();

		

};





