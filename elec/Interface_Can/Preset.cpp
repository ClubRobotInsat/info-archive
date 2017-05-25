//
// Created by paul on 25/05/17.
//

#include "Preset.h"
#include <iostream>


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

Preset::Preset(std::string rawPreset) {}


PresetArray::PresetArray(std::string path) {
	Json::Value json;
	std::cin >> json;

	const Json::Value array = json["presets"];
	for(int i = 0; i < array.size(); ++i) {
		_preset.push_back(Preset(array[i].asString()));
	}
}


void PresetArray::saveToFile(std::string path) {

	Json::Value json;
	for(Preset preset : _preset) {
		json["presets"] =
	}
}

void PresetArray::addPreset(Preset preset) {
	_preset.push_back(preset);
}

std::vector<Preset> PresetArray::getArray() {
	return _preset;
}
