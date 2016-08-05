//
// Created by paul on 18/07/16.
//

#include "Application.h"
#include "../../robot/Commun/RS232.h"

Application::Application(std::string const& port) {

    _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>(port)));
    _can->setTemporisation(10_ms);

}

void Application::loop() {

}
