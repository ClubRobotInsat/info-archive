//
// Created by paul on 18/08/16.
//

#ifndef ROOT_CANLISTENER_H
#define ROOT_CANLISTENER_H
#include "Header.h"
#include "../../robot/Commun/CAN.h"
#include "../../robot/Commun/RS232.h"
#include "../../robot/Commun/TCPIP.h"

class Monitor;

class CanListener {

public:
    CanListener(std::string& port);

    void start(Monitor* caller);

    Trame getOldestMessage();

protected:

    void mainLoop(Monitor* caller);

    Trame waitForMessage();

    bool shallStopListening();

private:

    mutable std::mutex _mutex;

    std::deque<Trame> _messageList;

    std::unique_ptr<Commun::CAN> _can;

    bool _shallStopListening;

    Units::Time _refreshRate;

};


#endif //ROOT_CANLISTENER_H
