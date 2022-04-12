#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


/*
 Author: Nicol� Castellazzi https://github.com/nicast
*/

#define LIMIT_ANGLE_SUP 60
#define LIMIT_ANGLE_INF 5
#define BOUNDING_RECT_FINGER_SIZE_SCALING 0.3
#define BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING 0.05

class FingerCount {
	public:
		FingerCount(void);
		cv::Mat findFingersCount(cv::Mat input_image, cv::Mat frame);
		cv::Rect getBoundingRect();
		int getBoundingRectArea();
	
	private:
		cv::Scalar color_blue;
		cv::Scalar color_green;
		cv::Scalar color_red;
		cv::Scalar color_black;
		cv::Scalar color_white;
		cv::Scalar color_yellow;
		cv::Scalar color_purple;
		cv::Rect bounding_rect;
		double findPointsDistance(cv::Point a, cv::Point b);
		std::vector<cv::Point> compactOnNeighborhoodMedian(std::vector<cv::Point> points, double max_neighbor_distance);
		double findAngle(cv::Point a, cv::Point b, cv::Point c);
		bool isFinger(cv::Point a, cv::Point b, cv::Point c, double limit_angle_inf, double limit_angle_sup, cv::Point palm_center, double distance_from_palm_tollerance);
		std::vector<cv::Point> findClosestOnX(std::vector<cv::Point> points, cv::Point pivot);
		double findPointsDistanceOnX(cv::Point a, cv::Point b);
		void drawVectorPoints(cv::Mat image, std::vector<cv::Point> points, cv::Scalar color, bool with_numbers);
};

FingerCount::FingerCount(void) {
	color_blue = cv::Scalar(255, 0, 0);
	color_green = cv::Scalar(0, 255, 0);
	color_red = cv::Scalar(0, 0, 255);
	color_black = cv::Scalar(0, 0, 0);
	color_white = cv::Scalar(255, 255, 255);
	color_yellow = cv::Scalar(0, 255, 255);
	color_purple = cv::Scalar(255, 0, 255);
}

