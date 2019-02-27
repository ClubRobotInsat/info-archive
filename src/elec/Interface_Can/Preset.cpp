//
// Created by paul on 25/05/17.
//

#include "Preset.h"
#include <fstream>
#include <iostream>

using JSON = nlohmann::json;

Preset::Preset(uint8_t id, uint8_t cmd, uint8_t* data)
        : _id(id)
        , _cmd(cmd)

{
	for(int i = 0; i < 6; i++) {
		_data[i] = data[i];
	}
}

std::string Preset::toString() {
	std::string result = std::to_string(_id) + " | " + std::to_string(_cmd);
	for(auto data : _data) {
		result += " | " + std::to_string(data);
	}
	return result;
}

Preset::Preset(std::string /*rawPreset*/) {}


PresetArray::PresetArray(std::string path) {
	JSON json;
	std::ifstream input(path);

	if(!(input >> json)) {
		throw std::runtime_error("Error while reading the JSON");
	} else {
		const JSON array = json["presets"];
		for(int i = 0; i < array.size(); ++i) {
			_preset.push_back(Preset(array[i].get<std::string>()));
		}
	}
	input.close();
}


void PresetArray::saveToFile(std::string path) {
	JSON result;
	result["presets"] = JSON(JSON::array());

	// Populate the array with strings representing a preset.
	for(Preset preset : _preset) {
		result.push_back(JSON(preset.toString()));
	}

	// Creating the final string and writing it to a file
	std::ofstream outputStream(path);
	outputStream << result;
	std::cout << result << std::endl;
}

void PresetArray::addPreset(Preset preset) {
	_preset.push_back(preset);
}

std::vector<Preset> PresetArray::getArray() {
	return _preset;
}
