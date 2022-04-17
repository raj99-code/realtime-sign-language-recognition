#include "handDetect.h"

int fontFace = cv::FONT_HERSHEY_PLAIN;
int square_len;
int avgColor[NSAMPLES][3] ;
int c_lower[NSAMPLES][3];
int c_upper[NSAMPLES][3];
int avgBGR[3];
int nrOfDefects;
int iSinceKFInit;
struct dim{int w; int h;}boundingDim;

cv::Mat edges;
My_ROI roi1, roi2,roi3,roi4,roi5,roi6;
std::vector <My_ROI> roi;
std::vector <cv::KalmanFilter> kf;
std::vector <cv::Mat_<float> > measurement;

handDetect::handDetect(){

}

void handDetect::init(MyImage *m){
	square_len=20;
	iSinceKFInit=0;
}

void handDetect::col2origCol(int hsv[3], int bgr[3], cv::Mat src){
	cv::Mat avgBGRMat=src.clone();	
	for(int i=0;i<3;i++){
		avgBGRMat.data[i]=hsv[i];	
	}
	cvtColor(avgBGRMat,avgBGRMat,COL2ORIGCOL);
	for(int i=0;i<3;i++){
		bgr[i]=avgBGRMat.data[i];	
	}
}

void handDetect::printText(cv::Mat src, std::string text){
	int fontFace =cv::FONT_HERSHEY_PLAIN;
	putText(src,text,cv::Point(src.cols/2, src.rows/10),fontFace, 1.2f,cv::Scalar(200,0,0),2);
}

void handDetect::waitForPalmCover(MyImage* m){
    m->cap >> m->src;
	flip(m->src,m->src,1);
	roi.push_back(My_ROI(cv::Point(m->src.cols/3, m->src.rows/6),cv::Point(m->src.cols/3+square_len,m->src.rows/6+square_len),m->src));
	roi.push_back(My_ROI(cv::Point(m->src.cols/4, m->src.rows/2),cv::Point(m->src.cols/4+square_len,m->src.rows/2+square_len),m->src));
	roi.push_back(My_ROI(cv::Point(m->src.cols/3, m->src.rows/1.5),cv::Point(m->src.cols/3+square_len,m->src.rows/1.5+square_len),m->src));
	roi.push_back(My_ROI(cv::Point(m->src.cols/2, m->src.rows/2),cv::Point(m->src.cols/2+square_len,m->src.rows/2+square_len),m->src));
	roi.push_back(My_ROI(cv::Point(m->src.cols/2.5, m->src.rows/2.5),cv::Point(m->src.cols/2.5+square_len,m->src.rows/2.5+square_len),m->src));
	roi.push_back(My_ROI(cv::Point(m->src.cols/2, m->src.rows/1.5),cv::Point(m->src.cols/2+square_len,m->src.rows/1.5+square_len),m->src));
	roi.push_back(My_ROI(cv::Point(m->src.cols/2.5, m->src.rows/1.8),cv::Point(m->src.cols/2.5+square_len,m->src.rows/1.8+square_len),m->src));
	
	
	for(int i =0;i<50;i++){
    	m->cap >> m->src;
		flip(m->src,m->src,1);
		for(int j=0;j<NSAMPLES;j++){
			roi[j].draw_rectangle(m->src);			
		}
		std::string imgText = std::string("Cover rectangles with palm");
		printText(m->src, imgText);	
		
		if(i==30){
		//	imwrite("./images/waitforpalm1.jpg",m->src);
		}

		imshow("img1", m->src);
		//out << m->src;
        if(cv::waitKey(30) >= 0) break;
	}
}

int handDetect::getMedian(std::vector<int> val){
  int median;
  size_t size = val.size();
  sort(val.begin(), val.end());
  if (size  % 2 == 0)  {
      median = val[size / 2 - 1] ;
  } else{
      median = val[size / 2];
  }
  return median;
}

void handDetect::getAvgColor(MyImage *m,My_ROI roi,int avg[3]){
	cv::Mat r;
	roi.roi_ptr.copyTo(r);
	std::vector<int>hm;
	std::vector<int>sm;
	std::vector<int>lm;
	// generate std::vectors
	for(int i=2; i<r.rows-2; i++){
    	for(int j=2; j<r.cols-2; j++){
    		hm.push_back(r.data[r.channels()*(r.cols*i + j) + 0]) ;
        	sm.push_back(r.data[r.channels()*(r.cols*i + j) + 1]) ;
        	lm.push_back(r.data[r.channels()*(r.cols*i + j) + 2]) ;
   		}
	}
	avg[0]=getMedian(hm);
	avg[1]=getMedian(sm);
	avg[2]=getMedian(lm);
}

