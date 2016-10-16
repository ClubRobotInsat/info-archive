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
	 * @return
	 */
	CanListener(std::string& port, Monitor* caller, bool& stopBooleanAdress);

	virtual ~CanListener();

	/**
	 * Enter the listening loop. This function will return if _shallStopListening is set to False.
	 */
	void start();

	/**
	 * Toogle the variable _acceptNewMessage;
	 */
	void toogleAcceptNewMessage();

	void sendMessage(const Trame& trame);

protected:
	/**
	 * Listen if _acceptNewMessage is set to true.
	 * Return if _shallStopListening is set to true;
	 */
	void mainLoop();

	Trame waitForMessage();

	bool shallStopListening();

private:
	Trame _currentTrame;

	std::atomic_bool _sentAMessage;

	std::unique_ptr<Commun::CAN> _can;

	bool& _shallStopListening;

	bool _acceptNewMessage;

	Units::Time _refreshRate;

	sigc::signal<void, Trame> _on_message_received;
};


#endif // ROOT_CANLISTENER_H
