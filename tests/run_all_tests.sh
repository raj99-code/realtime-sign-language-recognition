#!/bin/bash

echo -e "Testing process initiating\n"

echo -e "\nHand Gesture Tests\n"
cd test_hand_gesture
./test_hand_gesture
cd ..

echo -e "\nMy Image Tests\n"
cd test_myImage
./test_myImage
cd ..

echo -e "\nRecogModel Tests\n"
cd test_RecogModel
./test_RecogModel
cd ..

echo -e "\nROI Tests\n"
cd test_roi
./test_roi
cd ..

echo -e "\nAll tests executed successfully"
