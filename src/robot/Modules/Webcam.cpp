//
// Created by abdelsaidt on 2/27/20.
//

#include "Webcam.h"
#include <iostream>

#define COLORS 9

using namespace cv;

namespace PhysicalRobot {

	Webcam::Webcam(uint8_t id, Position pos) : Module{id, "Webcam"} {
		_position = pos;
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
			exit(-1);
		} else {

			uint8_t dataSetSize = 10; // Initialisation of number of shoot

			uint8_t results[4] = {0, 0, 0, 0}; // 0: none | 1: red | 2: green | 3: red and green

			// Definition of filters parameters
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
			PhysicalRobot::DetectedColor detectedColor;

			// std::vector<std::string> positions = {"normal", "reverse"};
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

				detectedColor = PhysicalRobot::DetectedColor::None;

				if(!contours.empty()) {
					if(cupFound(contours)) {
						detectedColor = PhysicalRobot::DetectedColor::Green;
					}
				}

				contours.clear();
				findContours(imgThreshRed, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

				if(!contours.empty()) {
					if(cupFound(contours)) {
						if(detectedColor == PhysicalRobot::DetectedColor::Green) {
							detectedColor = PhysicalRobot::DetectedColor::Red_and_Green;
						} else {
							detectedColor = PhysicalRobot::DetectedColor::Red;
						}
					}
				}

				logDebug1("Detected color: ", toString(detectedColor));

				switch(detectedColor) {
					case PhysicalRobot::DetectedColor::None:
						results[0]++;
						break;
					case PhysicalRobot::DetectedColor::Red:
						results[1]++;
						break;
					case PhysicalRobot::DetectedColor::Green:
						results[2]++;
						break;
					case PhysicalRobot::DetectedColor::Red_and_Green:
						results[3]++;
						break;
					default:
						logDebug1("Erreur ");
						break;
				}
			}

			uint8_t r = index_max(results, 4);

			std::cout << "Stats" << std::endl;
			std::string colors[4] = {"none", "red", "green", "red_and_green"};
			for(int i = 0; i < 4; i++) {
				std::cout << colors[i] << " :" << unsigned(results[i]) << std::endl;
			}


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

	PhysicalRobot::DetectedColors PhysicalRobot::Webcam::detectColors() const {

		VideoCapture cap(0); // capture the video from web cam

		if(!cap.isOpened()) // if not success, exit program
		{
			logDebug1("Cannot open the webcam");
			exit(-1);
		} else {

			uint8_t dataSetSize = 10; // Initialisation of number of shoot

			uint8_t results[COLORS] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // 0: none | 1: red | 2: green | 3: red and green
			                                                       // 4: red and none | 5: green and red | 6: green and
			                                                       // none 7: none and red | 8: none and green

			// Definition of filters parameters
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
			PhysicalRobot::DetectedColors detectedColors, detectedColors2;

			// std::vector<std::string> positions = {"normal", "reverse"};
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
				int colDim;

				bitwise_and(imgOriginal, imgOriginal, imgColored, imgThreshRed);
				bitwise_and(imgOriginal, imgOriginal, imgColored, imgThreshGreen);


				// imshow("Thresholded Green Image", imgThreshGreen); //show the thresholded image
				// imshow("Thresholded Red Image", imgThreshRed); //show the thresholded image

				// imshow("Colored Image", imgColored); //show the thresholded image
				// imshow("Original", imgOriginal); //show the original image

				std::vector<std::vector<Point>> contours;
				findContours(imgThreshGreen, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

				colDim = imgThreshGreen.cols;

				detectedColors = PhysicalRobot::DetectedColors::None;
				detectedColors2 = PhysicalRobot::DetectedColors::None;

				if(!contours.empty()) {
					detectedColors = cupFound(contours, colDim, DetectedColor::Green);
				}

				if(detectedColors != DetectedColors::Green) { // -> 2

					contours.clear();
					findContours(imgThreshRed, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

					if(!contours.empty()) {
						detectedColors2 = cupFound(contours, colDim, DetectedColor::Red);

						if(detectedColors2 != DetectedColors::Red) {               // -> 1
							if(detectedColors == DetectedColors::None_and_Green) { // -> 8
								if(detectedColors2 == DetectedColors::Red_and_None)
									detectedColors = DetectedColors::Red_and_Green;       // -> 3
							} else if(detectedColors == DetectedColors::Green_and_None) { // -> 6
								if(detectedColors2 == DetectedColors::None_and_Red)
									detectedColors = DetectedColors::Green_and_Red; // -> 5
							} else {                                                // None
								detectedColors = detectedColors2;
							}
						}
					}
				}


				logDebug1("Detected color: ", toString(detectedColors));

				switch(detectedColors) {
					case PhysicalRobot::DetectedColors::None:
						results[0]++;
						break;
					case PhysicalRobot::DetectedColors::Red:
						results[1]++;
						break;
					case PhysicalRobot::DetectedColors::Green:
						results[2]++;
						break;
					case PhysicalRobot::DetectedColors::Red_and_Green:
						results[3]++;
						break;
					case PhysicalRobot::DetectedColors::Red_and_None:
						results[4]++;
						break;
					case PhysicalRobot::DetectedColors::Green_and_Red:
						results[5]++;
						break;
					case PhysicalRobot::DetectedColors::Green_and_None:
						results[6]++;
						break;
					case PhysicalRobot::DetectedColors::None_and_Red:
						results[7]++;
						break;
					case PhysicalRobot::DetectedColors::None_and_Green:
						results[8]++;
						break;
					default:
						logDebug1("Erreur ");
						break;
				}
			}

			uint8_t r = index_max(results, 9);


			switch(r) {
				case 0:
					return PhysicalRobot::DetectedColors::None;
				case 1:
					return PhysicalRobot::DetectedColors::Red;
				case 2:
					return PhysicalRobot::DetectedColors::Green;
				case 3:
					return PhysicalRobot::DetectedColors::Red_and_Green;
				case 4:
					return PhysicalRobot::DetectedColors::Red_and_None;
				case 5:
					return PhysicalRobot::DetectedColors::Green_and_Red;
				case 6:
					return PhysicalRobot::DetectedColors::Green_and_None;
				case 7:
					return PhysicalRobot::DetectedColors::None_and_Red;
				case 8:
					return PhysicalRobot::DetectedColors::None_and_Green;
				default:
					logDebug1("Erreur ");
					break;
			}
		}

		exit(-1);
	}

	bool Webcam::cupFound(std::vector<std::vector<cv::Point>>& contours) const {
		Mat imgContour;
		double area;
		double cupThresholdMin = 40000; // To adjust

		auto it = contours.begin();

		bool notfound = true;

		while(it != contours.end() && notfound) {
			area = abs(cv::contourArea(*it, false));

			if(area > cupThresholdMin) {
				notfound = false;
				std::cout << "Max area: " << area << std::endl;
			} else {
				it++;
			}
		}

		return !notfound;
	}


	DetectedColors Webcam::cupFound(std::vector<std::vector<cv::Point>>& contours, int colDim, DetectedColor color) const {
		Mat imgContour;
		double area;
		double cupThresholdMin = 40000; // To adjust

		auto it = contours.begin();

		uint8_t cupfound = 0;
		int cx;
		Moments m;
		DetectedColors dC = DetectedColors::None;

		while(it != contours.end() && cupfound < 2) {
			area = abs(cv::contourArea(*it, false));

			if(area > cupThresholdMin) {
				cupfound++;
				m = cv::moments(*it);

				cx = int(m.m10 / m.m00);
				if(cx > colDim / 2) {
					if(color == DetectedColor::Red) {
						if(cupfound == 1)
							dC = DetectedColors::None_and_Red;
						else // 2
							dC = DetectedColors::Red;
					} else // green
					{
						if(cupfound == 1)
							dC = DetectedColors::None_and_Green;
						else // 2
							dC = DetectedColors::Green;
					}
				} else {
					if(color == DetectedColor::Red) {
						if(cupfound == 1)
							dC = DetectedColors::Red_and_None;
						else // 2
							dC = DetectedColors::Red;
					} else // green
					{
						if(cupfound == 1)
							dC = DetectedColors::Green_and_None;
						else // 2
							dC = DetectedColors::Green;
					}
				}

				std::cout << "Max area: " << area << std::endl;
			}
			it++;
		}

		return dC;
	}


	void Webcam::message_processing(const JSON& j) {
		if(json_has_fields(j, {"position"})) {
		}
	}

	std::vector<JSON> Webcam::generate_list_jsons() const {
		JSON webcam;
		webcam["position"] = toString(_position);
		return std::vector({webcam});
	}

	void Webcam::deactivation() {}

} // namespace PhysicalRobot