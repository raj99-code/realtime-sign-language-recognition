#include "InterHand.hpp"


InterHand::InterHand(){
	frameNumber = 0;	
	fontFace = cv::FONT_HERSHEY_PLAIN;
}


void InterHand::analyzeContours(){
	bRect_height=bRect.height;
	bRect_width=bRect.width;
}

bool InterHand::detectIfHand(){
	analyzeContours();
	double h = bRect_height; 
	double w = bRect_width;
	isHand=true;
	if(h==0 || w == 0){
		isHand=false;
	}else if(h/w > 4 || w/h >4){
		isHand=false;	
	}else if(bRect.x<20){
		isHand=false;	
	}	
	return isHand;
}
