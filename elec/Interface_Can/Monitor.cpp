//
// Created by paul on 15/08/16.
//

#include <iostream>
#include "Monitor.h"
#include "../../robot/Commun/TCPIP.h"

Monitor::Monitor(std::string const& port) : Gtk::Window() {


    this->set_size_request(800, 600);
    this->set_decorated(true);

    if (port.substr(0,5) == "TCPIP") {
        _can.reset(new Commun::CAN(std::make_unique<Commun::TCPIP>(port.substr(7,port.length()-12), 1234)));
    }
    else {
        _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>(port)));
    }

    _can->setTemporisation(10_ms);

    this->show_all();

}
