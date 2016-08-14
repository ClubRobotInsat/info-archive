//
// Created by paul on 18/07/16.
//

#ifndef ROOT_APPLICATION_H
#define ROOT_APPLICATION_H

#include <vector>
#include "../../robot/Commun/CAN.h"
#include "Header.h"
#include "Start_Screen.h"


class Application : public Gtk::Application {
public:

#ifdef ACTIVATE_CAN
    /**
     * @param port The port on wich the Can object will be listening
     */
    Application(std::string const& port, int argc, char** argv);
#else
    Application(int argc, char** argv, std::string id);
#endif

    /**
     * Delete Start_Screen and create a Can monitoring window
     * @param canPort a string to intialize the can object
     */
    void killStartScreen(std::string canPort);

    void startCan(std::string const& port);

private:

    std::vector<int> _messageList;

    std::shared_ptr<Commun::CAN> _can;

    Start_Screen _start_screen;

};

#endif //ROOT_APPLICATION_H
