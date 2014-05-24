// Log.cpp

#include "Log.h"
#include <iostream>
#include <cstring>

#define STRINGIFY(x) #x
#include "LogHTMLContents.h"
#include "LogRTFContents.h"

// Empty definitions for disabled logging
#ifndef ENABLE_LOGGING

void Log::open(Output, const std::string&, bool) {}
void Log::open(int, char**, bool) {}
void Log::close(Log::Output) {}

// When logging is enabled :
#else

// Static variables implementation
std::ostream* Log::_p_stream[Log::OUTPUT_COUNT] = {};

std::map<std::thread::id, Log::ThreadInfo> Log::_threadInfos;
std::mutex Log::_logMutex;
std::ofstream Log::_RTFfile;
std::ofstream Log::_HTMLfile;
std::ofstream Log::_TXTfile;
bool Log::_opened = false;


bool Log::openCommon(Output o, bool desync_with_stdio) {
	if(Log::_p_stream[o]) {
		logWarn("Log::open() called multiple times with same output(s), may be because of an earlier wall to Log::write()");

		return false;
	}

	// If asked, we de-sync with stdio
	std::ios_base::sync_with_stdio(!desync_with_stdio);
	return true;
}

// This "open()" parses argc and argv and calls the previous "open()".
void Log::open(int argc, char* argv[], bool desync_with_stdio) {
	// Look for "--log=XXX"
	// Usage :
	// --log=stdout (default)
	// --log=stderr
	// --log=my_file.rtf
	// --log=my_file.html or --log=my_file.htm
	
	static const char str_ref[] = "--log=";
	static const int str_ref_len = int(strlen(str_ref));
	bool found = false;
	for(int i=1 ; i < argc ; i++) {
		int j=0;
		for(; argv[i][j] != '\0' && j < str_ref_len ; j++)
			if(argv[i][j] != str_ref[j])
				break;
		
		if(j == str_ref_len) {
			std::string file_name = &argv[i][j];
			found = true;

			if(file_name == "stdout")
				open(TERMINAL, "stdout", desync_with_stdio);
			else if(file_name == "stderr")
				open(TERMINAL, "stderr", desync_with_stdio);
			else if(file_name == "stdout-nocolor")
				open(TERMINAL_NO_COLOR, "stdout-nocolor", desync_with_stdio);
			else if(file_name == "stderr-nocolor")
				open(TERMINAL_NO_COLOR, "stderr-nocolor", desync_with_stdio);
			else {
				auto file_name_len = file_name.length();

				if( file_name_len > 4 &&
				   ((file_name.substr(file_name_len-4, file_name_len-1) == ".htm") ||
					(file_name.substr(file_name_len-5, file_name_len-1) == ".html"))) {
					   open(HTML, file_name, desync_with_stdio);
				   }
				else if(file_name_len > 3 && file_name.substr(file_name_len-4, file_name_len-1) == ".rtf") {
					open(RTF, file_name, desync_with_stdio);
				}
				else if(file_name_len > 3 && file_name.substr(file_name_len-4, file_name_len-1) == ".txt") {
					open(TXT, file_name, desync_with_stdio);
				}
				else
					open(TERMINAL, "stdout", desync_with_stdio);
			}
		}
	}
	
	if(!found)
		open(TERMINAL, "stdout", desync_with_stdio);
}

void Log::open(Log::Output output, const std::string& filename, bool desync_with_stdio) {
	if(!Log::openCommon(output, desync_with_stdio))
		return;

	switch(output) {
		case TERMINAL:
			if(filename == "stderr")
				Log::_p_stream[output] = &std::cerr;
			else
				Log::_p_stream[output] = &std::cout;
			break;
		case TERMINAL_NO_COLOR:
			if(filename == "stderr-nocolor")
				Log::_p_stream[output] = &std::cerr;
			else
				Log::_p_stream[output] = &std::cerr;
			break;
		case TXT:
			_TXTfile.open(filename);
			if(!_TXTfile.is_open()) {
				std::cerr << "impossible to open the text file " << filename;
				return;
			}

			Log::_p_stream[output] = &_TXTfile;
			break;
		case HTML:
			_HTMLfile.open(filename);
			if(!_HTMLfile.is_open()) {
				std::cerr << "impossible to open the HTML file " << filename;
				return;
			}

			Log::_p_stream[output] = &_HTMLfile;
			(*_p_stream[output]) << html_begin << std::flush;
			break;
		case RTF:
			_RTFfile.open(filename);
			if(!_RTFfile.is_open()) {
				std::cerr << "impossible to open the RTF file " << filename;
				return;
			}

			Log::_p_stream[output] = &_RTFfile;
			(*_p_stream[output]) << rtf_begin << std::flush;
			break;
		case OUTPUT_COUNT:
			break;
	}

	_opened = true;
}


void Log::close(Output output) {
	if(Log::_p_stream[output]) {
		switch(output) {
			case TERMINAL:
			case TERMINAL_NO_COLOR:
				break;
			case TXT:
				Log::_TXTfile.close();
				break;
			case HTML:
				(*_p_stream[HTML]) << html_end << std::flush;
				Log::_HTMLfile.close();
				break;
			case RTF:
				(*_p_stream[RTF]) << rtf_end << std::flush;
				Log::_RTFfile.close();
				break;
			case OUTPUT_COUNT:
				break;
		}
	}
}

void Log::indent(int value) {
	std::lock_guard<std::mutex> lock(Log::_logMutex);
	
	// Look for the thread's ID in the indentation table
	auto id = std::this_thread::get_id();
	auto it = _threadInfos[id];
	
	// We add value to the current indentation.
	_threadInfos[id]._indent += value;
	if(_threadInfos[id]._indent < 0)
		_threadInfos[id]._indent = 0;
}

#endif // ENABLE_LOGGING
