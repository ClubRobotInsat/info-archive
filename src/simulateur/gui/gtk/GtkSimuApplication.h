#ifndef ROOT_GTKSIMUAPPLICATION_H
#define ROOT_GTKSIMUAPPLICATION_H

#include <functional>
#include <list>
#include <mutex>
#include <thread>

#include "GtkInclude.h"

class GtkSimuApplication : public Gtk::Application {
public:
	GtkSimuApplication(int argc, char** argv, std::string id);

	~GtkSimuApplication() override;

	void queueAction(const std::function<void()>& action);

private:
	std::unique_ptr<Gtk::Window> _mainWindow;

	/// Actions that should be executed on the gtk thread.
	std::list<std::function<void()>> _gtkQueue;
	std::mutex _gtkQueueMutex;


	/// C callback to execute all pending actions
	friend gboolean emptyGtkQueue(void* data);
};


#endif // ROOT_GTKSIMUAPPLICATION_H
