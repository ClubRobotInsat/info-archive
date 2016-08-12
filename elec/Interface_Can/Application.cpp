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
Application::Application(int argc, char** argv, std::string id) : Gtk::Application(argc, argv, id) {
#endif

    this->run(_start_screen);

}

void Application::startCan(std::string const& port) {

    std::cout << "Starting CAN" << std::endl;
}
