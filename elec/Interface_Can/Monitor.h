//
// Created by paul on 15/08/16.
//

#ifndef ROOT_MONITOR_H
#define ROOT_MONITOR_H


#include "CanListener.h"
#include "Header.h"
#include "Message.h"

class Monitor : public Gtk::Window {

public:
	/**
	 *
	 * @param port A string in the following form :
	 * either TCPIP:MY.IP.ADRESS.CONNECTION (the port is setted to 1234)
	 * either /dev/
	 * ttyUSB37
	 */
	Monitor(std::string& port);

	~Monitor();

	/**
	 * Method called by the listener thread to notify the main thread that a message has been received
	 */
	void notify();

	/**
	 * Create a new trame from the data in _trameData, _trameId, _trameType and send it to the CAN
	 */
	void sendMessage();

protected:
	/**
	* Return a string HH:MM:SS
	* For example : 16:34:10 if it is 16 hour past 34 minutes and 10 seconds
	*/
	std::string getLocalTime();

	/**
	 * Method called by notify, wich will call updateInterface
	 * @param colored : if the line shall be drawn in pink, send true, otherwise send false en it will be drawn normally
	 */
	void onListenerNotification(bool colored);

	/**
	 * Add a line to the _refTreeModel
	 */
	void updateInterface(bool colored);

	/**
	 *
	 * @param The raw data to represent in hexadecimal
	 * @return A string in the form 0x.. 0x.. 0x.. , etc.
	 */
	std::string convertToHexadecimal(unsigned int number);

	// int convertToHexadecimal(unsigned int number);

	/**
	 *
	 * @return a Trame object with the data from _trameID, _trameType and _trameData
	 * @throw std::runtime_error if checkInputs() return false
	 */
	Trame buildTrameFromInput();

	/**
	 * Will look at _trameData _trameType _trameId and verify that the fields are not empty.
	 * @return True if there is something as an input, false is one of the 3 is empty.
	 */
	bool checkInputs();

	/**
	 * @brief Toogle the reception of new message.
	 */
	void tooglePauseMode();

    /**
     * Will scroll _lowLevelWindow all the way to the top
     */
    void scrollToTop();

    /**
     * Called by updateInterface. Scroll to the top if the app is not paused.
     */
    void autoscroll();

	template <typename... Args>
	Trame make_trame(uint8_t id, uint8_t cmd, Args&&... donnees) {
		Trame t(id, cmd);
		t.addDonnees(donnees...);
		return t;
	}

private:
	/**
	 * Threading related members
	 */
	// Glib::Dispatcher _dispatcher;
	std::unique_ptr<std::thread> _listenerThread;
	CanListener _canListener;
	//mutable std::mutex mutex;
	/**
	 * Top level GUI elements
	 */
	Gtk::Paned _topLevelBox;
	Gtk::Frame _frame;
	Gtk::Grid _sendMessageContainer;
	/**
	 * GUI elements related to sending message
	 */
	Gtk::Button _pauseButton;
	Gtk::Button _sendTrameButton;

	Gtk::Entry _trameData;
	Gtk::Entry _trameId;
	Gtk::Entry _trameType;

	Gtk::Label _labelTrameId;
	Gtk::Label _labelTrameType;
	Gtk::Label _labelTrameData;

	/**
	 * GUI Elements related to the viewing of messages
	 */
	Gtk::ScrolledWindow _lowLevelWindow;

	Glib::RefPtr<Gtk::ListStore> _refTreeModel;

	Gtk::TreeView _messageTree;

	Message _message;
};


#endif // ROOT_MONITOR_H
