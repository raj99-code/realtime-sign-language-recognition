#!/bin/bash

echo -e "Testing process initiating\n"

echo -e "\nInter Hand Tests\n"
cd test_inter_hand
./test_inter_hand
cd ..

echo -e "\nFrame Gen Tests\n"
cd test_frame_gen
./test_frame_gen
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
