//
// Created by paul on 18/07/16.
//

#include "Application.h"
#include "../../robot/Commun/RS232.h"

#ifdef ACTIVATE_CAN
Application::Application(std::string const& port, int argc, char** argv) {
    _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>(port)));
    _can->setTemporisation(10_ms);
#else
Application::Application(int argc, char** argv, std::string id) : Gtk::Application(argc, argv, id), _start_screen() {
#endif

    _start_screen.startScreenSignalOnExit().connect(sigc::mem_fun(this, &Application::killStartScreen));
    this->run(_start_screen);

}

void Application::startCan(std::string const& port) {

    std::cout << "Starting CAN" << std::endl;
}

void Application::killStartScreen(std::string canPort) {

    std::cout << canPort << std::endl;
    //_start_screen.~Start_Screen();

}
