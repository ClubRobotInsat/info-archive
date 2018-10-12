#include "EmbeddedFiles.h"

#include <fstream>

// Uncomment this to read the file directly on the
// computer (and not the embedded version)
// #define READ_EXTERNAL_FILES

std::map<std::string, std::string> EmbeddedFiles::_files{
#ifndef READ_EXTERNAL_FILES
#include "embedded"
#endif
};

const std::string& EmbeddedFiles::readText(const std::string& path) {
	auto it = _files.find(path);

	if(it != _files.end()) {
		return it->second;
	} else {
#ifdef READ_EXTERNAL_FILES
		std::ifstream file(path);

		// TODO Check exception safety
		if(file.good()) {
			std::string contents;
			file.seekg(0, std::ios::end);
			contents.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&contents[0], contents.size());
			file.close();

			return _files[path] = contents;
		}
#endif
		return _files[path] = "";
	}
}