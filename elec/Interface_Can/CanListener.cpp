//
// Created by paul on 18/08/16.
//

#include "CanListener.h"
#include "Monitor.h"

CanListener::CanListener(std::string& port, Monitor* caller, bool& stopBooleanAdress)
        : _sentAMessage(false), _shallStopListening(stopBooleanAdress), _acceptNewMessage(true), _refreshRate(20_ms) {

	if(port.substr(0, 5) == "TCPIP") {
		_can.reset(new Commun::CAN(std::make_unique<Commun::TCPIP>(port.substr(7, port.length() - 12), 1234)));
		std::cout << "CAN listening on : " << port.substr(7, port.length() - 12) << ":1234" << std::endl;
	} else {
		_can.reset(new Commun::CAN(std::make_unique<Commun::RS232>("/dev/" + port)));
		std::cout << "CAN listening on : "
		          << "/dev/" + port << std::endl;
	}

	_can->setTemporisation(10_ms);
	_on_message_received.connect(sigc::mem_fun(caller, &Monitor::notify));
}

void CanListener::start() {
	this->mainLoop();
}

void CanListener::mainLoop() {

	while(!this->shallStopListening()) {
		if(_acceptNewMessage) {
			std::unique_ptr<Trame> trame;
			auto now = Units::TimePoint::now();
			while(Units::TimePoint::now() < now + _refreshRate) {
				trame = std::make_unique<Trame>(this->waitForMessage());
				if(not(trame->getId() == 0 or trame->getCmd() == 0)) {
					_on_message_received.emit(*trame.get());
				}
			}
		} else {
			sleep(sleepTime);
		}
	}
}

Trame CanListener::waitForMessage() {
	auto Trame = _can->recevoirTrame();
	return Trame;
}

bool CanListener::shallStopListening() {
	return _shallStopListening;
}

void CanListener::sendMessage(const Trame& trame) {
	_can->envoyerTrame(trame);
}

void CanListener::toogleAcceptNewMessage() {

	_acceptNewMessage = !_acceptNewMessage;
}

CanListener::~CanListener() {

	_shallStopListening = true;
	sleep(120_ms);
}
