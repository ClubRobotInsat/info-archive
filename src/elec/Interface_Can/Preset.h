//
// Created by paul on 25/05/17.
//

#ifndef ROOT_PRESET_H
#define ROOT_PRESET_H


#include <cstdint>
#include <json.hpp>
#include <string>
#include <vector>


/**
 * This class represent a preset for a can trame.
 */
class Preset {
public:
	Preset(uint8_t id, uint8_t cmd, uint8_t data[6]);

	/**
	 * Build a preset for a raw string representing a preset.
	 * @param rawPreset : the string that represent the trame.
	 */
	Preset(std::string rawPreset);

	/**
	 * This function return a representable string of a Trame.
	 * @return a string representing a Trame.
	 */
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
	/**
	 * This function will initialize the array from the file specified by path.
	 * @param path : the file where all the presets are loaded.
	 */
	PresetArray(std::string path);

	/**
	 * This function will save the preset to the given file.
	 * @param path : the path of a file where to save the preset.
	 */
	void saveToFile(std::string path);

	/**
	 * This function will add the given preset to the list of preset
	 * @param preset : the preset to add to the list.
	 */
	void addPreset(Preset preset);

	/**
	 * @return the internal trame array.
	 */
	std::vector<Preset> getArray();

private:
	std::vector<Preset> _preset;
};

#endif // ROOT_PRESET_H
