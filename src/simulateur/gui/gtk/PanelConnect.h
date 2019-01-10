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
	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::ComboBoxText *_connectionType;
	Gtk::Entry *_connectionArguments;
	Gtk::Button *_connectButton;
};


#endif // ROOT_PANELCONNECT_H