cv::Mat FingerCount::findFingersCount(cv::Mat input_image, cv::Mat frame) {
	cv::Mat contours_image = cv::Mat::zeros(input_image.size(), CV_8UC3);

	// check if the source image is good
	if (input_image.empty())
		return contours_image;

	// we work only on the 1 channel result, since this function is called inside a loop we are not sure that this is always the case
	if (input_image.channels() != 1)
		return contours_image;

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	findContours(input_image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	// we need at least one contour to work
	if (contours.size() <= 0)
		return contours_image;

	// find the biggest contour (let's suppose it's our hand)
	int biggest_contour_index = -1;
	double biggest_area = 0.0;

	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i], false);
		if (area > biggest_area) {
			biggest_area = area;
			biggest_contour_index = i;
		}
	}

	if (biggest_contour_index < 0)
		return contours_image;

	// find the convex hull object for each contour and the defects, two different data structure are needed by the OpenCV api
	std::vector<cv::Point> hull_points;
	std::vector<int> hull_ints;

	// for drawing the convex hull and for finding the bounding rectangle
	convexHull(cv::Mat(contours[biggest_contour_index]), hull_points, true);

	// for finding the defects
	convexHull(cv::Mat(contours[biggest_contour_index]), hull_ints, false);

	// we need at least 3 points to find the defects
	std::vector<cv::Vec4i> defects;
	if (hull_ints.size() > 3)
		convexityDefects(cv::Mat(contours[biggest_contour_index]), hull_ints, defects);
	else
		return contours_image;

	// we bound the convex hull
	cv::Rect bounding_rectangle = boundingRect(cv::Mat(hull_points));

	// we find the center of the bounding rectangle, this should approximately also be the center of the hand
	cv::Point center_bounding_rect(
		(bounding_rectangle.tl().x + bounding_rectangle.br().x) / 2,
		(bounding_rectangle.tl().y + bounding_rectangle.br().y) / 2
	);

	// we separate the defects keeping only the ones of intrest
	std::vector<cv::Point> start_points;
	std::vector<cv::Point> far_points;

	for (int i = 0; i < defects.size(); i++) {
		start_points.push_back(contours[biggest_contour_index][defects[i].val[0]]);

		// filtering the far point based on the distance from the center of the bounding rectangle
		if (findPointsDistance(contours[biggest_contour_index][defects[i].val[2]], center_bounding_rect) < bounding_rectangle.height * BOUNDING_RECT_FINGER_SIZE_SCALING)
			far_points.push_back(contours[biggest_contour_index][defects[i].val[2]]);
	}

	// we compact them on their medians
	std::vector<cv::Point> filtered_start_points = compactOnNeighborhoodMedian(start_points, bounding_rectangle.height * BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING);
	std::vector<cv::Point> filtered_far_points = compactOnNeighborhoodMedian(far_points, bounding_rectangle.height * BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING);

	// now we try to find the fingers
	std::vector<cv::Point> filtered_finger_points;

	if (filtered_far_points.size() > 1) {
		std::vector<cv::Point> finger_points;
		
		for (int i = 0; i < filtered_start_points.size(); i++) {
			std::vector<cv::Point> closest_points = findClosestOnX(filtered_far_points, filtered_start_points[i]);
			
			if (isFinger(closest_points[0], filtered_start_points[i], closest_points[1], LIMIT_ANGLE_INF, LIMIT_ANGLE_SUP, center_bounding_rect, bounding_rectangle.height * BOUNDING_RECT_FINGER_SIZE_SCALING))
				finger_points.push_back(filtered_start_points[i]);
		}

		if (finger_points.size() > 0) {

			// we have at most five fingers usually :)
			while (finger_points.size() > 5)
				finger_points.pop_back();

			// filter out the points too close to each other
			for (int i = 0; i < finger_points.size() - 1; i++) {
				if (findPointsDistanceOnX(finger_points[i], finger_points[i + 1]) > bounding_rectangle.height * BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING * 1.5)
					filtered_finger_points.push_back(finger_points[i]);
			}

			if (finger_points.size() > 2) {
				if (findPointsDistanceOnX(finger_points[0], finger_points[finger_points.size() - 1]) > bounding_rectangle.height * BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING * 1.5)
					filtered_finger_points.push_back(finger_points[finger_points.size() - 1]);
			}
			else
				filtered_finger_points.push_back(finger_points[finger_points.size() - 1]);
		}
	}
	
	// we draw what found on the returned image 
	cv::drawContours(contours_image, contours, biggest_contour_index, color_green, 2, 8, hierarchy);
	cv::polylines(contours_image, hull_points, true, color_blue);
	cv::rectangle(contours_image, bounding_rectangle.tl(), bounding_rectangle.br(), color_red, 2, 8, 0);
	cv::circle(contours_image, center_bounding_rect, 5, color_purple, 2, 8);
	drawVectorPoints(contours_image, filtered_start_points, color_blue, true);
	drawVectorPoints(contours_image, filtered_far_points, color_red, true);
	drawVectorPoints(contours_image, filtered_finger_points, color_yellow, false);
	putText(contours_image, std::to_string(filtered_finger_points.size()), center_bounding_rect, cv::FONT_HERSHEY_PLAIN, 3, color_purple);

	// and on the starting frame
	cv::drawContours(frame, contours, biggest_contour_index, color_green, 2, 8, hierarchy);
	cv::circle(frame, center_bounding_rect, 5, color_purple, 2, 8);
	drawVectorPoints(frame, filtered_finger_points, color_yellow, false);
	cv::putText(frame, std::to_string(filtered_finger_points.size()), center_bounding_rect, cv::FONT_HERSHEY_PLAIN, 3, color_purple);

	this->bounding_rect = bounding_rectangle;

	return contours_image;
}

double FingerCount::findPointsDistance(cv::Point a, cv::Point b) {
	cv::Point difference = a - b;
	return sqrt(difference.ddot(difference));
}

std::vector<cv::Point> FingerCount::compactOnNeighborhoodMedian(std::vector<cv::Point> points, double max_neighbor_distance) {
	std::vector<cv::Point> median_points;
	
	if (points.size() == 0)		
		return median_points;

	if (max_neighbor_distance <= 0)
		return median_points;

	// we start with the first point
	cv::Point reference = points[0];
	cv::Point median = points[0];

	for (int i = 1; i < points.size(); i++) {
		if (findPointsDistance(reference, points[i]) > max_neighbor_distance) {
			
			// the point is not in range, we save the median
			median_points.push_back(median);

			// we swap the reference
			reference = points[i];
			median = points[i];
		}
		else
			median = (points[i] + median) / 2;
	}

	// last median
	median_points.push_back(median);

	return median_points;
}

