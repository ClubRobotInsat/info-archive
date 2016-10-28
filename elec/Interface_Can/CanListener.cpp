//
// Created by paul on 18/08/16.
//

#include "CanListener.h"
#include "Monitor.h"

CanListener::CanListener(const std::string& port, std::shared_ptr<Glib::Dispatcher> receiver_signal_message_received, bool& stopBooleanAdress)
        : _sentAMessage(false)
        , _shallStopListening(stopBooleanAdress)
        , _acceptNewMessage(true)
        , _parentIsRequestingData(false)
        , _refreshRate(20_ms)
        , _thread(0)
        , signal_on_message_received(receiver_signal_message_received) {

	if(port.substr(0, 5) == "TCPIP") {
		_can.reset(new Commun::CAN(std::make_unique<Commun::TCPIP>(port.substr(7, port.length() - 12), 1234)));
		std::cout << "CAN listening on : " << port.substr(7, port.length() - 12) << ":1234" << std::endl;
	} else {
		_can.reset(new Commun::CAN(std::make_unique<Commun::RS232>("/dev/" + port)));
		std::cout << "CAN listening on : "
		          << "/dev/" + port << std::endl;
	}

	_can->setTemporisation(10_ms);
}

void CanListener::start() {

	_thread = Glib::Thread::create(sigc::mem_fun(*this, &CanListener::mainLoop), true);
}

void CanListener::mainLoop() {
	auto emitted = false;
	while(!this->shallStopListening()) {
		if(_acceptNewMessage) {
			std::unique_ptr<Trame> trame;
			auto now = Units::TimePoint::now();
			while(Units::TimePoint::now() < now + _refreshRate and not emitted) {
				trame = std::make_unique<Trame>(this->waitForMessage());
				if(not(trame->getId() == 0 or trame->getCmd() == 0)) {
					std::lock_guard<std::mutex> lock(mutex);
					_trameBuffer.push_front(*trame.get());
					if(not _parentIsRequestingData) {
						signal_on_message_received->emit();
					}
					emitted = true;
				}
			}
			emitted = false;
		} else {
			sleep(sleepTime);
		}
	}
}

Trame CanListener::waitForMessage() {
	auto Trame = _can->recevoirTrame();
	return Trame;
}

bool CanListener::shallStopListening() const {
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
	if(_thread) {
		_thread->join();
	}
}

std::deque<Trame> CanListener::getTrameReceived() {
	std::lock_guard<std::mutex> lock(mutex);
	auto result = _trameBuffer;
	_trameBuffer.clear();
	return result;
}

void CanListener::isRequestingData(const bool& value) {
	_parentIsRequestingData = value;
}
