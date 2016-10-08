//
// Created by paul on 18/08/16.
//

#include "CanListener.h"
#include "Monitor.h"

CanListener::CanListener(std::string& port) :  _sentAMessage(false), _shallStopListening(false), _refreshRate(20_ms), _buffer(10000) {

    if (port.substr(0,5) == "TCPIP") {
        _can.reset(new Commun::CAN(std::make_unique<Commun::TCPIP>(port.substr(7,port.length()-12), 1234)));
        std::cout << "CAN listening on : " << port.substr(7,port.length()-12) << ":1234" <<std::endl;
    }
    else {
        _can.reset(new Commun::CAN(std::make_unique<Commun::RS232>("/dev/" + port)));
        std::cout << "CAN listening on : " << "/dev/" + port << std::endl;
    }

    _can->setTemporisation(10_ms);

}

void CanListener::start(Monitor* caller) {

    this->mainLoop(caller);

}

void CanListener::mainLoop(Monitor* caller) {

    while(!this->shallStopListening()) {

        bool shallNotify = false;

        // check if other threads added stuff to the _buffer and if the caller needs to be notified
        // usefull for the sendMessage() function

        if (_sentAMessage) {
            caller->notify();
            _sentAMessage = false;
        }
        else {
            auto now = Units::TimePoint::now();
            while (Units::TimePoint::now() < now + _refreshRate) {
                auto Trame = this->waitForMessage();
                shallNotify = this->getBuffer().addMessage(Trame);
            }
            if (shallNotify) {
                caller->notify();
                shallNotify = false;
            }
        }
    }
}

Trame CanListener::waitForMessage() {

    auto Trame = _can->recevoirTrame();
    return Trame;

}

Trame CanListener::getMessage(bool oldest) {

    Trame result = this->getBuffer().retrieveMessageMatchingFilter(_filter, oldest);
    return result;
}

bool CanListener::shallStopListening() {
    return _shallStopListening;
}

void CanListener::setFilter(std::string newfilter) {

    _filter = newfilter;

}


Message_Buffer &CanListener::getBuffer() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _buffer;
}

bool CanListener::sendMessage(const Trame &trame) {
    _can->envoyerTrame(trame);
    this->getBuffer().addMessage(trame);
    return true;
}
