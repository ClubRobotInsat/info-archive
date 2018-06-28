//
// Created by paul on 24/04/18.
//

#ifndef ROOT_INTERFACE_H
#define ROOT_INTERFACE_H

#include "Interface.h"
#include "../../robot/Commun/Communication/NullCommunicator.h"
#include "../../robot/Commun/Communication/Serie.h"

void delay() {
	std::this_thread::sleep_for(std::chrono::microseconds(1));
}

uint8_t Interface::info_get_data() {
	uint8_t result = _from_elec.front();
	_from_elec.pop();
	return result;
}

uint8_t Interface::elec_get_data() {
	uint8_t result = _from_info.front();
	_from_info.pop();
	return result;
}

void Interface::info_put_data(uint8_t data) {
	{ _from_info.push(data); }
	delay();
}

void Interface::elec_put_data(uint8_t data) {
	{ _from_elec.push(data); }
	delay();
}

long Interface::queue_size_elec() {
	return _from_elec.size();
}

long Interface::queue_size_info() {
	return _from_info.size();
}

std::queue<uint8_t> Interface::_from_elec;
std::queue<uint8_t> Interface::_from_info;
std::mutex Interface::_mutex_elec;
std::mutex Interface::_mutex_info;

namespace Commun {
	// On réimplémente la classe nullcommunicator
	NullCommunicator::NullCommunicator() {}

	NullCommunicator::~NullCommunicator() {}

	void NullCommunicator::ecrireOctets(uint8_t const* data, std::size_t size) {
		std::lock_guard<std::mutex> lock(Interface::_mutex_info);
		for(int i = 0; i < size; i++) {
			interface_elec_info.info_put_data(data[i]);
		}
	}
	void NullCommunicator::lireOctets(uint8_t* octets, std::size_t nombre) {
		while(interface_elec_info.queue_size_elec() < nombre) {
		}

		std::lock_guard<std::mutex> lock(Interface::_mutex_elec);
		for(int i = 0; i < nombre; i++) {
			octets[i] = interface_elec_info.info_get_data();
		}
	}
}


#endif // ROOT_INTERFACE_H
