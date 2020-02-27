#include "EmbeddedFiles.h"
#include "Constants.h"

#include <fstream>

// Uncomment this to read the file directly on the
// computer (and not the embedded version)
//
//#define READ_EXTERNAL_FILES

const std::string& EmbeddedFiles::readText(const std::string& path) {
	auto it = files().find(path);

	//for(auto it: files())
	    //std::cout << it.first << std::endl;

	if(it != files().end()) {
		return it->second;
	} else {
#ifdef READ_EXTERNAL_FILES
	   /* std::string configFolderPath = GLOBAL_CONSTANTS().get;
	    std::string confPath;
	    confPath = configFolderPath + path;
		std::ifstream file(confPath);*/
	   std::ifstream file(path);

		// TODO Check exception safety
		if(file.good()) {
			std::string contents;
			file.seekg(0, std::ios::end);
			contents.resize(file.tellg());
			file.seekg(0, std::ios::beg);
			file.read(&contents[0], contents.size());
			file.close();

			return files()[path] = contents;
		}
#endif
		//No file has been found
		return NULL;
	}
}

std::map<std::string, std::string>& EmbeddedFiles::files() {
	static std::map<std::string, std::string> _files{
        #ifndef READ_EXTERNAL_FILES
        #include "embedded"
        #endif
	};
	return _files;
}
