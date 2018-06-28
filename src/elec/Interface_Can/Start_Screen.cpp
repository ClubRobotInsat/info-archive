//
// Created by paul on 05/08/16.
//

#include "Start_Screen.h"
#include "../../robot/Commun/Communication/TCPIP.h"
#include <cstring>
#include <dirent.h>
#include <iostream>
#include <log/Log.h>

Start_Screen::Start_Screen() : Gtk::Window(), _canAdress(""), _serialList({}) {

	_canListeningOnTCPIP = false;

	this->set_border_width(1);
	this->set_decorated(true);
	this->set_size_request(400, 50);
	this->set_position(Gtk::WindowPosition::WIN_POS_CENTER_ALWAYS);
	this->set_resizable(false);

	add(_container);
	_container.add(_displayedList);
	_container.add(_launchCanMonitor);
	_container.add(_refreshList);

	_refreshList.set_label("Refresh");
	_launchCanMonitor.set_label("Start Connection");

	_refreshList.signal_clicked().connect(sigc::mem_fun(*this, &Start_Screen::mainLoop));
	_launchCanMonitor.signal_clicked().connect(sigc::mem_fun(*this, &Start_Screen::emitLaunchCanSignal));

	show_all_children();

	this->mainLoop();
}

Start_Screen::~Start_Screen() {}


bool Start_Screen::scanSerialConnection() {

	_serialList.clear();

	// temp file for storing everything that is in /dev;
	std::vector<std::string> files_in_dev = {};

	// open a directory and read it
	auto open_dir = opendir("/dev");
	auto dev_dir = readdir(open_dir);

	// populate files_in_dev with all the files in /dev that matches "ttyU"
	while(dev_dir != NULL) {
		std::string string_filename = dev_dir->d_name;
		if(string_filename.substr(0, 6) == "ttyUSB" or string_filename.substr(0, 6) == "ttyACM") {
			files_in_dev.push_back(string_filename);
		}
		dev_dir = readdir(open_dir);
	}
	if(_serialList != files_in_dev) {
		_serialList = files_in_dev;
		return true;
	} else {
		return false;
	}
}

void Start_Screen::mainLoop() {

	this->scanSerialConnection();
	this->scanTCPIPConnection();
	this->updateComboBoxList();
	this->updateConnectionButton();
}

void Start_Screen::updateComboBoxList() {

	_displayedList.remove_all();
	for(auto items : _serialList) {
		_displayedList.append(items);
		_displayedList.set_active_text(items);
	}
	if(_canListeningOnTCPIP) {
		_displayedList.append("TCPIP: 127.0.0.1:1234");
		_displayedList.set_active_text("TCPIP: 127.0.0.1:1234");
	}
}

void Start_Screen::scanTCPIPConnection() {

	try {
		Commun::TCPIP connection("127.0.0.1", 1234);
		_canListeningOnTCPIP = true;
	} catch(...) {
		logVert("Mais pas de panique ! C'est normal.");
		_canListeningOnTCPIP = false;
	}
}

Start_Screen::type_startScreenSignalOnExit Start_Screen::startScreenSignalOnExit() {
	return _startScreenSignalOnExit;
}

void Start_Screen::emitLaunchCanSignal() {

	_canAdress = _displayedList.get_active_text();
	_startScreenSignalOnExit.emit(_canAdress);
}

void Start_Screen::updateConnectionButton() {

	_launchCanMonitor.set_sensitive((_displayedList.get_active_text() != ""));
}
