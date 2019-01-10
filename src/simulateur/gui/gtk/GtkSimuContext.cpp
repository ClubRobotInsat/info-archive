#include <iostream>
#include "GtkSimuContext.h"

#include "GtkSimuApplication.h"

GtkSimuContext::GtkSimuContext(int argc, char** argv, std::string id, IGuiClient& guiClient) {

	auto gtkApp = [this, argc, argv, id, &guiClient]() {
		this->_application = std::make_unique<GtkSimuApplication>(argc, argv, id, *this, guiClient);
		this->_application->callRun();
	};

	_gtkThread = std::thread(gtkApp);
	_gtkThread.detach();
}

GtkSimuContext::~GtkSimuContext() = default;

void GtkSimuContext::update() {
	std::lock_guard<std::mutex> guard(_actionsQueueMutex);

	for(auto& action : _actionsQueue) {
		try {
			action();
		}
		catch (std::exception &e) {
			displayErrorMessage(e.what());
		}
	}

	_actionsQueue.clear();
}

void GtkSimuContext::displayMessage(const std::string& message) {}

void GtkSimuContext::displayErrorMessage(const std::string &message) {
	_application->queueAction([this, message]() {
		_application->showErrorDialog(message);
	});
}

void GtkSimuContext::queueAction(const std::function<void()>& action) {
	std::lock_guard<std::mutex> guard(_actionsQueueMutex);

	_actionsQueue.push_back(action);
}