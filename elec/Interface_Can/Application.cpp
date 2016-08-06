//
// Created by paul on 18/07/16.
//

#include "Application.h"
#include "../../robot/Commun/RS232.h"

#ifdef ACTIVATE_CAN
Application::Application(std::string const& port) {
    _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>(port)));
    _can->setTemporisation(10_ms);
#else
Application::Application() : _start_screen(std::make_unique<Start_Screen>()) {
#endif

    //_start_screen = std::make_unique<Start_Screen>();

}

void Application::startCan(std::string const &port) {

    _start_screen.reset();
    std::cout << "Starting CAN" << std::endl;
}
