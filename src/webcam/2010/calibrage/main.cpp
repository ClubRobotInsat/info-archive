// main.cpp

#include "Application.h"

#include <iostream>
#include <sstream>
#include <string.h>

using namespace std;

int main(int argc, char* argv[]) {
	return Application::GetInstance()->Run(argc, argv);
}
