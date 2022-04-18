#include "../../scripts/handGesture.hpp"
#include "../../scripts/myImage.hpp"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include <iostream>

// Test Constructor Frame Number
TEST(handGesture, constructorTest_frameNumber) {
	// Arrange
	HandGesture* hand_gest = new HandGesture();
	
	// Act
	
	
	// Assert
	int expected = 0;
	EXPECT_EQ(hand_gest->frameNumber, expected);
	
	// Delete
	delete hand_gest;
}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
