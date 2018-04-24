//
// Created by paul on 24/04/18.
//

#ifndef ROOT_INTERFACE_H
#define ROOT_INTERFACE_H

#include "Interface.h"
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

	uint8_t Serie::lireOctet() {
		return interface_elec_info.info_get_data();
	};

	void Serie::ecrireOctet(uint8_t octet) {
		interface_elec_info.info_put_data(octet);
	};
}


#endif // ROOT_INTERFACE_H
