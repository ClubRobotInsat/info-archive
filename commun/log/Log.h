// Log.h

#ifndef LOG_H
#define LOG_H

#define ENABLE_LOGGING
//#define DISABLE_DEBUG_LOGGING // If defined, disables only logDebugXXX() macros.
//#define LOG_USE_GLEW
//#define LOG_ENABLE_GLFW_THREADS

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <mutex>
#include <thread>

//#define LOG_NOINFO
//#define LOG_NOSUCCESS

#ifdef LOG_USE_GLEW
#include <GL/glew.h>
#endif

#ifdef LOG_ENABLE_GLFW_THREADS
#include <GL/glfw.h>
#endif

#ifndef ENABLE_LOGGING

inline void logInfo(...) {}
inline void logSuccess(...) {}
inline void logFailed(...) {}
inline void logWarn(...) {}
inline void logError(...) {}
inline void logDebug(...) {}
inline void logDebug0(...) {}
inline void logDebug1(...) {}
inline void logDebug2(...) {}
inline void logDebug3(...) {}
inline void logDebug4(...) {}
inline void logDebug5(...) {}
inline void logDebug6(...) {}
inline void logDebug7(...) {}
inline void logDebug8(...) {}
inline void logDebug9(...) {}

#else // if ENABLE_LOGGING is enabled :

#ifndef LOG_NOINFO
#define logInfo(...) Log::write(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define logInfo(...)
#endif

#ifndef LOG_NOSUCCESS
#define logSuccess(...) Log::write(LOG_SUCCESS, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define logSuccess(...)
#endif

#define logFailed(...) Log::write(LOG_FAILED, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logWarn(...) Log::write(LOG_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logError(...) Log::write(LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#ifndef DISABLE_DEBUG_LOGGING

#define logDebug(...) Log::write(LOG_DEBUG, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug0(...) Log::write(LOG_DEBUG_0, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug1(...) Log::write(LOG_DEBUG_1, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug2(...) Log::write(LOG_DEBUG_2, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug3(...) Log::write(LOG_DEBUG_3, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug4(...) Log::write(LOG_DEBUG_4, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug5(...) Log::write(LOG_DEBUG_5, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug6(...) Log::write(LOG_DEBUG_6, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug7(...) Log::write(LOG_DEBUG_7, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug8(...) Log::write(LOG_DEBUG_8, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logDebug9(...) Log::write(LOG_DEBUG_9, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#else // if DISABLE_DEBUG_LOGGING is defined :

inline void logDebug0(...) {}
inline void logDebug1(...) {}
inline void logDebug2(...) {}
inline void logDebug3(...) {}
inline void logDebug4(...) {}
inline void logDebug5(...) {}
inline void logDebug6(...) {}
inline void logDebug7(...) {}
inline void logDebug8(...) {}
inline void logDebug9(...) {}

#endif

// Utility retrieve the file name from the complete path, i.e. "ls" from "/bin/ls"
static inline const char* getFileNameFromPath(const char* file_path) {
	const char* p = &file_path[0];
	for(int i=0 ; file_path[i] != '\0' ; i++)
		if(file_path[i] == '/' || file_path[i] == '\\')
			p = &file_path[i+1];
	return p;
}

enum LogType {
	LOG_INFO,
	LOG_SUCCESS,
	LOG_FAILED,
	LOG_WARN,
	LOG_ERROR,
	LOG_DEBUG,
	LOG_DEBUG_0=LOG_DEBUG,
	LOG_DEBUG_1,
	LOG_DEBUG_2,
	LOG_DEBUG_3,
	LOG_DEBUG_4,
	LOG_DEBUG_5,
	LOG_DEBUG_6,
	LOG_DEBUG_7,
	LOG_DEBUG_8,
	LOG_DEBUG_9
};

// Functions implemented in Log[TermWindows/VT100]/HTML/RTF.cpp
// Terminal :
void writeTermFormattedString(std::ostream* p_stream, const std::string& str);
void resetTerm(std::ostream* p_stream);
void doTermFormatting(std::string &msg, LogType type);

// HTML :
void doHTMLFormatting(std::string &msg, LogType type);

// RTF :
void doRTFFormatting(std::string &msg, LogType type);

#endif // ENABLE_LOGGING

class Log {
public:
	enum Output {
		TERMINAL = (1<<0),
		TERMINAL_NO_COLOR = (1<<1),
		TXT = (1<<2),
		HTML = (1<<3),
		RTF = (1<<4),
	};
	static const int OUTPUT_COUNT = 5;
	typedef unsigned int OutputMask;
	
private:
	// Information specific to a thread
	struct ThreadInfo {
		std::string name;
		int indent;
		
		ThreadInfo() : name(""), indent(0) {}
	};
	
private:
	static std::mutex _logMutex;
	static OutputMask _output;
	static bool opened;
	static std::ostream* p_stream[OUTPUT_COUNT];
	static std::ofstream file;
	static std::map<std::thread::id, ThreadInfo> _threadInfos;
#ifdef LOG_ENABLE_GLFW_THREADS
	// If we use GLFW's threading support, we manage a table of informations for each thread.
	static std::map<GLFWthread, ThreadInfo> thread_infos;
	static GLFWmutex mutex;
#else
	static int current_indent;
#endif
	
public:
	static void open(OutputMask output, const std::string& filename = "", bool desync_with_stdio=true);
	static void open(int argc, char* argv[], bool desync_with_stdio=true);
	static void close();
	static void indent(int value=1);
	
#ifdef LOG_ENABLE_GLFW_THREADS
	static void setThreadName(const std::string& name) {thread_infos[glfwGetThreadID()].name = name;}
	static std::string getThreadName() {return thread_infos[glfwGetThreadID()].name;}
#endif
	
#ifndef ENABLE_LOGGING
	static void write( LogType type,
			  const char* file_path,
			  int line,
			  const char* function_name,
			  ...) {
	}
#else
	