double FingerCount::findAngle(cv::Point a, cv::Point b, cv::Point c) {
	double ab = findPointsDistance(a, b);
	double bc = findPointsDistance(b, c);
	double ac = findPointsDistance(a, c);
	return std::acos((ab * ab + bc * bc - ac * ac) / (2 * ab * bc)) * 180 / CV_PI;
}

bool FingerCount::isFinger(cv::Point a, cv::Point b, cv::Point c, double limit_angle_inf, double limit_angle_sup, cv::Point palm_center, double min_distance_from_palm) {
	double angle = findAngle(a, b, c);
	if (angle > limit_angle_sup || angle < limit_angle_inf)
		return false;

	// the finger point sohould not be under the two far points
	int delta_y_1 = b.y - a.y;
	int delta_y_2 = b.y - c.y;
	if (delta_y_1 > 0 && delta_y_2 > 0)
		return false;

	// the two far points should not be both under the center of the hand
	int delta_y_3 = palm_center.y - a.y;
	int delta_y_4 = palm_center.y - c.y;
	if (delta_y_3 < 0 && delta_y_4 < 0)
		return false;

	double distance_from_palm = findPointsDistance(b, palm_center);
	if (distance_from_palm < min_distance_from_palm)
		return false;
	
	// this should be the case when no fingers are up
	double distance_from_palm_far_1 = findPointsDistance(a, palm_center);
	double distance_from_palm_far_2 = findPointsDistance(c, palm_center);
	if (distance_from_palm_far_1 < min_distance_from_palm / 4 || distance_from_palm_far_2 < min_distance_from_palm / 4)
		return false;

	return true;
}

std::vector<cv::Point> FingerCount::findClosestOnX(std::vector<cv::Point> points, cv::Point pivot) {
	std::vector<cv::Point> to_return(2);

	if (points.size() == 0)
		return to_return;

	double distance_x_1 = DBL_MAX;
	double distance_1 = DBL_MAX;
	double distance_x_2 = DBL_MAX;
	double distance_2 = DBL_MAX;
	int index_found = 0;

	for (int i = 0; i < points.size(); i++) {
		double distance_x = findPointsDistanceOnX(pivot, points[i]);
		double distance = findPointsDistance(pivot, points[i]);

		if (distance_x < distance_x_1 && distance_x != 0 && distance <= distance_1) {
			distance_x_1 = distance_x;
			distance_1 = distance;
			index_found = i;
		}
	}

	to_return[0] = points[index_found];

	for (int i = 0; i < points.size(); i++) {
		double distance_x = findPointsDistanceOnX(pivot, points[i]);
		double distance = findPointsDistance(pivot, points[i]);

		if (distance_x < distance_x_2 && distance_x != 0 && distance <= distance_2 && distance_x != distance_x_1) {
			distance_x_2 = distance_x;
			distance_2 = distance;
			index_found = i;
		}
	}

	to_return[1] = points[index_found];

	return to_return;
}

double FingerCount::findPointsDistanceOnX(cv::Point a, cv::Point b) {
	double to_return = 0.0;

	if (a.x > b.x)
		to_return = a.x - b.x;
	else
		to_return = b.x - a.x;

	return to_return;
}

void FingerCount::drawVectorPoints(cv::Mat image, std::vector<cv::Point> points, cv::Scalar color, bool with_numbers) {
	for (int i = 0; i < points.size(); i++) {
		cv::circle(image, points[i], 5, color, 2, 8);
		if(with_numbers)
			cv::putText(image, std::to_string(i), points[i], cv::FONT_HERSHEY_PLAIN, 3, color);
	}
}

cv::Rect FingerCount::getBoundingRect() {
	return this->bounding_rect;
}

int FingerCount::getBoundingRectArea() {
	cv::Point2i br_tl = this->bounding_rect.tl(); 
	cv::Point2i br_br = this->bounding_rect.br();
	return std::abs((br_tl.x-br_br.x) * (br_tl.y-br_br.y));
	
}
