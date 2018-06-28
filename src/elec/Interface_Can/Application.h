//
// Created by paul on 18/07/16.
//

#ifndef ROOT_APPLICATION_H
#define ROOT_APPLICATION_H

#include "../../robot/Commun/Communication/CAN.h"
#include "Header.h"
#include "Monitor.h"
#include "Start_Screen.h"
#include <vector>


class Application : public Gtk::Application {
public:
	Application(int argc, char** argv, std::string id);

	~Application();

	/**
	 * Delete Start_Screen and intialize a Can monitoring window
	 * @param canPort a string to intialize the can object
	 */
	void killStartScreen(std::string canPort);

	void startCan(std::string& port);

	/**
	 * Is called upon closing the monitor window
	 * @param event : the event send by Monitor::signal_delete_event
	 * @return Always true
	 */
	bool on_delete_event(GdkEventAny* event);

private:
	Start_Screen _start_screen;

	std::shared_ptr<Monitor> _monitor;
};

#endif // ROOT_APPLICATION_H
