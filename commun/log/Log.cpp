// Log.cpp

#include "Log.h"
#include <iostream>
#include <cstring>

// Empty definitions for disabled logging
#ifndef ENABLE_LOGGING

inline void Log::open(OutputMask output, const std::string& filename) {}
inline void Log::open(int argc, char* argv[]) {}
inline void Log::close() {}
inline void Log::indent(int value) {}
inline void Log::write( LogType type,
		       const char* file_path,
		       int line,
		       const char* function_name,
		       ...) {}

// When logging is enabled :
#else

#define STRINGIFY(x) #x

#include "LogHTMLContents.h"
#include "LogRTFContents.h"

// Static variables implementation
Log::OutputMask Log::_output = Log::TERMINAL;
bool Log::opened = false;
std::ostream* Log::p_stream[Log::OUTPUT_COUNT];
std::ofstream Log::file;

int Log::current_indent = 0;
std::map<std::thread::id, Log::ThreadInfo> Log::_threadInfos;
std::mutex Log::_logMutex;


void Log::open(OutputMask output, const std::string& filename, bool desync_with_stdio) {
	if(opened) {
		logWarn("Log::open() called multiple times, probably because of an earlier wall to Log::write()");
		return;
	}
	
#ifdef LOG_ENABLE_GLFW_THREADS
	glfwInit(); // NB : there are no problems with calling glfwInit() multiple times (Cf glfwInit()'s source code)
	Log::mutex = glfwCreateMutex();
	thread_infos.clear();
#else
	Log::current_indent = 0;
#endif
	
	// If asked, we de-sync with stdio
	std::ios_base::sync_with_stdio(!desync_with_stdio);
	
	Log::opened = true;
	Log::_output = output;
	
	memset(Log::p_stream, 0, sizeof(Log::p_stream));
	
	for(int output_index=0 ; output_index < OUTPUT_COUNT ; output_index++) {
		Output cur_output = (Output)(1<<output_index);
		if(!(Log::_output & cur_output))
			continue;
		switch(cur_output) {
			case TERMINAL:
				if(filename == "stderr")
					Log::p_stream[output_index] = &std::cerr;
				else
					Log::p_stream[output_index] = &std::cout;
				break;
			case TERMINAL_NO_COLOR:
				if(filename == "stderr-nocolor")
					Log::p_stream[output_index] = &std::cerr;
				else
					Log::p_stream[output_index] = &std::cerr;
				break;
			case TXT:
				file.open(filename);
				if(!file.is_open()) {
					Log::_output = TERMINAL;
					std::cerr << "impossible to open the text file " << filename.c_str();
					return;
				}
				
				Log::p_stream[output_index] = &file;
				break;
			case HTML:
				file.open(filename);
				if(!file.is_open()) {
					Log::_output = TERMINAL;
					std::cerr << "impossible to open the HTML file " << filename.c_str();
					return;
				}
				
				Log::p_stream[output_index] = &file;
				
				(*p_stream[output_index]) << html_begin << std::flush;
				break;
			case RTF:
				file.open(filename);
				if(!file.is_open()) {
					Log::_output = TERMINAL;
					std::cerr << "impossible to open the RTF file " << filename.c_str();
					return;
				}
				
				Log::p_stream[output_index] = &file;
				
				(*p_stream[output_index]) << rtf_begin << std::flush;
				break;
			default:
				break;
		}
	}
}

// This "open()" parses argc and argv and calls the previous "open()".
void Log::open(int argc, char* argv[], bool desync_with_stdio) {
	if(opened) {
		logWarn("Log::open() called multiple times, probably because of an earlier wall to Log::write()");
		return;
	}
	
	// Default value : output to terminal
	_output = TERMINAL;
	memset(Log::p_stream, 0, sizeof(Log::p_stream));
	p_stream[0] = &std::cout;
	
	// Look for "--log=XXX"
	// Usage :
	// --log=stdout (default)
	// --log=stderr
	// --log=my_file.rtf
	// --log=my_file.html or --log=my_file.htm
	
	static const char str_ref[] = "--log=";
	static const int str_ref_len = int(strlen(str_ref));
	char* raw_file_name = nullptr;
	bool found = false;
	for(int i=1 ; i < argc && !found ; i++) {
		int j=0;
		for(; argv[i][j] != '\0' && j < str_ref_len ; j++)
			if(argv[i][j] != str_ref[j])
				break;
		
		if(j == str_ref_len) {
			raw_file_name = &argv[i][j];
			found = true;
		}
	}
	
	if(found) {
		std::string file_name = raw_file_name;
		
		if(file_name == "stdout")
			open(TERMINAL, "stdout", desync_with_stdio);
		else if(file_name == "stderr")
			open(TERMINAL, "stderr", desync_with_stdio);
		else if(file_name == "stdout-nocolor")
			open(TERMINAL_NO_COLOR, "stdout-nocolor", desync_with_stdio);
		else if(file_name == "stderr-nocolor")
			open(TERMINAL_NO_COLOR, "stderr-nocolor", desync_with_stdio);
		else {
			std::string file_name = raw_file_name;
			int file_name_len = int(file_name.length());
			
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
	else
		open(TERMINAL, "stdout", desync_with_stdio);
}

void Log::close() {
	if(Log::opened) {
		Log::opened = false;
		
#ifdef LOG_ENABLE_GLFW_THREADS
		glfwDestroyMutex(Log::mutex);
#endif
		
		switch(_output) {
			case TERMINAL:
			case TERMINAL_NO_COLOR:
				break;
			case TXT:
				Log::_output = TERMINAL;
				Log::p_stream[0] = &std::cout;
				Log::file.close();
				break;
			case HTML:
				(*p_stream[HTML]) << html_end << std::flush;
				Log::_output = TERMINAL;
				Log::p_stream[0] = &std::cout;
				Log::file.close();
				break;
			case RTF:
				(*p_stream[RTF]) << rtf_end << std::flush;
				Log::_output = TERMINAL;
				Log::p_stream[0] = &std::cout;
				Log::file.close();
				break;
			default:
				break;
		}
	}
}

#ifndef LOG_ENABLE_GLFW_THREADS
void Log::indent(int value) {
	current_indent += value;
	if(current_indent < 0)
		current_indent = 0;
}
#else
void Log::indent(int value) {
	glfwLockMutex(Log::mutex);
	
	// Look for the thread's ID in the indentation table
	GLFWthread id = glfwGetThreadID();
	std::map<GLFWthread, ThreadInfo>::iterator it = thread_infos.find(id);
	
	// If we haven't found the ID, we create a new entry in the table, otherwise we add value to the current indentation.
	if(it == thread_infos.end()) {
		if(value < 0)
			thread_infos[id].indent = 0;
		else
			thread_infos[id].indent = value;
	}
	else {
		thread_infos[id].indent += value;
		if(thread_infos[id].indent < 0)
			thread_infos[id].indent = 0;
	}
	
	glfwUnlockMutex(Log::mutex);
}
#endif // LOG_ENABLE_GLFW_THREADS

#endif // ENABLE_LOGGING
