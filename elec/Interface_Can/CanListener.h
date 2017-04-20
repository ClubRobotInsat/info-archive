//
// Created by paul on 18/08/16.
//

#ifndef ROOT_CANLISTENER_H
#define ROOT_CANLISTENER_H
#include "../../robot/Commun/CAN.h"
#include "../../robot/Commun/RS232.h"
#include "../../robot/Commun/TCPIP.h"
#include "Header.h"

/**
 * Represent the time waited between two call to Can::waitForMessage
 * Used to avoid having a while(true) loop in the code
 */
const Duration sleepTime = 5_ms;

class Monitor;

/**
 * A class wich abstracts a Can Listener : shall be started by the start() method, and will return when
 * _shallStopListening is set to false.
 *
 */
class CanListener {

public:
	/**
	 *
	 * @param port : a string
	 * @param caller : an pointer used to connect the signal _on_message_received
	 */
	CanListener(const std::string& port, std::shared_ptr<Glib::Dispatcher> receiver_signal_message_received, bool& stopBooleanAdress);

	virtual ~CanListener();

	/**
	 * Enter the listening loop. This function will return if _shallStopListening is set to False.
	 */
	void start();

	/**
	 * Toogle the variable _acceptNewMessage;
	 */
	void toogleAcceptNewMessage();

	/**
	 * Then the trame given to the CAN. Does not notify the GUI thread that it has sent one.
	 * @param trame : the trame to be sent.
	 */
	void sendMessage(const Trame& trame);

	/**
	 * Empty CanListener::_trameBuffer
	 * Lock until it returns
	 * @return a list with all the trame received.
	 */
	std::deque<Trame> getTrameReceived();

	/**
	 * Update CanListener::_parentIsRequestingData with value
	 * @param value : the value that Monitor::_parentIsRequestingData will take
	 */
	void isRequestingData(const bool& value);

protected:
	/**
	 * Listen if _acceptNewMessage is set to true.
	 * Return if _shallStopListening is set to true;
	 */
	void mainLoop();

	/// When _canShouldStop is false, the function returns early, raising an exception.
	Trame waitForMessage();

	/// Stop listening on the CAN. This will raise an exception because we are interrupting
	void stopListening();

private:
	mutable std::mutex mutex;

	std::atomic_bool _sentAMessage;

	std::unique_ptr<Commun::CAN> _can;

	bool& _shallStopListening;

	std::atomic_bool _canShouldStop;

	std::atomic_bool _acceptNewMessage;

	std::atomic_bool _parentIsRequestingData;

	Units::Time _refreshRate;

	Glib::Thread* _thread;

	std::deque<Trame> _trameBuffer;

	std::shared_ptr<Glib::Dispatcher> signal_on_message_received;
};


#endif // ROOT_CANLISTENER_H
