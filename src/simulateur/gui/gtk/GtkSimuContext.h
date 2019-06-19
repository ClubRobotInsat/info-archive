#ifndef ROOT_GTKSIMUCONTEXT_H
#define ROOT_GTKSIMUCONTEXT_H

#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <thread>

#include "../IGuiClient.h"
#include "../IGuiContext.h"

class GtkSimuApplication;

/** This class manages a Simulator GUI context based on Gtk.
 * \see IGuiContext */
class GtkSimuContext : public IGuiContext {
public:
	GtkSimuContext(int argc, char** argv, std::string id, IGuiClient& client);

	~GtkSimuContext() override;

	void update() override;

	void displayMessage(const std::string& message) override;

	void displayErrorMessage(const std::string& message) override;

	void displayRobotState(const JSON& robotState);

	void close() override;

	IGuiClient& getGuiClient() {
		return _guiClient;
	}

	/// Add an action to be executed on the simulator thread.
	/// Used in the gtk thread to sync with the simulator thread.
	void queueAction(const std::function<void()>& action);

private:
	std::unique_ptr<GtkSimuApplication> _application;
	IGuiClient& _guiClient;

	std::thread _gtkThread;
	std::atomic_bool _appRunning;

	/// Actions that should be executed on the simulator thread
	std::list<std::function<void()>> _actionsQueue;
	std::mutex _actionsQueueMutex;
};


#endif // ROOT_GTKSIMUCONTEXT_H
