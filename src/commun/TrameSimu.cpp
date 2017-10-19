/*
 * TrameSimu.cpp
 *
 *  Created on: 30 avr. 2015
 *      Author: seydoux
 */

#include "TrameSimu.h"

static const int TrameSimu::NB_OCTETS_CMD = 1;

TrameSimu::TrameSimu(uint8_t cmd) : _cmd(cmd) {}

TrameSimu::~TrameSimu() {
	// TODO Auto-generated destructor stub
}
