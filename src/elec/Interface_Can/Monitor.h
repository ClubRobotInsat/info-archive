//
// Created by paul on 15/08/16.
//

#ifndef ROOT_MONITOR_H
#define ROOT_MONITOR_H


#include "CanListener.h"
#include "Header.h"
#include "Message.h"
#include "Preset.h"
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
	 * either /dev/ttyUSB37 .
	 */
	Monitor(std::string& port);

	~Monitor();

	/**
	 * Method called by the listener thread to notify the main thread that a message has been received.
	 * Call onListenerNotification(true);
	 */
	void notify();

	/**
	 * Create a new trame from the data in _trameData, _trameId, _trameType and send it to the CAN.
	 */
	void sendMessage();

	/**
	 *  Save the Trame from _trameData in a file
	 */
	void saveTramePreset();

	/**
	 * Will call update interface after splitting the Trame data.
	 * This is the function responsible for handling the spacebar interruption.
	 *
	 * @param Trame : the trame to display on the GUI.
	 */
	void handleTrame(const std::deque<std::pair<Trame, std::chrono::milliseconds>>& buffer, bool isColored);

	/**
	 * Send a Trame for pinging id.
	 * Trame Id : id.
	 * Trame Cmd : 0x00.
	 * Trame Data : [0x55].
	 * This function also set _internalCardData[id] to false.
	 * @param id : the id of the trame that will be sent.
	 */
	void sendPing(uint8_t id);

	/**
	 * Call sendPing() for all id between 0 and 10;
	 */
	void pingAll();

protected:
	/**
	 * The function called when the user press a key while focusing on the window.
	 * @param event : the event with the key.
	 * @return
	 */
	bool on_key_release_event(GdkEventKey* event) override;

	/**
	 * A callback function for Monitor::_toggleAllIDs.
	 * Set the state to each button in Monitor::_buttonIdList to be the same as Monitor::_toggleAllIDs.
	 */
	void onToggleAllClicked();

	/**
	 * This method looks at Monitor::_buttonIdList and for each button, if it is toggled, add its ID to the returned.
	 * set.
	 * @return A set with all the IDs of all the buttons that are toogled.
	 */
	std::set<int> generateIdSet() const;

	/**
	 *
	 * @param buffer : the buffer of Trame to process.
	 * @param acceptableIDs : a set of all the IDs that should be shown.
	 * @return A new buffer with only the relevant ID. Keep the original trame order (aka the 1st Trame will still be
	 * the first).
	 */
	std::deque<std::pair<Trame, std::chrono::milliseconds>>
	    filterBuffer(const std::deque<std::pair<Trame, std::chrono::milliseconds>>& buffer, const std::set<int>& acceptableIDs) const;

	/**
	 * Return a string HH:MM:SS.
	 * For example : 16:34:10 if it is 16 hour past 34 minutes and 10 seconds.
	 */
	std::string getLocalTime() const;

	/**
	 * Method called by notify, wich will call updateInterface.
	 * @param colored : if the line shall be drawn in pink, send true, otherwise send false en it will be drawn
	 * normally.
	 * @param trame : the trame that will be handled by the GUI.
	 */
	void onListenerNotification(const std::deque<std::pair<Trame, std::chrono::milliseconds>>& buffer, bool colored);

	/**
	 * Add a line to the _refTreeModel  with the data passed to the function.
	 * This function is responsible for the autoscroll behavior on the message list.
	 * @param colored : if the line should be in pink or not.
	 * @param id : the ID in hexadecimal.
	 * @param cmd : the CMD of the Trame in hexa.
	 * @param time : the time.
	 * @param data : An array of string.
	 */
	void updateInterface(bool colored, const std::string& id, const std::string& cmd, const std::string& time, const std::string& data);

	/**
	 * @param number : The raw data to represent in hexadecimal
	 * @param addPrefix : If the function should add "0x" to the result
	 * @return A string with the number in an hexadecimal base
	 */
	std::string convertToHexadecimal(const unsigned int& number, bool addPrefix) const;

	/**
	 *
	 * @return a Trame object with the data from _trameID, _trameType and _trameData.
	 * @throw std::runtime_error if checkInputs() return false.
	 */
	Trame buildTrameFromInput() const;

	/**
	 * Will look at _trameData _trameType _trameId and verify that the fields are not empty.
	 * @return True if there is something as an input, false is one of the 3 is empty.
	 */
	bool checkInputs() const;

	/**
	 * @brief Toogle the reception of new message.
	 */
	void tooglePauseMode();

	template <typename... Args>
	Trame make_trame(uint8_t id, uint8_t cmd, Args&&... donnees) const {
		Trame t(id, cmd);
		t.addDonnees(donnees...);
		return t;
	}

	/**
	 * This function is called uppon receiving a certain type of trame that correspond to a "pong".
	 * This function set _internalCardData[id] to true.
	 * @param id : the id of the card that responded "pong".
	 */
	void onPongReceived(uint8_t id);

	/**
	 * @return true if the cursor is in any of the entry field.
	 */
	bool isEntryFieldFocused() const;

private:
	/**
	 * Threading related members.
	 */
	std::shared_ptr<Glib::Dispatcher> signal_on_message_received;
	bool _stopListenerThread;
	CanListener _canListener;

	/**
	 * Top level GUI elements.
	 */
	Gtk::Paned _topLevelBox;

	/// Hold _middleFrame and _rightFrame together.
	Gtk::Paned _frameContainer;

	/// The Frame with alle the button inside.
	Gtk::Frame _rightFrame;

	/// The Frame with all the sending stuf inside.
	Gtk::Frame _middleFrame;

	/// The grid that hold the button.
	Gtk::Grid _buttonLayout;

	/// The grid that hold the GUI for sending messages.
	Gtk::Grid _sendMessageLayout;

	/// the path to file
	std::string _path;

	/// the Preset's variable list
	// PresetArray _tramePreset;


	/**
	 * GUI elements related to the right pane : pinging cards and filtering messages.
	 */

	/// All the button for filtering message.
	std::vector<std::shared_ptr<ButtonWithId>> _buttonIdList;

	/// A button that toogle all the button contained in _buttonIdList.
	Gtk::ToggleButton _toggleAllIDs;

	/// A button that will ping all IDs.
	Gtk::Button _pingAllIDs;

	std::vector<std::shared_ptr<Gtk::Label>> _pingStatus;

	/**
	 * GUI elements related to sending message.
	 */
	Gtk::ToggleButton _pauseButton;
	Gtk::Button _sendTrameButton;
	Gtk::Button _saveTramePreset;

	std::vector<std::shared_ptr<Gtk::Entry>> _dataArray;
	Gtk::SpinButton _trameId;
	Gtk::Entry _trameType;


	Gtk::Label _labelTrameId;
	Gtk::Label _labelTrameType;
	Gtk::Label _labelTrameData;

	/**
	 * GUI Elements related to the viewing of messages.
	 */
	Gtk::ScrolledWindow _lowLevelWindow;

	Glib::RefPtr<Gtk::ListStore> _refTreeModel;

	Gtk::TreeView _messageTree;

	Message _message;

	/**
	 * Internal data for storing card information (pinged, etc.).
	 */
	std::map<int, bool> _internalCardData;
};


#endif // ROOT_MONITOR_H