void handDetect::average(MyImage *m){
	m->cap >> m->src;
	flip(m->src,m->src,1);
	for(int i=0;i<30;i++){
		m->cap >> m->src;
		flip(m->src,m->src,1);
		cvtColor(m->src,m->src,ORIGCOL2COL);
		for(int j=0;j<NSAMPLES;j++){
			getAvgColor(m,roi[j],avgColor[j]);
			roi[j].draw_rectangle(m->src);
		}	
		cvtColor(m->src,m->src,COL2ORIGCOL);
		std::string imgText = std::string("Finding average color of hand");
		printText(m->src,imgText);	
		imshow("img1", m->src);
        if(cv::waitKey(30) >= 0) break;
	}
}

void handDetect::initTrackbars(){
	for(int i=0;i<NSAMPLES;i++){
		c_lower[i][0]=12;
		c_upper[i][0]=7;
		c_lower[i][1]=30;
		c_upper[i][1]=40;
		c_lower[i][2]=80;
		c_upper[i][2]=80;
	}
	cv::createTrackbar("lower1","trackbars",&c_lower[0][0],255);
	cv::createTrackbar("lower2","trackbars",&c_lower[0][1],255);
	cv::createTrackbar("lower3","trackbars",&c_lower[0][2],255);
	cv::createTrackbar("upper1","trackbars",&c_upper[0][0],255);
	cv::createTrackbar("upper2","trackbars",&c_upper[0][1],255);
	cv::createTrackbar("upper3","trackbars",&c_upper[0][2],255);
}

void handDetect::normalizeColors(MyImage * myImage){
	// copy all boundries read from trackbar
	// to all of the different boundries
	for(int i=1;i<NSAMPLES;i++){
		for(int j=0;j<3;j++){
			c_lower[i][j]=c_lower[0][j];	
			c_upper[i][j]=c_upper[0][j];	
		}	
	}
	// normalize all boundries so that 
	// threshold is whithin 0-255
	for(int i=0;i<NSAMPLES;i++){
		if((avgColor[i][0]-c_lower[i][0]) <0){
			c_lower[i][0] = avgColor[i][0] ;
		}if((avgColor[i][1]-c_lower[i][1]) <0){
			c_lower[i][1] = avgColor[i][1] ;
		}if((avgColor[i][2]-c_lower[i][2]) <0){
			c_lower[i][2] = avgColor[i][2] ;
		}if((avgColor[i][0]+c_upper[i][0]) >255){ 
			c_upper[i][0] = 255-avgColor[i][0] ;
		}if((avgColor[i][1]+c_upper[i][1]) >255){
			c_upper[i][1] = 255-avgColor[i][1] ;
		}if((avgColor[i][2]+c_upper[i][2]) >255){
			c_upper[i][2] = 255-avgColor[i][2] ;
		}
	}
}

void handDetect::produceBinaries(MyImage *m){	
	cv::Scalar lowerBound;
	cv::Scalar upperBound;
	cv::Mat foo;
	for(int i=0;i<NSAMPLES;i++){
		normalizeColors(m);
		lowerBound=cv::Scalar( avgColor[i][0] - c_lower[i][0] , avgColor[i][1] - c_lower[i][1], avgColor[i][2] - c_lower[i][2] );
		upperBound=cv::Scalar( avgColor[i][0] + c_upper[i][0] , avgColor[i][1] + c_upper[i][1], avgColor[i][2] + c_upper[i][2] );
		m->bwList.push_back(cv::Mat(m->srcLR.rows,m->srcLR.cols,CV_8U));	
		inRange(m->srcLR,lowerBound,upperBound,m->bwList[i]);	
	}
	m->bwList[0].copyTo(m->bw);
	for(int i=1;i<NSAMPLES;i++){
		m->bw+=m->bwList[i];	
	}
	medianBlur(m->bw, m->bw,7);
}

int handDetect::findBiggestContour(std::vector<std::vector<cv::Point> > contours){
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++){
        if(contours[i].size() > sizeOfBiggestContour){
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}
cv::Mat handDetect::crop(MyImage *m, HandGesture *hg){

      cv::Rect croppin(hg->bRect.tl(),hg->bRect.br());
	  cv::Mat q=m->src(cv::Rect(hg->bRect.tl(),hg->bRect.br()));
	cv::imshow("img22",q);	
	return q;

}



void handDetect::makeContours(MyImage *m, HandGesture* hg){
	cv::Mat aBw;
	cv::pyrUp(m->bw,m->bw);
	m->bw.copyTo(aBw);
	findContours(aBw,hg->contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	hg->initVectors(); 
	hg->cIdx=findBiggestContour(hg->contours);
	if(hg->cIdx!=-1){
		hg->bRect=boundingRect(cv::Mat(hg->contours[hg->cIdx]));		
		convexHull(cv::Mat(hg->contours[hg->cIdx]),hg->hullP[hg->cIdx],false,true);
		convexHull(cv::Mat(hg->contours[hg->cIdx]),hg->hullI[hg->cIdx],false,false);
		approxPolyDP( cv::Mat(hg->hullP[hg->cIdx]), hg->hullP[hg->cIdx], 18, true );
	
		bool isHand=hg->detectIfHand();
		
	
	}
}

