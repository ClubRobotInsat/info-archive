//
// Created by paul on 24/04/18.
//

#include <iostream>

void wait_ms(int ms) {
	std::cout << "Attente de " << ms << "ms." << std::endl;
}

void mbed_assert_internal(const char* expr, const char* file, int line) {
	printf("mbed assertation failed: %s, file: %s, line %d \n", expr, file, line);
	exit(1);
}