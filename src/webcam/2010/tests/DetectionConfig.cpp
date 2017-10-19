// DetectionConfig.cpp (2010)

#include "ImageFiltree.h"
#include "Webcam2010.h"
#include "log/Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;


int main(int argc, char* argv[]) {
	Webcam2010* web = new Webcam2010();
	web->Init(argc, argv, true);

	while(1) {
		web->ExtraireConfigurations();

		cout << "config lateraux : " << web->GetConfigLateraux() << endl;
		cout << "config centraux : " << web->GetConfigCentraux() << endl;

		//		getchar();
	}

	delete web;

	return 0;
}
