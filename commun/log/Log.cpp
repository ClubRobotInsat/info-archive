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

namespace {
	struct LogCloser {
		~LogCloser() {
			Log::closeAll();
		}
	};
}

// Static variables implementation
std::ostream* Log::_p_stream[Log::OUTPUT_COUNT] = {};

std::unordered_map<std::thread::id, Log::ThreadInfo> Log::_threadInfos;
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

char const *Log::getFileNameFromPath(const char* file_path) {
	const char* p = &file_path[0];
	for(int i=0 ; file_path[i] != '\0' ; i++)
		if(file_path[i] == '/' || file_path[i] == '\\')
			p = &file_path[i+1];
	return p;
}

void Log::doFormatting(std::string &msg, LogType type, Log::Output output) {
	if(output == Log::TERMINAL)
		Log::doTermFormatting(msg, type);
	else if(output == Log::HTML)
		Log::doHTMLFormatting(msg, type);
	else if(output == Log::RTF)
		Log::doRTFFormatting(msg, type);
}

// This "open()" parses argc and argv and calls the previous "open()".
void Log::open(int argc, char* argv[], bool desync_with_stdio) {
	// Look for "--log=XXX"
	// Usage :
	// --log=stdout (default)
	// --log=stderr
	// --log=my_file.rtf
	// --log=my_file.html or --log=my_file.htm

	// Wrapper qui va appeler automatiquement Log::closeAll à la fin du programme à travers son destructeur
	static LogCloser logCloser;

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

void Log::closeAll() {
	for(Output o = OUTPUT_FIRST; o != OUTPUT_COUNT; o = static_cast<Log::Output>(static_cast<int>(o) + 1)) {
		Log::close(o);
	}
}

void Log::indent(int value) {
	std::lock_guard<std::mutex> lock(Log::_logMutex);
	
	// Look for the thread's ID in the indentation table
	auto id = std::this_thread::get_id();
	
	// We add value to the current indentation.
	_threadInfos[id]._indent += value;
	if(_threadInfos[id]._indent < 0)
		_threadInfos[id]._indent = 0;
}

void Log::writeInternal(LogType type, Output outputIndex, std::stringstream &stream) {
	stream << std::flush;
	std::string msg_first_part = stream.str();
	std::string msg_second_part;

	switch(type) {
	case LOG_INFO:
		doFormatting(msg_first_part, LOG_INFO, outputIndex);
		break;
	case LOG_SUCCESS:
		doFormatting(msg_first_part, LOG_INFO, outputIndex);

		msg_second_part = "[OK]";
		doFormatting(msg_second_part, LOG_SUCCESS, outputIndex);
		break;
	case LOG_FAILED:
		doFormatting(msg_first_part, LOG_INFO, outputIndex);

		msg_second_part = "[FAILED]";
		doFormatting(msg_second_part, LOG_FAILED, outputIndex);
		break;
	case LOG_WARN:
		doFormatting(msg_first_part, LOG_WARN, outputIndex);
		break;
	case LOG_ERROR:
		doFormatting(msg_first_part, LOG_ERROR, outputIndex);
		break;

	default: // LOG_DEBUG_X
		doFormatting(msg_first_part, type, outputIndex);
		break;
	}

	auto infos = _threadInfos[std::this_thread::get_id()];

	// Finally write the message
	if(outputIndex == TERMINAL) {
		for(int i = 0; i < infos._indent ;i++)
			(*_p_stream[outputIndex]) << ' ';

		writeTermFormattedString(*_p_stream[outputIndex], msg_first_part);
		writeTermFormattedString(*_p_stream[outputIndex], msg_second_part);

		resetTerm(*_p_stream[outputIndex]); // If the terminal was used, reset its colors before the final newline,
		(*_p_stream[outputIndex]) << std::endl;
	}
	else if(outputIndex == HTML) {
		std::string indentation = ("<span style=\"margin-left:") + std::to_string(infos._indent * 30) + "px;\">";

		(*_p_stream[outputIndex]) << indentation << msg_first_part << msg_second_part << " <br/>" << "</span>" << std::flush;
	}
	else if(outputIndex == RTF) {
		for(int i = 0; i < infos._indent ; i++)
			(*_p_stream[outputIndex]) << "\\tab";
		(*_p_stream[outputIndex]) << msg_first_part << msg_second_part << "\\par" << std::flush;
	}
	else {// TXT or TERMINAL_NO_COLOR
		for(int i = 0;i < infos._indent; i++)
			(*_p_stream[outputIndex]) << ' ';
		(*_p_stream[outputIndex]) << msg_first_part << msg_second_part << std::endl;
	}
}

#endif // ENABLE_LOGGING
