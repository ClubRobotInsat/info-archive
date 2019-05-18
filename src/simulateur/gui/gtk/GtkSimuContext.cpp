#include "GtkSimuContext.h"
#include <iostream>

#include "GtkSimuApplication.h"

GtkSimuContext::GtkSimuContext(int argc, char** argv, std::string id, IGuiClient& guiClient) : _guiClient(guiClient) {

	auto gtkApp = [this, argc, argv, id]() {
		this->_application = std::make_unique<GtkSimuApplication>(argc, argv, id, *this);
		this->_application->callRun();
		this->_application = nullptr;
	};

	_gtkThread = std::thread(gtkApp);
}

GtkSimuContext::~GtkSimuContext() {
	// TODO ensure complete thread safety
	if(_application != nullptr) {
		_application->queueAction([this]() { _application->stop(); });
	}
	_gtkThread.join();
}

void GtkSimuContext::update() {
	std::lock_guard<std::mutex> guard(_actionsQueueMutex);

	for(auto& action : _actionsQueue) {
		try {
			action();
		} catch(std::exception& e) {
			displayErrorMessage(e.what());
		}
	}

	_actionsQueue.clear();
}

void GtkSimuContext::displayMessage(const std::string& message) {
	_application->queueAction([this, message]() { _application->showDialog(message); });
}

void GtkSimuContext::displayErrorMessage(const std::string& message) {
	_application->queueAction([this, message]() { _application->showErrorDialog(message); });
}

void GtkSimuContext::close() {
	_application->queueAction([this]() { _application->stop(); });
	_application->waitStopped();
}

void GtkSimuContext::queueAction(const std::function<void()>& action) {
	std::lock_guard<std::mutex> guard(_actionsQueueMutex);

	_actionsQueue.push_back(action);
}