//
// Created by paul on 25/05/17.
//

#include "Preset.h"


Preset::Preset(uint8_t id, uint8_t cmd, uint8_t* data)
        : _id(id)
        , _cmd(cmd)

{
	for(int i = 0; i < 6; i++) {
		_data[i] = data[i];
	}
}

std::string Preset::toString() {
	return std::__cxx11::string();
}


PresetArray::PresetArray(std::string path) {}


std::vector<Preset> PresetArray::getArray() {
	return std::vector<Preset>();
}
