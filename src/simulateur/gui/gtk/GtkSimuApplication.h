#ifndef ROOT_GTKSIMUAPPLICATION_H
#define ROOT_GTKSIMUAPPLICATION_H

#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <thread>

#include "GtkInclude.h"
#include "GtkSimuContext.h"
#include "PanelConnect.h"
#include "PanelRobotState.h"
#include "PanelTestNavigation.h"

/** This class holds the gtk application, creates the main window
 * and executes every actions on the UI. */
class GtkSimuApplication : public Gtk::Application {
public:
	GtkSimuApplication(int argc, char** argv, std::string id, GtkSimuContext& context);

	~GtkSimuApplication() override;

	/** Calls "run" method with the main window */
	int callRun();

	void stop();

	/** Waits until the gtk app is completely stopped. */
	void waitStopped();

	void showDialog(const std::string& message);

	void showErrorDialog(const std::string& message);

	/** Queues an action to be executed on the gtk thread. */
	void queueAction(const std::function<void()>& action);

private:
	GtkSimuContext& _context;

	std::unique_ptr<Gtk::Window> _mainWindow;

	Gtk::VBox _globalBox;
	PanelTestNavigation _panelTestNavigation;
	PanelConnect _panelConnect;
	PanelRobotState _panelRobotState;

	/// Actions that should be executed on the gtk thread.
	std::list<std::function<void()>> _gtkQueue;
	std::mutex _gtkQueueMutex;
	/// Condition variable used for method waitStopped()
	std::condition_variable _stoppedCV;
	std::mutex _stoppedMutex;


	/// C callback to execute all pending actions
	friend gboolean emptyGtkQueue(void* data);

	void onConnect();
	void onTestNavigation();
};


#endif // ROOT_GTKSIMUAPPLICATION_H
