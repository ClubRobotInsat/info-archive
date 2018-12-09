#include "GtkSimuApplication.h"

#include <iostream>

gboolean emptyGtkQueue(void* data) {
	auto app = static_cast<GtkSimuApplication*>(data);

	std::lock_guard<std::mutex> guard(app->_gtkQueueMutex);

	for(auto& action : app->_gtkQueue) {
		action();
	}

	app->_gtkQueue.clear();
	return TRUE;
}


GtkSimuApplication::GtkSimuApplication(int argc, char** argv, std::string id) : Gtk::Application(argc, argv, id) {

	_mainWindow = std::make_unique<Gtk::Window>();
	_mainWindow->set_title("Club robot Simulator 3");
	_mainWindow->set_size_request(400, 800);
	_mainWindow->set_resizable(true);

	g_timeout_add(20, emptyGtkQueue, this);

	run(*_mainWindow);
}

GtkSimuApplication::~GtkSimuApplication() {
	_mainWindow->close();
}

void GtkSimuApplication::queueAction(const std::function<void()>& action) {
	std::lock_guard<std::mutex> guard(_gtkQueueMutex);

	_gtkQueue.push_back(action);
}
