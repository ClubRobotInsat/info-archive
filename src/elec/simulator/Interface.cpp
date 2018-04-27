//
// Created by paul on 24/04/18.
//

#ifndef ROOT_INTERFACE_H
#define ROOT_INTERFACE_H

#include "Interface.h"
#include "../../robot/Commun/NullCommunicator.h"
#include "../../robot/Commun/Serie.h"

uint8_t Interface::info_get_data() {
	std::lock_guard<std::mutex> lock(_mutex_elec);
	uint8_t result = _from_elec.front();
	_from_elec.pop();
	return result;
}

uint8_t Interface::elec_get_data() {
	std::lock_guard<std::mutex> lock(_mutex_info);
	uint8_t result = _from_info.front();
	_from_info.pop();
	return result;
}

void Interface::info_put_data(uint8_t data) {
	std::lock_guard<std::mutex> lock(_mutex_info);
	_from_info.emplace(data);
}

void Interface::elec_put_data(uint8_t data) {
	std::lock_guard<std::mutex> lock(_mutex_elec);
	_from_elec.emplace(data);
}

long Interface::queue_size_elec() const {
	return _from_elec.size();
}

long Interface::queue_size_info() const {
	return _from_info.size();
}

namespace Commun {
	// On réimplémente la classe nullcommunicator
	NullCommunicator::NullCommunicator() {}

	NullCommunicator::~NullCommunicator() {}

	void NullCommunicator::ecrireOctets(uint8_t const* data, std::size_t size) {
		for(int i = 0; i < size; i++) {
			interface_elec_info.info_put_data(data[i]);
		}
	}
	void NullCommunicator::lireOctets(uint8_t* octets, std::size_t nombre) {
		while(interface_elec_info.queue_size_elec() < nombre)
			;
		for(int i = 0; i < nombre; i++) {
			octets[i] = interface_elec_info.info_get_data();
		}
	}
}


#endif // ROOT_INTERFACE_H
