//
// Created by paul on 15/08/16.
//

#ifndef ROOT_MONITOR_H
#define ROOT_MONITOR_H


#include "Header.h"
#include "../../robot/Commun/CAN.h"
#include "../../robot/Commun/RS232.h"

class Monitor : public Gtk::Window {

public :

    Monitor(std::string const& port);

private:

    std::unique_ptr<Commun::CAN> _can;

};


#endif //ROOT_MONITOR_H
