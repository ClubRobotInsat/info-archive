#include "PanelRobotState.h"
#include <resources/EmbeddedFiles.h>

template <class UnaryFunction>

PanelRobotState::PanelRobotState() {
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/state-panel.glade"));
}


void PanelRobotState::refreshPanel(JSON json) {
	setElement(json, [](JSON::const_iterator it) {
		std::cout << *it << std::endl; // Print in console
	});)
}


void setElement(JSON json, UnaryFunction f) {
	for(auto it = json.begin(); it != json.end(); ++it) {
		if(it->is_structured()) {
			setElement(it, f);
		} else {
			f(it);
		}
	}
}

JSON PanelRobotState::generateTestJSON() {
	JSON json;
	json["servo1"] = JSON();
	json["servo2"] = JSON();

	json["servo1"]["blocked"] = true;
	json["servo2"]["blocked"] = false;
	json["servo1"]["mode"] = "Speed";
	json["servo1"]["color"] = "Color";

	return json;
}
