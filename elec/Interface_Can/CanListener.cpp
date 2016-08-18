//
// Created by paul on 18/08/16.
//

#include "CanListener.h"
#include "Monitor.h"
#include "../../commun/Units/TimePoint.h"

CanListener::CanListener(std::string& port) : _shallStopListening(false), _refreshRate(20_ms) {

    if (port.substr(0,5) == "TCPIP") {
        _can.reset(new Commun::CAN(std::make_unique<Commun::TCPIP>(port.substr(7,port.length()-12), 1234)));
        std::cout << "CAN listening on : " << port.substr(7,port.length()-12) << ":1234" <<std::endl;
    }
    else {
        _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>(port)));
    }

    _can->setTemporisation(10_ms);

}

void CanListener::start(Monitor* caller) {

    this->mainLoop(caller);

}

void CanListener::mainLoop(Monitor* caller) {

    while(!this->shallStopListening()) {
        auto now = Units::TimePoint::now();
        while (Units::TimePoint::now() < now + _refreshRate) {
            auto Trame = this->waitForMessage();
            std::lock_guard<std::mutex> lock(_mutex);
            _messageList.push_back(Trame);
        }
        caller->onListenerNotification();
    }
}

Trame CanListener::waitForMessage() {

    auto Trame = _can->recevoirTrame();
    return Trame;

}

Trame CanListener::getOldestMessage() {
    std::lock_guard<std::mutex> lock(_mutex);
    Trame result = _messageList.front();
    //_messageList.erase(_messageList.begin());
    return result;
}

bool CanListener::shallStopListening() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _shallStopListening;
}
