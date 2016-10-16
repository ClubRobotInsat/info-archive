//
// Created by paul on 18/07/16.
//

#include "Application.h"

Application::Application(int argc, char** argv, std::string id)
        : Gtk::Application(argc, argv, id), _start_screen(), _monitor() {

	_start_screen.startScreenSignalOnExit().connect(sigc::mem_fun(this, &Application::killStartScreen));
	this->run(_start_screen);
}

void Application::startCan(std::string& port) {

	_monitor = std::make_shared<Monitor>(port);
	_monitor->signal_delete_event().connect(sigc::mem_fun(this, &Application::on_delete_event));
	this->add_window(*_monitor);
}

void Application::killStartScreen(std::string canPort) {

	this->startCan(canPort);
	this->remove_window(_start_screen);
	_start_screen.hide();
	//_start_screen.~Start_Screen();
}

bool Application::on_delete_event(GdkEventAny*) {

	this->_monitor->endListenerThread();
	this->quit();
	return true;
}

Application::~Application() {}