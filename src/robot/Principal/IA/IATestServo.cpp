//
// Created by terae on 3/22/18.
//

#include "IATestServo.h"

int main(int argc, char** argv) {
	logDebug("Usage: ", argv[0], " -p <port> -t <tempo in ms>");
	std::string port{"/dev/ttyACM0"};
	Duration tempo = 100_ms;
	int i = 1;
	while(i < argc) {
		if(std::string(argv[i]) == "-t") {
			try {
				tempo = Duration::makeFromMs(std::stoi(argv[++i]));
			} catch(...) {
			}
		} else if(std::string(argv[i]) == "-p") {
			try {
				port = argv[++i];
			} catch(...) {
			}
		} else {
			++i;
		}
	}

	IATestServo strategy(port, tempo);
	strategy.executer();
}
