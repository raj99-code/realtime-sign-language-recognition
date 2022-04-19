#include "../../src/FrameGen.hpp"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include <iostream>

// Test Constructor1 cameraSrc
TEST(frame_gen, constructor1Test_cameraSrc) {
	// Arrange
	int Camera = 2;
	
	// Act
	FrameGen* img = new FrameGen(Camera);
	
	// Assert
	int expected = 2;
	EXPECT_EQ(img->cameraSrc, expected);
	
	// Delete
	delete img;
}

// Test Constructor2_scrLR
TEST(frame_gen, constructor2Test_scrLR) {
	// Arrange
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Mat src_exp = src.clone();
	cv::Mat src_expLR;
	
	// Act
	FrameGen* img = new FrameGen(src);
	cv::flip(src_exp, src_exp, 1);
	cv::pyrDown(src_exp, src_expLR);
	cv::blur(src_expLR, src_expLR, cv::Size(3,3));
	cv::cvtColor(src_expLR, src_expLR, cv::COLOR_BGR2HLS);
	
	// Assert
	EXPECT_TRUE(!cv::norm(img->srcLR, src_expLR, cv::NORM_L1));
	
	// Delete
	delete img;
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

