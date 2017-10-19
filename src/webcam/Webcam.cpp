// Webcam.cpp

#include "Webcam.h"
#include "WebcamDummy.h"
#include "WebcamKinect.h"
#include "WebcamSimuIPC.h"
#include "WebcamSimuTCPIP.h"
#include "WebcamTCPIPMJPG.h"
#include "WebcamTGA.h"
#include "WebcamUVC.h"
#include "WebcamV4L.h"
#include "log/Log.h"
#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

// Fonction qui crée un nouvelle webcam en fonction de ce qui est passé en paramètre
IWebcam* NewWebcam(int argc, char* argv[], bool force_error) {
	IWebcam* webcam = NULL;
	bool initialized = false;

	for(int i = 0; i < argc; i++) {
		string s = argv[i];

		// - DUMMY
		if(s == "DUMMY") {
			webcam = new WebcamDummy();
			initialized = webcam->Init("");
			break;
		}

		// - KINECT
		else if(s == "KINECT") {
			webcam = new WebcamKinect();
			initialized = webcam->Init("");
			break;
		}

		else if((i + 1) < argc) {
			const char* device = argv[i + 1];

			// - V4L
			if(s == "V4L") {
				webcam = new WebcamV4L();
				initialized = webcam->Init(device);
				break;
			}

			// - UVC
			else if(s == "UVC") {
				webcam = new WebcamUVC();
				initialized = webcam->Init(device);
				break;
			}

			// - TCPCAM
			// 2 utilisations possibles :
			// TCPCAM ip:port
			// TCPCAM port
			else if(s == "TCPCAM") {
				webcam = new WebcamSimuTCPIP();

				string str_device = device;
				if(str_device.find(':') != string::npos)
					initialized = webcam->Init(device);
				else
					initialized = webcam->Init((string("127.0.0.1:") + str_device).c_str());
				break;
			}

			// - IPC
			else if(s == "IPC") {
				webcam = new WebcamSimuIPC();
				initialized = webcam->Init(device);
				break;
			}

			// - TCPIPMJPG
			else if(s == "TCPMJPG") {
				webcam = new WebcamTCPIPMJPG();

				string str_device = device;
				if(str_device.find(':') != string::npos)
					initialized = webcam->Init(device);
				else
					initialized = webcam->Init((string("127.0.0.1:") + str_device).c_str());
				break;
			}

			// - TGA
			// Idem, pour utiliser WebcamTGA décommentez ici et dans le sconstruct
			/*else if(s == "TGA")
			{
			    webcam = new WebcamTGA();
			    initialized = webcam->Init(device);
			    break;
			}
			*/
		}
	}

	if(!initialized)
		logError("impossible d'initialiser la webcam !");

	if((webcam == NULL || !initialized) && force_error) {
		cout << "Utilisation (webcam) :" << endl;
		cout << "- " << argv[0] << " V4L [peripherique]    (ex : " << argv[0] << " V4L /dev/video0)" << endl;
		cout << "- " << argv[0] << " UVC [peripherique]    (ex : " << argv[0] << " UVC /dev/video0)" << endl;
		cout << "- " << argv[0] << " KINECT [nb]           (ex : " << argv[0] << " KINECT .)" << endl;
		cout << "- " << argv[0] << " TCPCAM [ip]:[port]    (ex : " << argv[0] << " TCPCAM 127.0.0.1:4242)" << endl;
		cout << "- " << argv[0] << " TCPCAM [port]         (ex : " << argv[0] << " TCPCAM 4242)" << endl;
		cout << "- " << argv[0] << " IPC [nom fichier]     (deprecie)" << endl;
		cout << "- " << argv[0] << " DUMMY" << endl;
		cout << "- " << argv[0] << " TCPMJPG [ip]:[port]   (ex : " << argv[0] << " TCPMJPG 127.0.0.1:4242)" << endl;
		cout << "- " << argv[0] << " TCPMJPG [port]        (ex : " << argv[0] << " TCPMJPG 4242)" << endl;
		cout << "- " << argv[0] << " TGA [image en 24 bpp] (ex : " << argv[0] << " image.tga)" << endl;

		// exit(EXIT_FAILURE);
		webcam = NULL;
	}

	return webcam;
}
