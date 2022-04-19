#include "../../src/roi.hpp"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include <iostream>

// Test Constructor1 Upper Corner
TEST(roiTest, constructor1Test_upper_corner) {
	// Arrange
	My_ROI* roi = new My_ROI();
	
	// Act
	
	
	// Assert
	cv::Point expected = cv::Point(0, 0);
	EXPECT_EQ(roi->upper_corner, expected);
	
	// Delete
	delete roi;
}

// Test Constructor1 Lower Corner
TEST(roiTest, constructor1Test_lower_corner) {
	// Arrange
	My_ROI* roi = new My_ROI();
	
	// Act
	
	
	// Assert
	cv::Point expected = cv::Point(0, 0);
	EXPECT_EQ(roi->lower_corner, expected);
	
	// Delete
	delete roi;
}

// Test Constructor2 Upper Corner
TEST(roiTest, constructor2Test_upper_corner) {
	// Arrange
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Point upper_corner = cv::Point(917, 177);
	cv::Point lower_corner = cv::Point(2321, 2393);	
	
	// Act
	My_ROI* roi = new My_ROI(upper_corner, lower_corner, src);
	
	// Assert
	cv::Point expected = cv::Point(917, 177);
	EXPECT_EQ(roi->upper_corner, expected);
	
	// Delete
	delete roi;
}

// Test Constructor2 Lower Corner
TEST(roiTest, constructor2Test_lower_corner) {
	// Arrange
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Point upper_corner = cv::Point(917, 177);
	cv::Point lower_corner = cv::Point(2321, 2393);	
	
	// Act
	My_ROI* roi = new My_ROI(upper_corner, lower_corner, src);
	
	// Assert
	cv::Point expected = cv::Point(2321, 2393);
	EXPECT_EQ(roi->lower_corner, expected);
	
	// Delete
	delete roi;
}

// Test Constructor2 Color
TEST(roiTest, constructor2Test_color) {
	// Arrange
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Point upper_corner = cv::Point(917, 177);
	cv::Point lower_corner = cv::Point(2321, 2393);	
	
	// Act
	My_ROI* roi = new My_ROI(upper_corner, lower_corner, src);
	
	// Assert
	cv::Scalar expected = cv::Scalar(0, 255, 0);
	EXPECT_EQ(roi->color, expected);
	
	// Delete
	delete roi;
}

// Test Constructor2 Border Thickness
TEST(roiTest, constructor2Test_border_thickness) {
	// Arrange
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Point upper_corner = cv::Point(917, 177);
	cv::Point lower_corner = cv::Point(2321, 2393);	
	
	// Act
	My_ROI* roi = new My_ROI(upper_corner, lower_corner, src);
	
	// Assert
	int expected = 2;
	EXPECT_EQ(roi->border_thickness, expected);
	
	// Delete
	delete roi;
}

// Test Constructor2 ROI Ptr
TEST(roiTest, constructor2Test_roi_ptr) {
	// Arrange
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Point upper_corner = cv::Point(917, 177);
	cv::Point lower_corner = cv::Point(2321, 2393);	
	cv::Mat roi_ptr_exp = src(cv::Rect(upper_corner.x, upper_corner.y, lower_corner.x-upper_corner.x,lower_corner.y-upper_corner.y));

	// Act
	My_ROI* roi = new My_ROI(upper_corner, lower_corner, src);
	
	// Assert
	EXPECT_TRUE(!cv::norm(roi->roi_ptr, roi_ptr_exp, cv::NORM_L1));
	
	// Delete
	delete roi;
}

// Test Draw Rectangle
TEST(roiTest, test_draw_rectangle) {
	// Arrange
	My_ROI* roi = new My_ROI();
	
	cv::Mat src = cv::imread("assets/src.jpg");
	cv::Mat src_exp = src.clone();
	cv::Point upper_corner = cv::Point(917, 177);
	cv::Point lower_corner = cv::Point(2321, 2393);
	cv::Scalar color = cv::Scalar(0, 255, 0);
	int border_thickness = 3;

	roi->upper_corner = upper_corner;
	roi->lower_corner = lower_corner;
	roi->color = color;
	roi->border_thickness = border_thickness;

	cv::rectangle(src_exp, upper_corner, lower_corner, color, border_thickness);
	
	// Act
	roi->draw_rectangle(src);
	
	// Assert
	EXPECT_TRUE(!cv::norm(src, src_exp, cv::NORM_L1));
	
	// Delete
	delete roi;
}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
