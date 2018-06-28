//
// Created by paul on 18/08/16.
//

#ifndef ROOT_CANLISTENER_H
#define ROOT_CANLISTENER_H
#include "../../robot/Commun/Communication/CAN.h"
#include "../../robot/Commun/Communication/RS232.h"
#include "../../robot/Commun/Communication/TCPIP.h"
#include "Header.h"
#include <chrono>

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
	CanListener(const std::string& port, const std::shared_ptr<Glib::Dispatcher> receiver_signal_message_received, bool& stopBooleanAdress);

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
	const std::deque<std::pair<Trame, std::chrono::milliseconds>> getTrameReceived();

	/**
	 * Update CanListener::_parentIsRequestingData with value
	 * @param value : the value that Monitor::_parentIsRequestingData will take
	 */
	void isRequestingData(bool value);

protected:
	/**
	 * Listen if _acceptNewMessage is set to true.
	 * Return if _shallStopListening is set to true;
	 */
	void mainLoop();

	/// When _canShouldStop is false, the function returns early, raising an exception.
	Trame waitForMessage();

	/// Stop listening on the CAN. This will raise an exception because we are interrupting the serial connection in
	///  a bad maneer.
	void stopListening();

private:
	mutable std::mutex mutex;

	/// The Object that implements all the fonction for communicating with the Can.
	std::unique_ptr<Commun::CAN> _can;

	/// When this bool is true, the thread will stop as soon as possible.
	bool& _shallStopListening;

	/// When this bool is true, the Can will listen and store any new message.
	std::atomic_bool _acceptNewMessage;

	/// When this bool is true, the GUI thread is waiting for us to release the lock on the message buffer.
	std::atomic_bool _parentIsRequestingData;

	/// The time we sleep before verifying if we have received a new Trame.
	const Units::Time _refreshRate;

	/// The underlying thread.
	Glib::Thread* _thread;

	/// This field stores all the Trame that we receive. It is cleared once it has been transmited to the GUI Thread.
	std::deque<std::pair<Trame, std::chrono::milliseconds>> _trameBuffer;

	/// This is the function that we call when we receive a new message.
	const std::shared_ptr<Glib::Dispatcher> signal_on_message_received;
};


#endif // ROOT_CANLISTENER_H
