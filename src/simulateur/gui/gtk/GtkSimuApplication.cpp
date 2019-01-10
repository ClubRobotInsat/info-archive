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


GtkSimuApplication::GtkSimuApplication(int argc, char** argv, std::string id, GtkSimuContext& context, IGuiClient& guiClient)
        : Gtk::Application(argc, argv, id), _context(context), _guiClient(guiClient) {

	// Window configuration
	_mainWindow = std::make_unique<Gtk::Window>();
	_mainWindow->set_title("Club robot Simulator 3");
	_mainWindow->set_size_request(400, 800);
	_mainWindow->set_resizable(true);

	// Fill the window
	_globalBox.add(_panelConnect);
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

void GtkSimuApplication::showErrorDialog(const std::string &message) {
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
		_guiClient.connect({_panelConnect.getConnectionType(), _panelConnect.getConnectionArguments()});
	});
}