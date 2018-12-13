#include "PanelConnect.h"

#include <sstream>
#include <vector>

PanelConnect::PanelConnect() {
	_connectButton.set_label("Connect");

	_connectionType.append("UDP");
	_connectionType.set_active(0);
	_connectionType.set_size_request(-1, 30);

	_connectionTypeLbl.set_text("Connection type");
	_connectionTypeLbl.set_size_request(100, 30);

	_box1.set_size_request(-1, 30);
	_box1.set_border_width(10);
	
	_box1.pack_start(_connectionTypeLbl);
	_box1.pack_start(_connectionType);

	_connectionArgsLbl.set_text("Connection arguments");
	_connectionArgsLbl.set_size_request(100, -1);

	_box2.set_border_width(10);

	_box2.pack_start(_connectionArgsLbl);
	_box2.pack_start(_connectionArguments);

	_globalBox.pack_start(_box1);
	_globalBox.pack_start(_box2);
	_globalBox.pack_start(_connectButton);

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