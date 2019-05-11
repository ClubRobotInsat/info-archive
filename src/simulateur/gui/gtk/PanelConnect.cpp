#include "PanelConnect.h"

#include <sstream>
#include <vector>

#include "resources/EmbeddedFiles.h"

// clang-format off
std::map<std::string, std::string> CONNECTIONS{
    {"LOCAL", "[local port]"},
	{"UDP", "[@IP] [local port] [remote port]"},
	{"TCPIP", "[@IP] [remote port]"},
};
// clang-format on

PanelConnect::PanelConnect() {
	// On recupere le widget depuis un fichier embarqué dans l'executable
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/connection-panel.glade"));
	_builder->get_widget("connection-type", _connectionType);
	_builder->get_widget("connection-arguments", _connectionArguments);
	_builder->get_widget("connect-button", _connectButton);
	_builder->get_widget("arguments-tooltip", _argumentsTooltip);

	Widget* root;
	_builder->get_widget("root", root);
	add(*root);

	// Initialization
	for(auto& keyval : CONNECTIONS) {
		_connectionType->append(keyval.first);
	}

	// TODO simulator.ini
	_connectionArguments->set_text("localhost 5101 5001");

	// signals
	_connectionType->signal_changed().connect(sigc::mem_fun(*this, &PanelConnect::onConnectionTypeChanged));
}

Gtk::Button& PanelConnect::getConnectButton() {
	return *_connectButton;
}

std::string PanelConnect::getConnectionType() {
	return _connectionType->get_active_text();
}

// TODO faire de cette fonction une fonction utilitaire
std::vector<std::string> PanelConnect::getConnectionArguments() {
	std::vector<std::string> args;

	std::string parsed = _connectionArguments->get_text();
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

void PanelConnect::onConnectionTypeChanged() {
	try {
		_argumentsTooltip->set_text(CONNECTIONS.at(_connectionType->get_active_text()));
	} catch(std::exception&) {
		_argumentsTooltip->set_text(" -- no information");
	}
}