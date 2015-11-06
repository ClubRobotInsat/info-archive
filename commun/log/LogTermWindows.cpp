// LogTermWindows.cpp

#ifdef WIN32

#include <windows.h>
#include "Log.h"

enum TermColor {
	TERM_BLACK,
	TERM_BLUE,
	TERM_GREEN,
	TERM_CYAN,
	TERM_RED,
	TERM_MAGENTA,
	TERM_BROWN,
	TERM_LIGHTGRAY,
	TERM_DARKGRAY,
	TERM_LIGHTBLUE,
	TERM_LIGHTGREEN,
	TERM_LIGHTCYAN,
	TERM_LIGHTRED,
	TERM_LIGHTMAGENTA,
	TERM_YELLOW,
	TERM_WHITE
};

struct TermFormat {
	// Declaring this constructor prevents a silent failure when a LogType enumarator is added, and the array below is
	// not update accordingly.
	TermFormat(TermColor fc, TermColor bc) : front_color(fc), back_color(bc) {}

	TermColor front_color;
	TermColor back_color;
};

static const TermFormat term_formats[LogType::ALL] = {
	{TERM_WHITE, TERM_RED},   // error
	{TERM_WHITE, TERM_BROWN}, // warn

	{TERM_RED, TERM_BLACK},   // failed
	{TERM_GREEN, TERM_BLACK}, // success

	{TERM_LIGHTGRAY, TERM_BLACK}, // info

	{TERM_WHITE, TERM_GREEN},	// debug0
	{TERM_WHITE, TERM_BLUE},	 // debug1
	{TERM_WHITE, TERM_MAGENTA},  // debug2
	{TERM_WHITE, TERM_CYAN},	 // debug3
	{TERM_WHITE, TERM_BLACK},	// debug4
	{TERM_YELLOW, TERM_GREEN},   // debug5
	{TERM_YELLOW, TERM_BLUE},	// debug6
	{TERM_YELLOW, TERM_MAGENTA}, // debug7
	{TERM_YELLOW, TERM_CYAN},	// debug8
	{TERM_YELLOW, TERM_BLACK}	// debug9
};

void Log::writeTermFormattedString(ostream &p_stream, const string &str) {
	std::string str_buffer = "";
	HANDLE handle;
	if(&p_stream == &cerr)
		handle = GetStdHandle(STD_ERROR_HANDLE);
	else
		handle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Parse the string, looking for a 0x1B;<front>;<back>; sequence
	// NB : the user shouldn't write an ESCAPE character followed by a ';'.
	for(std::size_t i = 0; i < str.length(); i++) {
		if(i <= str.length() - 6 && str[i] == 0x1B && str[i + 1] == ';' && str[i + 3] == ';' && str[i + 5] == ';') {
			// We analyze the sequence and get the TermFormat
			TermFormat format;
			format.front_color = (TermColor)(int(str[i + 2]) - 42);
			format.back_color = (TermColor)(int(str[i + 4]) - 42);

			SetConsoleTextAttribute(handle, format.back_color << 4 | format.front_color);

			p_stream << str_buffer << std::flush;
			i += 5; // We jump over the sequence
			str_buffer = "";
		} else
			str_buffer += str[i];
	}
	p_stream << str_buffer << std::flush;
}

void Log::resetTerm(ostream &p_stream) {
	HANDLE handle;
	if(&p_stream == &cerr)
		handle = GetStdHandle(STD_ERROR_HANDLE);
	else
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, TERM_BLACK << 4 | TERM_LIGHTGRAY);
}

void Log::doTermFormatting(string &msg, LogType type) {
	const TermFormat &format = term_formats[(int)type];

	char str_beginning[64] = "";
	sprintf(str_beginning, "%c;%c;%c;", 0x1B, char(int(format.front_color) + 42), char(int(format.back_color) + 42));
	msg = string(str_beginning) + msg;
}
#endif // WIN32
