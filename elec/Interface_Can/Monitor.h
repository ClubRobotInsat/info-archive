//
// Created by paul on 15/08/16.
//

#ifndef ROOT_MONITOR_H
#define ROOT_MONITOR_H


#include "CanListener.h"
#include "Header.h"
#include "Message.h"
#include <set>

/**
 * A simple struct to hold a toogle button with the ID attached to it. Usefull for filtering elements.
 */
struct ButtonWithId {
public:
	ButtonWithId(const std::string& label, const int ID) : _button(std::make_shared<Gtk::ToggleButton>(label)) {
		_id = ID;
	}
	std::shared_ptr<Gtk::ToggleButton> _button;
	int _id;
};

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
	 * Method called by the listener thread to notify the main thread that a message has been received.
	 * Call onListenerNotification(true);
	 */
	void notify();

	/**
	 * Create a new trame from the data in _trameData, _trameId, _trameType and send it to the CAN
	 */
	void sendMessage();

	/**
	 * Will call update interface after splitting the Trame data.
	 *
	 * @param Trame : the trame to display on the GUI
	 */
	void handleTrame(const std::deque<Trame>& buffer, const bool& isColored) const;

protected:
	/**
	 * A callback function for Monitor::_toggleAllIDs
	 * Set the state to each button in Monitor::_buttonIdList to be the same as Monitor::_toggleAllIDs
	 */
	void onToggleAllClicked();

	/**
	 * This method looks at Monitor::_buttonIdList and for each button, if it is toggled, add its ID to the returned
	 * set.
	 * @return A set with all the IDs of all the buttons that are toogled
	 */
	std::set<int> generateIdSet() const;

	/**
	 *
	 * @param buffer : the buffer of Trame to process
	 * @param acceptableIDs : a set of all the IDs that should be shown
	 * @return A new buffer with only the relevant ID. Keep the original trame order (aka the 1st Trame will still be
	 * the first).
	 */
	std::deque<Trame> filterBuffer(const std::deque<Trame>& buffer, const std::set<int>& acceptableIDs) const;

	/**
	* Return a string HH:MM:SS
	* For example : 16:34:10 if it is 16 hour past 34 minutes and 10 seconds
	*/
	std::string getLocalTime() const;

	/**
	 * Method called by notify, wich will call updateInterface
	 * @param colored : if the line shall be drawn in pink, send true, otherwise send false en it will be drawn normally
	 * @param trame : the trame that will be handled by the GUI
	 */
	void onListenerNotification(const std::deque<Trame>& buffer, const bool& colored) const;

	/**
	 * Add a line to the _refTreeModel  with the data passed to the function
	 * @param colored : if the line should be in pink or not
	 * @param id : the ID in hexadecimal
	 * @param cmd : the CMD of the Trame in hexa
	 * @param time : the time.
	 * @param data : An array of string
	 */
	void updateInterface(const bool& colored, const std::string& id, const std::string& cmd, const std::string& time, const std::string& data) const;

	/**
	 *
	 * @param The raw data to represent in hexadecimal
	 * @return A string in the form 0x.. 0x.. 0x.. , etc.
	 */
	std::string convertToHexadecimal(const unsigned int& number) const;

	/**
	 *
	 * @param data : a string formatted to represent some data in hexadecimal with the following shape 0x01 13f 07 4e
	 * @return a vector with all the pertinent information splitted appart, in this example it would be [01, 13f, 07,
	 * 4e]
	 */
	std::vector<uint8_t> buildTrameData(const std::string& data) const;

	/**
	 *
	 * @return a Trame object with the data from _trameID, _trameType and _trameData
	 * @throw std::runtime_error if checkInputs() return false
	 */
	Trame buildTrameFromInput() const;

	/**
	 * Will look at _trameData _trameType _trameId and verify that the fields are not empty.
	 * @return True if there is something as an input, false is one of the 3 is empty.
	 * @return True if there is something as an input, false is one of the 3 is empty.
	 */
	bool checkInputs() const;

	/**
	 * @brief Toogle the reception of new message.
	 */
	void tooglePauseMode();

	template <typename... Args>
	Trame make_trame(const uint8_t& id, const uint8_t& cmd, Args&&... donnees) const {
		Trame t(id, cmd);
		t.addDonnees(donnees...);
		return t;
	}

private:
	/**
	 * Threading related members
	 */
	std::shared_ptr<Glib::Dispatcher> signal_on_message_received;
	bool _stopListnenerThread;
	CanListener _canListener;

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

	std::vector<std::shared_ptr<ButtonWithId>> _buttonIdList;

	Gtk::ToggleButton _toggleAllIDs;
};


#endif // ROOT_MONITOR_H
