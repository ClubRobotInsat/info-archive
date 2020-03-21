#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// C file program
// Author: Ast
// Description:


bool cupFound(vector<vector<Point>>& contours) {

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


		vector<Point> pts = *itmx;
		cout << "MaxAreaSigned: " << mx << " NbPoint: " << itmx->size() << " = " << pts.size() << endl;
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

		cout << "DL: x=" << dl_x << " y=" << dl_y << endl;

		cout << "DR: x=" << dr_x << " y=" << dr_y << endl;

		cout << "UL: x=" << ul_x << " y=" << ul_y << endl;

		cout << "UR: x=" << ur_x << " y=" << ur_y << endl;

		if(ur_x - ul_x > dr_x - dl_x)
			cout << "Position: normal" << endl;
		else
			cout << "Position: reverse" << endl;

		/*for(auto p: *itmx){
		    //find upleft, upright, downleft, downright
		    if(p.x > )
		     cout << "x=" << p.x << " y=" << p.y << endl;
		 }*/
	}
	return res;
}

int main() {
	VideoCapture cap(0); // capture the video from web cam

	if(!cap.isOpened()) // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
		return -1;
	}

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

	string detectedColor = "";
	vector<string> positions = {"normal", "reverse"};
	while(true) {
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if(!bSuccess) // if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
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


		imshow("Thresholded Green Image", imgThreshGreen); // show the thresholded image
		imshow("Thresholded Red Image", imgThreshRed);     // show the thresholded image


		imshow("Colored Image", imgColored); // show the thresholded image
		imshow("Original", imgOriginal);     // show the original image

		vector<vector<Point>> contours;
		findContours(imgThreshGreen, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		bool green = false;
		detectedColor = "none";

		if(!contours.empty()) {
			/*
			 double max = contourArea(contours[0], true);
			 double a_mx = abs(max);
			 int imax = 0;
			 double c_mx, ca_mx;
			 int sizeCount = contours.size();
			 for(int i=1; i<sizeCount; i++){
			     c_mx = contourArea(contours[i], true);
			     ca_mx = abs(c_mx);

			     if(a_mx < ca_mx){
			         max = c_mx;
			         a_mx = ca_mx;
			         imax = i;
			     }
			 }
			 cout << "MaxAreaSigned: " << max << " Indice: " << imax  << " NbPoint: " << contours[imax].size() << endl;
		 */

			if(cupFound(contours)) {
				detectedColor = "green";
				green = true;
			}
		}

		contours.clear();
		findContours(imgThreshRed, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

		if(!contours.empty()) {
			if(cupFound(contours)) {
				if(green)
					detectedColor += " and red";
				else
					detectedColor = "red";
			}
		}

		cout << "Detected color: " << detectedColor << endl;

		if(waitKey(30) == 27) // wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	return 0;
}
