//
// Created by paul on 18/07/16.
//

#include "Application.h"
#include <log/Log.h>

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
	try {
		this->startCan(canPort);
		this->remove_window(_start_screen);
		_start_screen.hide();
	} catch(std::exception& e) {
		logError("-> Lancer l'application en sudo.");
		Gtk::MessageDialog dialog(_start_screen,
		                          "Je n'ai pas les permissions nécessaires pour me connecter au robot. "
		                          "Essayez de lancer l'application avec sudo");
		dialog.run();
	}
}

bool Application::on_delete_event(GdkEventAny*) {
	logDebug("Quitter l'application...");
	this->quit();
	return true;
}

Application::~Application() {}