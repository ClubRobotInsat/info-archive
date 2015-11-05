// LogHTML.cpp

#include "Log.h"

// Using this struct with its constructor prevents a silent failure when a LogType enumarator is added, and the array
// below is not update accordingly.
struct StringWrapper {
	StringWrapper(char const *str) : string(str) {}

	char const *string;
};

static StringWrapper css_classes[(int)LogType::ALL] = {
	"error",
	"warn",

	"failed",
	"success",

	"info",
	"debug0",
	"debug1",
	"debug2",
	"debug3",
	"debug4",
	"debug5",
	"debug6",
	"debug7",
	"debug8",
	"debug9",
};

void Log::doHTMLFormatting(std::string &msg, LogType type) {
	const char *css_class = css_classes[(int)type].string;
	msg = std::string("<span class=\"") + std::string(css_class) + "\">" + msg + "</span>";
}
