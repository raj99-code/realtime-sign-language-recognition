#pragma once

#include <opencv2/opencv.hpp>

/*
 Author: Pierfrancesco Soffritti https://github.com/PierfrancescoSoffritti
*/


class FaceDetector {
	public:
		FaceDetector(void);
		void removeFaces(cv::Mat input, cv::Mat output);
};

cv::Rect getFaceRect(cv::Mat input);

cv::String faceClassifierFileName = "../model/haarcascade_frontalface_alt.xml";
cv::CascadeClassifier faceCascadeClassifier;

FaceDetector::FaceDetector(void) {
	if (!faceCascadeClassifier.load(faceClassifierFileName))
		throw std::runtime_error("can't load file " + faceClassifierFileName);
}

void FaceDetector::removeFaces(cv::Mat input, cv::Mat output) {
	std::vector<cv::Rect> faces;
	cv::Mat frameGray;

	cv::cvtColor(input, frameGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(frameGray, frameGray);

	faceCascadeClassifier.detectMultiScale(frameGray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(120, 120));

	for (size_t i = 0; i < faces.size(); i++) {
		cv::rectangle(
			output,
			cv::Point(faces[i].x, faces[i].y),
			cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height),
			cv::Scalar(0, 0, 0),
			-1
		);
	}
}

cv::Rect getFaceRect(cv::Mat input) {
	std::vector<cv::Rect> faceRectangles;
	cv::Mat inputGray;

	cv::cvtColor(input, inputGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(inputGray, inputGray);

	faceCascadeClassifier.detectMultiScale(inputGray, faceRectangles, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(120, 120));

	if (faceRectangles.size() > 0)
		return faceRectangles[0];
	else
		return cv::Rect(0, 0, 1, 1);
}
