// Log.h

#ifndef LOG_H
#define LOG_H

#define ENABLE_LOGGING
//#define DISABLE_DEBUG_LOGGING // If defined, disables only logDebugXXX() macros.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <thread>

#define logInfo(...) Log::write(LOG_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logSuccess(...) Log::write(LOG_SUCCESS, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logFailed(...) Log::write(LOG_FAILED, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logWarn(...) Log::write(LOG_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define logError(...) Log::write(LOG_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

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

#define logRouge(...) logError(__VA_ARGS__)
#define logVert(...) logDebug0(__VA_ARGS__)
#define logBleu(...) logDebug1(__VA_ARGS__)
#define logCyan(...) logDebug3(__VA_ARGS__)
#define logMagenta(...) logDebug2(__VA_ARGS__)
#define logJaune(...) logWarn(__VA_ARGS__)
#define logNoir(...) logDebug4(__VA_ARGS__)

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

class Log {
public:
	enum Output {
		OUTPUT_FIRST,

		TERMINAL = OUTPUT_FIRST,
		TERMINAL_NO_COLOR,
		TXT,
		HTML,
		RTF,

		OUTPUT_COUNT
	};

	static void open(Output output, const std::string& filename = "", bool desync_with_stdio=true);
	static void open(int argc, char* argv[], bool desync_with_stdio=true);

	// Close a log output
	static void close(Output output);
	// Close all log outputs; not required to be called, as it is automatically invoked at program shutdown
	static void closeAll();
	static void indent(int value=1);

	template <typename... Args>
	static void write(LogType type, const char* file_path, int line, const char* function_name, Args &&... args);

private:
	// Information specific to a thread
	struct ThreadInfo {
		int _indent = 0;
	};

	static bool _opened;
	static std::mutex _logMutex;
	static std::ostream* _p_stream[OUTPUT_COUNT];
	static std::ofstream _RTFfile;
	static std::ofstream _HTMLfile;
	static std::ofstream _TXTfile;
	static std::unordered_map<std::thread::id, ThreadInfo> _threadInfos;

	template <typename T, typename... Args>
	static void writeInternal(LogType type, Output outputIndex, std::stringstream &stream, T &&arg, Args &&... args);
	static void writeInternal(LogType type, Output outputIndex, std::stringstream &stream);

	static bool openCommon(Output o, bool desync_with_stdio);
	// Utility retrieve the file name from the complete path, i.e. "ls" from "/bin/ls"
	static char const *getFileNameFromPath(const char* file_path);

	// Functions implemented in Log[TermWindows/VT100]/HTML/RTF.cpp
	// Terminal :
	static void writeTermFormattedString(std::ostream &p_stream, const std::string& str);
	static void resetTerm(std::ostream &p_stream);
	static void doTermFormatting(std::string &msg, LogType type);

	// HTML :
	static void doHTMLFormatting(std::string &msg, LogType type);

	// RTF :
	static void doRTFFormatting(std::string &msg, LogType type);

	// Handy function, wrapper for all doXXXFormatting() ones
	static inline void doFormatting(std::string &msg, LogType type, Log::Output output);
};

// RAII used to control indentation
struct LogIndent {
	LogIndent() {Log::indent(1);}
	~LogIndent() {Log::indent(-1);}
};

template <typename T, typename... Args>
inline void Log::writeInternal( LogType type, Output outputIndex, std::stringstream &stream, T &&arg, Args &&... args) {
	stream << arg;
	Log::writeInternal(type, outputIndex, stream, std::forward<Args>(args)...);
}

// write() implementation :
template <typename... Args>
inline void Log::write( LogType type, const char* file_path, int line, const char* function_name, Args &&... args) {
#ifdef ENABLE_LOGGING
#ifdef DISABLE_DEBUG_LOGGING
	if(type >= LOG_DEBUG) {
		return;
	}
#endif
	// If the log has not been previously opened, we open it as a color terminal log.
	std::lock_guard<std::mutex> lock(_logMutex);

	if(!_opened)
		Log::open(TERMINAL);

	// Get the file's name from its path
	const char* file_name = Log::getFileNameFromPath(file_path);

	for(int output_index=0 ; output_index < OUTPUT_COUNT ; output_index++) {
		if(_p_stream[output_index]) {
			std::stringstream ss;

			ss << "[" << file_name << ":" << line << " in " << function_name << "] : ";
			Log::writeInternal(type, static_cast<Output>(output_index), ss, std::forward<Args>(args)...);
		}
	}
#endif
}

#endif // LOG_H
