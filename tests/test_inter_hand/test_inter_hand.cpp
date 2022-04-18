#include "../../src/InterHand.hpp"
#include "../../src/FrameGen.hpp"
#include "gtest/gtest.h"
#include "opencv2/opencv.hpp"
#include <iostream>

// Test Constructor Frame Number
TEST(interHand, constructorTest_frameNumber) {
	// Arrange
	InterHand* hand_gest = new InterHand();
	
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
