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


GtkSimuApplication::GtkSimuApplication(int argc, char** argv, std::string id, GtkSimuContext& context)
        : Gtk::Application(argc, argv, id), _context(context), _panelTestNavigation(context), _panelCentral(context), _panelRobotState() {

	// Window configuration
	_mainWindow = std::make_unique<Gtk::Window>();
	_mainWindow->set_title("Club robot Simulator 3");
	_mainWindow->set_size_request(400, 800);
	_mainWindow->set_resizable(true);

	// Fill the window
	_globalBox.add(_panelConnect);
	_globalBox.add(_panelCentral);
	_globalBox.add(_panelTestNavigation);
	_globalBox.add(_panelRobotState);
	_mainWindow->add(_globalBox);

	// Signals
	_panelConnect.getConnectButton().signal_clicked().connect(sigc::mem_fun(*this, &GtkSimuApplication::onConnect));

	// Show
	_mainWindow->show_all_children();

	g_timeout_add(20, emptyGtkQueue, this);
}

GtkSimuApplication::~GtkSimuApplication() {
	_mainWindow->close();
}

int GtkSimuApplication::callRun() {
	return run(*_mainWindow);
}

void GtkSimuApplication::stop() {
	_mainWindow->close();
	std::unique_lock<std::mutex> lck;
	_stoppedCV.notify_all();
}

void GtkSimuApplication::waitStopped() {
	std::unique_lock<std::mutex> lck(_stoppedMutex);
	_stoppedCV.wait(lck);
}

void GtkSimuApplication::showDialog(const std::string& message) {
	Gtk::MessageDialog infoDialog(*_mainWindow, "Info", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
	infoDialog.set_secondary_text(message.c_str());
	infoDialog.run();
}

void GtkSimuApplication::showErrorDialog(const std::string& message) {
	Gtk::MessageDialog errorDialog(*_mainWindow, "Error", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
	errorDialog.set_secondary_text(message.c_str());
	errorDialog.run();
}

void GtkSimuApplication::queueAction(const std::function<void()>& action) {
	std::lock_guard<std::mutex> guard(_gtkQueueMutex);

	_gtkQueue.push_back(action);
}

void GtkSimuApplication::onConnect() {
	_context.queueAction([this]() {
		_context.getGuiClient().connect({_panelConnect.getConnectionType(), _panelConnect.getConnectionArguments()});
	});
}