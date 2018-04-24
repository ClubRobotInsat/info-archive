//
// Created by paul on 24/04/18.
//

#ifndef ROOT_MBED_H
#define ROOT_MBED_H

#include <stdarg.h>
#include <stdio.h>
#include <tgmath.h>

#include "Can.h"
#include "DigitalIn.h"
#include "DigitalOut.h"
#include "FunctionPointer.h"
#include "InterruptIn.h"
#include "PinName.h"
#include "PwmOut.h"
#include "Serial.h"


void wait_ms(int ms);

static inline void debug(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

static inline void error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
}

#endif // ROOT_MBED_H
