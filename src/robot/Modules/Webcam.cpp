//
// Created by abdelsaidt on 2/27/20.
//

#include "Webcam.h"
#include <iostream>
#include <opencv2/opencv.hpp>

namespace PhysicalRobot {

	Webcam::Webcam(uint8_t id, Position position) : Module(id, "Webcam"), _position(position) {
		_decision_time = 2_s;
	}

	void Webcam::setPosition(Position position) {
		if(position != _position) {
			_position = position;
		}
	}

	uint8_t index_max(uint8_t arr[], uint8_t s) {
		uint8_t max = arr[0], i_max = 0;
		for(int i = 1; i < s; i++) {
			if(max < arr[i]) {
				i_max = i;
				max = arr[i];
			}
		}
		return i_max;
	}

	PhysicalRobot::DetectedColor PhysicalRobot::Webcam::detectColor() const {

		VideoCapture cap(0); // capture the video from web cam

		if(!cap.isOpened()) // if not success, exit program
		{
			logDebug1("Cannot open the webcam");

		} else {

			uint8_t dataSetSize = 10;
			uint8_t results[4] = {0, 0, 0, 0}; // 0: none | 1: red | 2: green | 3: red and green

			int iLowGreenH = 60;
			int iHighGreenH = 132;

			int iLowGreenS = 120;
			int iHighGreenS = 255;

			int iLowGreenV = 0; // 165 -> 0
			int iHighGreenV = 255;

			int iLowRedH = 123;
			int iHighRedH = 179;

			int iLowRedS = 131;
			int iHighRedS = 255;

			int iLowRedV = 116;
			int iHighRedV = 255;

			Scalar greenLow = Scalar(iLowGreenH, iLowGreenS, iLowGreenV);
			Scalar greenHigh = Scalar(iHighGreenH, iHighGreenS, iHighGreenV);

			Scalar redLow = Scalar(iLowRedH, iLowRedS, iLowRedV);
			Scalar redHigh = Scalar(iHighRedH, iHighRedS, iHighRedV);
			std::string detectedColor = "";

			std::vector<std::string> positions = {"normal", "reverse"};
			while(dataSetSize-- > 0) {
				Mat imgOriginal;

				bool bSuccess = cap.read(imgOriginal); // read a new frame from video

				if(!bSuccess) // if not success, break loop
				{
					logDebug1("Cannot read a frame from video stream");
					break;
				}

				Mat imgHSV;

				cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); // Convert the captured frame from BGR to HSV

				Mat imgThreshGreen, imgThreshRed;

				inRange(imgHSV, greenLow, greenHigh, imgThreshGreen); // Threshold the image
				inRange(imgHSV, redLow, redHigh, imgThreshRed);       // Threshold the image


				// morphological opening (remove small objects from the foreground)
				erode(imgThreshGreen, imgThreshGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				dilate(imgThreshGreen, imgThreshGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				// morphological closing (fill small holes in the foreground)
				dilate(imgThreshGreen, imgThreshGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				erode(imgThreshGreen, imgThreshGreen, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));


				// morphological opening (remove small objects from the foreground)
				erode(imgThreshRed, imgThreshRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				dilate(imgThreshRed, imgThreshRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				// morphological closing (fill small holes in the foreground)
				dilate(imgThreshRed, imgThreshRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				erode(imgThreshRed, imgThreshRed, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				Mat imgColored;

				bitwise_and(imgOriginal, imgOriginal, imgColored, imgThreshRed);
				bitwise_and(imgOriginal, imgOriginal, imgColored, imgThreshGreen);


				// imshow("Thresholded Green Image", imgThreshGreen); //show the thresholded image
				// imshow("Thresholded Red Image", imgThreshRed); //show the thresholded image

				// imshow("Colored Image", imgColored); //show the thresholded image
				// imshow("Original", imgOriginal); //show the original image

				std::vector<std::vector<Point>> contours;
				findContours(imgThreshGreen, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

				bool green = false;
				detectedColor = "none";
				results[0]++;

				if(!contours.empty()) {
					if(cupFound(contours)) {
						green = true;
						results[0]--;
						results[2]++;
					}
				}

				contours.clear();
				findContours(imgThreshRed, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

				if(!contours.empty()) {
					if(cupFound(contours)) {
						if(green) {
							detectedColor += " and red";
							results[0]--;
							results[3]++;
						} else {
							detectedColor = "red";
							results[0]--;
							results[1]++;
						}
					}
				}

				logDebug1("Detected color: ", detectedColor);

				if(waitKey(30) == 27) // wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
				{
					logDebug1("esc key is pressed by user");
					break;
				}
			}


			uint8_t r = index_max(results, 4);

			if(r == 1) {
				logDebug1("Detected color (final): Red");
				return DetectedColor::Red;
			} else if(r == 2) {
				logDebug1("Detected color (final): Green");
				return DetectedColor::Green;
			} else if(r == 3) {
				logDebug1("Detected color (final): Red_and_Green");
				return DetectedColor::Red_and_Green;
			} else {
				logDebug1("Detected color (final): None");
				return DetectedColor::None;
			}
		}
	}


	bool Webcam::cupFound(std::vector<std::vector<Point>>& contours) const {
		Mat imgContour;
		double c_mx, ca_mx, mx, a_mx;

		double cupThresholdMin = 40000, cupThresholdMax = 75000; // To adjust

		auto it = contours.begin();
		c_mx = contourArea(*it, true);
		ca_mx = abs(c_mx);
		auto itmx = it;
		int indx = -1;

		bool res;

		if(ca_mx < cupThresholdMin) {
			it = contours.erase(it);
		} else {
			if(ca_mx > a_mx) {
				mx = c_mx;
				a_mx = ca_mx;
				itmx = it;
				indx++;
			}
			it++;
		}

		while(it != contours.end()) {
			c_mx = contourArea(*it, true);
			ca_mx = abs(c_mx);

			if(ca_mx < cupThresholdMin) {
				it = contours.erase(it);
			} else {
				if(ca_mx > a_mx) {
					mx = c_mx;
					a_mx = ca_mx;
					itmx = it;
					indx++;
				}
				it++;
			}
		}

		res = !contours.empty();
		if(res) {

			// drawContours(imgContour, contours, indx, Scalar(359, 0, 100));
			// imshow("Contours ", imgContour); //show the thresholded image


			std::vector<Point> pts = *itmx;
			std::cout << "MaxAreaSigned: " << mx << " NbPoint: " << itmx->size() << " = " << pts.size() << std::endl;
			double ul_x, ul_y, ur_x, ur_y, dl_x, dl_y, dr_x, dr_y;


			ul_x = pts[0].x;
			ul_y = pts[0].y;

			ur_x = pts[0].x;
			ur_y = pts[0].y;

			dl_x = pts[0].x;
			dl_y = pts[0].y;

			dr_x = pts[0].x;
			dr_y = pts[0].y;


			for(int i = 1; i < pts.size(); i++) {
				// upleft
				if(pts[i].x <= ul_x && pts[i].y >= ul_y) {
					ul_x = pts[i].x;
					ul_y = pts[i].y;
				}

				// upright
				if(pts[i].x >= ur_x && pts[i].y >= ur_y) {
					ur_x = pts[i].x;
					ur_y = pts[i].y;
				}

				// downright
				if(pts[i].x >= dr_x && pts[i].y <= dr_y) {
					dr_x = pts[i].x;
					dr_y = pts[i].y;
				}

				// downleft
				if(pts[i].x <= dl_x && pts[i].y <= dl_y) {
					dl_x = pts[i].x;
					dl_y = pts[i].y;
				}
			}

			std::cout << "DL: x=" << dl_x << " y=" << dl_y << std::endl;

			std::cout << "DR: x=" << dr_x << " y=" << dr_y << std::endl;

			std::cout << "UL: x=" << ul_x << " y=" << ul_y << std::endl;

			std::cout << "UR: x=" << ur_x << " y=" << ur_y << std::endl;

			if(ur_x - ul_x > dr_x - dl_x)
				std::cout << "Position: normal" << std::endl;
			else
				std::cout << "Position: reverse" << std::endl;

			/*for(auto p: *itmx){
			    //find upleft, upright, downleft, downright
			    if(p.x > )
			     cout << "x=" << p.x << " y=" << p.y << endl;
			 }*/
		}
		return res;
	}


	void Webcam::message_processing(const JSON& j) {
		if(json_has_fields(j, {"position"})) {
		}
	}

	std::vector<JSON> Webcam::generate_list_jsons() const {
		JSON webcam;
		webcam["position"] = toString(_position.load());
		return std::vector({webcam});
	}

	void Webcam::deactivation() {}

} // namespace PhysicalRobot