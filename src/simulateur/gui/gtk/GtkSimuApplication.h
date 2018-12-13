#ifndef ROOT_GTKSIMUAPPLICATION_H
#define ROOT_GTKSIMUAPPLICATION_H

#include <functional>
#include <list>
#include <mutex>
#include <thread>

#include "GtkInclude.h"
#include "GtkSimuContext.h"
#include "PanelConnect.h"
#include "PanelRobotState.h"

class GtkSimuApplication : public Gtk::Application {
public:
	GtkSimuApplication(int argc, char** argv, std::string id, GtkSimuContext& context, IGuiClient& guiClient);

	~GtkSimuApplication() override;

	/** Calls "run" method with the main window */
	int callRun();

	void showErrorDialog(const std::string &message);

	/** Queues an action to be executed on the gtk thread. */
	void queueAction(const std::function<void()>& action);

private:
	GtkSimuContext& _context;
	IGuiClient& _guiClient;

	std::unique_ptr<Gtk::Window> _mainWindow;

	Gtk::VBox _globalBox;
	PanelConnect _panelConnect;
	PanelRobotState _panelRobotState;

	/// Actions that should be executed on the gtk thread.
	std::list<std::function<void()>> _gtkQueue;
	std::mutex _gtkQueueMutex;


	/// C callback to execute all pending actions
	friend gboolean emptyGtkQueue(void* data);

	void onConnect();
};


#endif // ROOT_GTKSIMUAPPLICATION_H
