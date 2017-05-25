//
// Created by paul on 25/05/17.
//

#ifndef ROOT_PRESET_H
#define ROOT_PRESET_H


#include <cstdint>
#include <string>
#include <vector>

/**
 * This class represent a preset for a can trame.
 */
class Preset {

	Preset(uint8_t id, uint8_t cmd, uint8_t data[6]);

	std::string toString();


private:
	uint8_t _id;
	uint8_t _cmd;
	uint8_t _data[6];
};


/**
 * This class represent an array of presets for a can trame.
 */
class PresetArray {

public:
	PresetArray(std::string path);

	std::vector<Preset> getArray();

private:
	std::vector<Preset> _preset;
};


#endif // ROOT_PRESET_H
