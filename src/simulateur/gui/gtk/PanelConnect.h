#ifndef ROOT_PANELCONNECT_H
#define ROOT_PANELCONNECT_H

#include "GtkInclude.h"

class PanelConnect : public Gtk::Frame {
public:
	PanelConnect();

	Gtk::Button& getConnectButton();

	std::string getConnectionType();

	std::vector<std::string> getConnectionArguments();

private:
	Gtk::Label _connectionTypeLbl;
	Gtk::Label _connectionArgsLbl;
	Gtk::ComboBoxText _connectionType;
	Gtk::Entry _connectionArguments;
	Gtk::Button _connectButton;

	Gtk::Box _box1;
	Gtk::Box _box2;
	Gtk::VBox _globalBox;
};


#endif // ROOT_PANELCONNECT_H
