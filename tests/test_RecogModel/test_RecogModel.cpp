#include "../../src/RecogModel.h"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include <iostream>

// Test LetterRecog Forward Validity
TEST(LetterRecogTest, forwardTestResultValid) {
	// Arrange
	LetterRecog* model = new LetterRecog("../../model/asl_alphabet.onnx");
	cv::Mat src = cv::imread("assets/src.jpg");

	// Act
	cv::Point2f out = model->forward(src);
	
	// Assert
	bool truth_val = ((out.x >= 0) && (out.x <= 28));
	EXPECT_TRUE(truth_val);
	
	// Delete
	delete model;
}

// Test LetterRecog Forward Correctness (may be incorrect and fail; ML model)
TEST(LetterRecogTest, forwardTest) {
	// Arrange
	LetterRecog* model = new LetterRecog("../../model/asl_alphabet.onnx");
	cv::Mat src = cv::imread("assets/src.jpg");
	int expected = 22;

	// Act
	cv::Point2f out = model->forward(src);
	
	// Assert
	EXPECT_EQ(out.x, expected);
	
	// Delete
	delete model;
}



int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
