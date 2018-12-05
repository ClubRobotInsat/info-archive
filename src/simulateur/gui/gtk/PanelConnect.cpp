#include "PanelConnect.h"

#include <sstream>
#include <vector>

PanelConnect::PanelConnect() {
	_connectButton.set_label("Connect");

	_connectionType.append("TCPIP");
	_connectionType.append("localhost");
	_connectionType.set_active(0);

	_connectionTypeLbl.set_text("Connection type");

	_box1.add(_connectionTypeLbl);
	_box1.add(_connectionType);

	_connectionArgsLbl.set_text("Connection arguments");

	_box2.add(_connectionArgsLbl);
	_box2.add(_connectionArguments);

	_globalBox.add(_box1);
	_globalBox.add(_box2);
	_globalBox.add(_connectButton);

	add(_globalBox);
	set_border_width(10);
}

Gtk::Button& PanelConnect::getConnectButton() {
	return _connectButton;
}

std::string PanelConnect::getConnectionType() {
	return _connectionType.get_active_text();
}

// TODO faire de cette fonction une fonction utilitaire
std::vector<std::string> PanelConnect::getConnectionArguments() {
	std::vector<std::string> args;

	std::string parsed = _connectionArguments.get_text();
	std::ostringstream outputStream;
	int state = 0;

	for(char c : parsed) {
		switch(state) {
			case 0: // reading normally
				if(c == ' ') {
					if(outputStream.str().length()) {
						args.push_back(outputStream.str());
						outputStream = std::ostringstream();
					}
				} else if(c == '"') {
					state = 1;
				} else if(c == '\\') {
					state = 2;
				} else {
					outputStream << c;
				}
				break;
			case 1: // reading quoted
				if(c == '"') {
					state = 0;
				} else {
					outputStream << c;
				}
				break;
			case 2: // reading escaped
				outputStream << c;
				state = 0;
			default:
				break;
		}
	}

	if(outputStream.str().length() != 0)
		args.push_back(outputStream.str());

	return args;
}