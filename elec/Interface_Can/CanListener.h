//
// Created by paul on 18/08/16.
//

#ifndef ROOT_CANLISTENER_H
#define ROOT_CANLISTENER_H
#include "Header.h"
#include "../../robot/Commun/CAN.h"
#include "../../robot/Commun/RS232.h"
#include "../../robot/Commun/TCPIP.h"
#include "Message_Buffer.h"

class Monitor;

class CanListener {

public:
    CanListener(std::string& port);

    void start(Monitor* caller);

    Trame getOldestMessage();

    void setFilter(std::string newfilter);

    bool sendMessage(Trame& trame);

protected:

    void mainLoop(Monitor* caller);

    Trame waitForMessage();

    bool shallStopListening();

    Message_Buffer& getBuffer();

private:

    std::atomic_bool _sentAMessage;

    mutable std::mutex _mutex;

    std::unique_ptr<Commun::CAN> _can;

    bool _shallStopListening;

    Units::Time _refreshRate;

    Message_Buffer _buffer;

    std::string _filter;

};


#endif //ROOT_CANLISTENER_H
