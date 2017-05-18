//
// Created by paul on 15/08/16.
//

#include "Monitor.h"
#include <iomanip>
#include <log/Log.h>

Monitor::Monitor(std::string& port)
        : Gtk::Window()
        , signal_on_message_received(std::make_shared<Glib::Dispatcher>())
        , _stopListenerThread(false)
        , _canListener(port, signal_on_message_received, _stopListenerThread)
        , _pauseButton("Pause")
        , _sendTrameButton("Envoyer la trame")
        , _labelTrameId("Trame ID")
        , _labelTrameType("Trame Cmd")
        , _labelTrameData("Trame Data")
        , _toggleAllIDs("Toggle all") {

	//-----------------GUI Stuff|

	this->set_size_request(800, 600);
	this->set_decorated(true);

	// Connecting the button to their designated function
	_sendTrameButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::sendMessage));
	_pauseButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::tooglePauseMode));
	_toggleAllIDs.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::onToggleAllClicked));

	_rightFrame.set_label("Send a message");
	_rightFrame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_rightFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	_rightFrame.set_border_width(15);

	// Setting up the Grid for sending messages
	_sendMessageContainer.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	_sendMessageContainer.attach(_pauseButton, 0, 1, 2, 1);

	_sendMessageContainer.attach(_labelTrameId, 0, 1, 7, 1);
	_sendMessageContainer.attach_next_to(_trameId, _labelTrameId, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_labelTrameType, _trameId, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_trameType, _labelTrameType, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_labelTrameData, _trameType, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_trameData, _labelTrameData, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_sendTrameButton, _trameData, Gtk::POS_BOTTOM, 7, 1);

	_trameId.set_adjustment(Gtk::Adjustment::create(0, 0, 1, 1, 1, 1));
	_trameId.set_increments(1, 5);
	_trameId.set_range(0, 10);
	_trameId.set_digits(0);

	_lowLevelWindow.add(_messageTree);
	_lowLevelWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	// Setting up the filter
	for(int i = 0; i < 10; i++) {
		std::string buttonLabel = "Show ID " + std::to_string(i);
		std::shared_ptr<ButtonWithId> button = std::make_shared<ButtonWithId>(buttonLabel, i);
		_sendMessageContainer.attach(*button->_button.get(), 18, 1 + i, 10, 1);
		_buttonIdList.push_back(button);
	}
	_sendMessageContainer.attach(_toggleAllIDs, 18, 12, 10, 1);


	// Creating and initializing the refTreeModel
	_refTreeModel = Gtk::ListStore::create(_message);
	_messageTree.set_model(_refTreeModel);
	_messageTree.set_vscroll_policy(Gtk::ScrollablePolicy::SCROLL_MINIMUM);

	_messageTree.append_column("Date", _message._time);
	_messageTree.append_column("ID", _message._id);
	_messageTree.append_column("Cmd", _message._cmd);
	_messageTree.append_column("Data", _message._data);

	for(int i = 0; i < 4; i++) {
		auto column = _messageTree.get_column(i);
		auto cellRenderer = column->get_first_cell();
		column->add_attribute(cellRenderer->property_cell_background(), _message._color);
	}

	this->add(_topLevelBox);
	_rightFrame.add(_sendMessageContainer);
	_topLevelBox.add1(_lowLevelWindow);
	_topLevelBox.add2(_rightFrame);
	_topLevelBox.set_position(200);


	this->set_title("Can Monitor");
	this->show_all();
	this->show_all_children();

	//-----------Threading Stuff|

	signal_on_message_received->connect(sigc::mem_fun(*this, &Monitor::notify));
	_canListener.start();
}


bool Monitor::on_key_release_event(GdkEventKey* event) {
	if(event->keyval == 32 and
	   not(_trameData.property_is_focus().get_value() or _trameId.property_is_focus().get_value() or
	       _trameType.property_is_focus().get_value())) {
		_pauseButton.clicked();
		return true;
	} else {
		return Gtk::Window::on_key_release_event(event);
	}
}

void Monitor::notify() {

	// Tell the worker thread to stop sending us signals because we are already processing one
	_canListener.isRequestingData(true);
	auto buffer = _canListener.getTrameReceived();
	onListenerNotification(this->filterBuffer(buffer, this->generateIdSet()), false);
	// We tell the worker thread that he can send us signals again once we finished processing AND displaying the data.
	_canListener.isRequestingData(false);
}


void Monitor::onListenerNotification(const std::deque<Trame>& buffer, const bool& colored) {
	this->handleTrame(buffer, colored);
}

std::string Monitor::convertToHexadecimal(const unsigned int& number, bool addPrefix) const {
	std::stringstream stream;
	if(addPrefix) {
		stream << "0x";
	}
	stream << std::setfill('*') << std::hex << number;
	return stream.str();
}


