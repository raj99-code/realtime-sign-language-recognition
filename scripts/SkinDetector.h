#pragma once

#include <opencv2/opencv.hpp>

/*
 Author: Pierfrancesco Soffritti https://github.com/PierfrancescoSoffritti
*/

class SkinDetector {
	public:
		SkinDetector(void);

		void drawSkinColorSampler(cv::Mat input);
		void calibrate(cv::Mat input);
		cv::Mat getSkinMask(cv::Mat input);

	private:
		int hLowThreshold = 0;
		int hHighThreshold = 0;
		int sLowThreshold = 0;
		int sHighThreshold = 0;
		int vLowThreshold = 0;
		int vHighThreshold = 0;

		bool calibrated = false;

		cv::Rect skinColorSamplerRectangle1, skinColorSamplerRectangle2;

		void calculateThresholds(cv::Mat sample1, cv::Mat sample2);
		void performOpening(cv::Mat binaryImage, int structuralElementShapde, cv::Point structuralElementSize);
};


SkinDetector::SkinDetector(void) {
	hLowThreshold = 0;
	hHighThreshold = 0;
	sLowThreshold = 0;
	sHighThreshold = 0;
	vLowThreshold = 0;
	vHighThreshold = 0;

	calibrated = false;

	skinColorSamplerRectangle1, skinColorSamplerRectangle2;
}

void SkinDetector::drawSkinColorSampler(cv::Mat input) {
	int frameWidth = input.size().width, frameHeight = input.size().height;

	int rectangleSize = 20;
	cv::Scalar rectangleColor = cv::Scalar(255, 0, 255);

	skinColorSamplerRectangle1 = cv::Rect(frameWidth / 5, frameHeight / 2, rectangleSize, rectangleSize);
	skinColorSamplerRectangle2 = cv::Rect(frameWidth / 5, frameHeight / 3, rectangleSize, rectangleSize);

	cv::rectangle(
		input,
		skinColorSamplerRectangle1,
		rectangleColor
	);

	cv::rectangle(
		input,
		skinColorSamplerRectangle2,
		rectangleColor
	);
}

void SkinDetector::calibrate(cv::Mat input) {
	
	cv::Mat hsvInput;
	cv::cvtColor(input, hsvInput, cv::COLOR_BGR2HSV);

	cv::Mat sample1 = cv::Mat(hsvInput, skinColorSamplerRectangle1);
	cv::Mat sample2 = cv::Mat(hsvInput, skinColorSamplerRectangle2);

	calculateThresholds(sample1, sample2);

	calibrated = true;
}

void SkinDetector::calculateThresholds(cv::Mat sample1, cv::Mat sample2) {
	int offsetLowThreshold = 80;
	int offsetHighThreshold = 30;

	cv::Scalar hsvMeansSample1 = mean(sample1);
	cv::Scalar hsvMeansSample2 = mean(sample2);

	hLowThreshold = std::min(hsvMeansSample1[0], hsvMeansSample2[0]) - offsetLowThreshold;
	hHighThreshold = std::max(hsvMeansSample1[0], hsvMeansSample2[0]) + offsetHighThreshold;

	sLowThreshold = std::min(hsvMeansSample1[1], hsvMeansSample2[1]) - offsetLowThreshold;
	sHighThreshold = std::max(hsvMeansSample1[1], hsvMeansSample2[1]) + offsetHighThreshold;

	// the V channel shouldn't be used. By ignorint it, shadows on the hand wouldn't interfire with segmentation.
	// Unfortunately there's a bug somewhere and not using the V channel causes some problem. This shouldn't be too hard to fix.
	vLowThreshold = std::min(hsvMeansSample1[2], hsvMeansSample2[2]) - offsetLowThreshold;
	vHighThreshold = std::max(hsvMeansSample1[2], hsvMeansSample2[2]) + offsetHighThreshold;
	//vLowThreshold = 0;
	//vHighThreshold = 255;
}

cv::Mat SkinDetector::getSkinMask(cv::Mat input) {
	cv::Mat skinMask;

	if (!calibrated) {
		skinMask = cv::Mat::zeros(input.size(), CV_8UC1);
		return skinMask;
	}

	cv::Mat hsvInput;
	cv::cvtColor(input, hsvInput, cv::COLOR_BGR2HSV);

	inRange(
		hsvInput,
		cv::Scalar(hLowThreshold, sLowThreshold, vLowThreshold),
		cv::Scalar(hHighThreshold, sHighThreshold, vHighThreshold),
		skinMask);

	performOpening(skinMask, cv::MORPH_ELLIPSE, { 3, 3 });
	cv::dilate(skinMask, skinMask, cv::Mat(), cv::Point(-1, -1), 3);

	return skinMask;
}

void SkinDetector::performOpening(cv::Mat binaryImage, int kernelShape, cv::Point kernelSize) {
	cv::Mat structuringElement = cv::getStructuringElement(kernelShape, kernelSize);
	cv::morphologyEx(binaryImage, binaryImage, cv::MORPH_OPEN, structuringElement);
}