	template <typename... Args>
	static void write( LogType type, const char* file_path, int line, const char* function_name, Args &&... args);

	template <typename T, typename... Args>
	static void writeInternal( LogType type, int outputIndex, std::stringstream &stream, T &&arg, Args &&... args);
	static void writeInternal( LogType type, int outputIndex, std::stringstream &stream);
#endif
};

// RAII used to control indentation
struct LogIndent {
	LogIndent() {Log::indent(1);}
	~LogIndent() {Log::indent(-1);}
};

// Handy function, wrapper for all doXXXFormatting() ones
static inline void doFormatting(std::string &msg, LogType type, Log::Output output) {
	if(output == Log::TERMINAL)
		doTermFormatting(msg, type);
	else if(output == Log::HTML)
		doHTMLFormatting(msg, type);
	else if(output == Log::RTF)
		doRTFFormatting(msg, type);
}

template <typename T, typename... Args>
inline void Log::writeInternal( LogType type, int outputIndex, std::stringstream &stream, T &&arg, Args &&... args) {
	stream << arg;
	Log::writeInternal(type, outputIndex, stream, std::forward<Args>(args)...);
}

inline void Log::writeInternal( LogType type, int outputIndex, std::stringstream &stream) {
	stream << std::flush;
	std::string msg_first_part = stream.str();
	std::string msg_second_part;

	Output const cur_output = static_cast<Output>(1 << outputIndex);
	switch(type) {
		case LOG_INFO:
			doFormatting(msg_first_part, LOG_INFO, cur_output);
			break;
		case LOG_SUCCESS:
			doFormatting(msg_first_part, LOG_INFO, cur_output);

			msg_second_part = "[OK]";
			doFormatting(msg_second_part, LOG_SUCCESS, cur_output);
			break;
		case LOG_FAILED:
			doFormatting(msg_first_part, LOG_INFO, cur_output);

			msg_second_part = "[FAILED]";
			doFormatting(msg_second_part, LOG_FAILED, cur_output);
			break;
		case LOG_WARN:
			doFormatting(msg_first_part, LOG_WARN, cur_output);
			break;
		case LOG_ERROR:
			doFormatting(msg_first_part, LOG_ERROR, cur_output);
			break;

		default: // LOG_DEBUG_X
			doFormatting(msg_first_part, type, cur_output);
			break;
	}

	// Finally write the message
	std::string indentation = "";
	if(cur_output == TERMINAL) {
		for(int i=0 ; i<current_indent ; i++)
			indentation += " ";
		(*p_stream[outputIndex]) << indentation;
		writeTermFormattedString(p_stream[outputIndex], msg_first_part);
		writeTermFormattedString(p_stream[outputIndex], msg_second_part);

		resetTerm(p_stream[outputIndex]); // If the terminal was used, reset its colors before the final newline,
		(*p_stream[outputIndex]) << std::endl;
	}
	else if(cur_output == HTML) {
		char str_nb_pixels[100] = "";
		sprintf(str_nb_pixels, "%d", current_indent*30);

		indentation = std::string("<span style=\"margin-left:") + std::string(str_nb_pixels) + "px;\">";

		(*p_stream[outputIndex]) << indentation << msg_first_part << msg_second_part << " <br/>" << "</span>" << std::flush;
	}
	else if(cur_output == RTF) {
		for(int i=0 ; i<current_indent ; i++)
			indentation += "\\tab";
		(*p_stream[outputIndex]) << indentation << msg_first_part << msg_second_part << "\\par" << std::flush;
	}
	else {// TXT or TERMINAL_NO_COLOR
		for(int i=0 ; i<current_indent ; i++)
			indentation += " ";
		(*p_stream[outputIndex]) << indentation << msg_first_part << msg_second_part << std::endl;
	}
}

// write() implementation :
template <typename... Args>
inline void Log::write( LogType type, const char* file_path, int line, const char* function_name, Args &&... args) {
	// If the log has not been previously opened, we open it as a color terminal log.
	std::unique_lock<std::mutex> lock(_logMutex);

	if(!opened) {
		open(TERMINAL);
		lock.unlock();
		LogError("Le log n'a pas été ouvert en utilisant les arguments du programme !\n Veuillez insérer a ligne \"Log::writeLog::open(argc, argv, false);\" en début du main.\n\n");
		lock.lock();
	}

	auto it = _threadInfos.find(std::this_thread::get_id());
	if(it == _threadInfos.end()) // If not found, current indent is 0
		current_indent = 0;
	else
		current_indent = it->second.indent;

	// Get the file's name from its path
	const char* file_name = getFileNameFromPath(file_path);
	
	for(int output_index=0 ; output_index < OUTPUT_COUNT ; output_index++) {
		if(!(Log::_output & (OutputMask)(1 << output_index)))
			continue;
		
		std::stringstream ss;

		ss << "[" << file_name << ":" << line << " in " << function_name << "] : ";
		Log::writeInternal(type, output_index, ss, std::forward<Args>(args)...);
	}
}

#endif // LOG_H