void Monitor::updateInterface(const bool& colored, const std::string& id, const std::string& cmd, const std::string& time, const std::string& data) {

	Gtk::TreeModel::Row row = *(_refTreeModel->prepend());

	row[_message._id] = id;
	row[_message._cmd] = cmd;
	row[_message._time] = time;
	row[_message._data] = data;


	if(colored) {
		row[_message._color] = "pink";
	} else {
		row[_message._color] = "white";
	}

	// This is the part of the code managing the autoscroll functionnality.
	auto adj = this->_lowLevelWindow.get_vadjustment();
	adj->set_value(adj->get_lower());
}

void Monitor::sendMessage() {
	std::unique_ptr<Trame> message;
	auto sendMessage = true;
	try {
		message = std::make_unique<Trame>(this->buildTrameFromInput());
	} catch(std::runtime_error& e) {
		Gtk::MessageDialog dialog(*this, e.what());
		dialog.run();
		sendMessage = false;
	}

	std::deque<Trame> result;
	if(sendMessage) {
		std::deque<Trame> temp;
		temp.push_front(*message.release());
		_canListener.sendMessage(temp.front());
		this->handleTrame(temp, true);
	}
}


Trame Monitor::buildTrameFromInput() const {

	if(this->checkInputs()) {
		try {
			int id = std::stoi(_trameId.get_buffer()->get_text(), nullptr, 10);
			int cmd = std::stoi(_trameType.get_buffer()->get_text(), nullptr, 16);
			std::vector<uint8_t> data = buildTrameData(_trameData.get_buffer()->get_text());

			// No data because we will add it later
			Trame result = make_trame(id, cmd);
			// Adding the data to the Trame
			for(auto item : data) {
				result.addDonnees(item);
			}

			return result;
		} // Catch les erreurs de stoi
		catch(std::exception& e) {
			throw std::runtime_error("Please input some valid data.");
		}
	} else {
		throw std::runtime_error("Please input some data in all fields.");
	}
}

bool Monitor::checkInputs() const {
	bool testId = _trameId.get_buffer()->get_text().empty();
	bool testData = _trameData.get_buffer()->get_text().empty();
	bool testCmd = _trameType.get_buffer()->get_text().empty();
	return !(testId or testData or testCmd);
}

std::string Monitor::getLocalTime() const {
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S");
	auto str = oss.str();
	return str;
}

void Monitor::tooglePauseMode() {
	this->_canListener.toogleAcceptNewMessage();
}

Monitor::~Monitor() {}

void Monitor::handleTrame(const std::deque<Trame>& buffer, const bool& isColored) {

	for(auto Trame : buffer) {
		auto id = convertToHexadecimal(Trame.getId(), true);
		auto cmd = convertToHexadecimal(Trame.getCmd(), true);
		auto time = this->getLocalTime();
		std::string data;
		for(int i = 0; i < Trame.getNbDonnees(); i++) {
			data += convertToHexadecimal(Trame.getDonnee(i), i == 0) + " ";
		}
		this->updateInterface(isColored, id, cmd, time, data);
	}
}

std::vector<uint8_t> Monitor::buildTrameData(const std::string& data) const {

	// all the hex data in the string
	std::vector<uint8_t> result;
	// list of all data in the string seperated by the ' ' character
	std::vector<std::string> wordlist;
	// we populate it with the data from the argument
	std::string temp_word;
	for(int i = 0; i <= data.size(); i++) {
		char character = data[i];
		// Check if we are at the end so that we could add the last word that does not ends by a ' '
		if(i == data.size()) {
			temp_word += character;
			wordlist.push_back(temp_word);
		} else {
			if(character != ' ') {
				temp_word += character;
			} else {
				wordlist.push_back(temp_word);
				temp_word.clear();
			}
		}
	}

	// We remove the "0x" from the first word
	temp_word.clear();
	for(int i = 0; i <= wordlist.front().size(); i++) {
		if(i >= 2) {
			temp_word += wordlist.front()[i];
		}
	}
	wordlist.front() = temp_word;
	temp_word.clear();

	// Now we populate the result
	for(auto& word : wordlist) {
		result.push_back(std::stoi(word, nullptr, 16));
	}

	return result;
}

std::deque<Trame> Monitor::filterBuffer(const std::deque<Trame>& buffer, const std::set<int>& acceptableIDs) const {

	std::deque<Trame> result;

	for(auto& trame : buffer) {
		// Verify if the ID is in the list
		if(acceptableIDs.find(trame.getId()) != acceptableIDs.end()) {
			result.push_back(trame);
		}
	}


	return result;
}

std::set<int> Monitor::generateIdSet() const {
	std::set<int> result;
	for(int i = 0; i < _buttonIdList.size(); i++) {
		auto button = _buttonIdList[i];
		if(!button->_button->get_active()) {
			result.insert(button->_id);
		}
	}
	return result;
}

void Monitor::onToggleAllClicked() {

	for(auto& buttons : _buttonIdList) {
		buttons->_button->set_active(_toggleAllIDs.get_active());
	}
}
