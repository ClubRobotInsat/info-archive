//
// Created by paul on 15/08/16.
//

#include "Monitor.h"
#include <iomanip>

Monitor::Monitor(std::string& port)
        : Gtk::Window()
        , signal_on_message_received(std::make_shared<Glib::Dispatcher>())
        , _stopListnenerThread(false)
        , _canListener(port, signal_on_message_received, _stopListnenerThread)
        , _pauseButton("Pause")
        , _sendTrameButton("Envoyer la trame")
        , _labelTrameId("Trame ID")
        , _labelTrameType("Trame Type")
        , _labelTrameData("Trame Data") {

	//-----------------GUI Stuff|

	this->set_size_request(800, 600);
	this->set_decorated(true);

	// Connecting the button to their designated function
	_sendTrameButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::sendMessage));
	_pauseButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::tooglePauseMode));

	_frame.set_label("Send a message");
	_frame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	_frame.set_border_width(15);

	// Setting up the Grid for sending messages
	_sendMessageContainer.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	_sendMessageContainer.attach(_pauseButton, 0, 1, 2, 1);

	_sendMessageContainer.attach(_labelTrameId, 15, 1, 7, 1);
	_sendMessageContainer.attach_next_to(_trameId, _labelTrameId, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_labelTrameType, _trameId, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_trameType, _labelTrameType, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_labelTrameData, _trameType, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_trameData, _labelTrameData, Gtk::POS_BOTTOM, 7, 1);
	_sendMessageContainer.attach_next_to(_sendTrameButton, _trameData, Gtk::POS_BOTTOM, 7, 1);


	_lowLevelWindow.add(_messageTree);
	_lowLevelWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	// Creating and initializing the refTreeModel
	_refTreeModel = Gtk::ListStore::create(_message);
	_messageTree.set_model(_refTreeModel);

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
	_frame.add(_sendMessageContainer);
	_topLevelBox.add1(_lowLevelWindow);
	_topLevelBox.add2(_frame);
	_topLevelBox.set_position(200);


	this->set_title("Can Monitor");
	this->show_all();
	this->show_all_children();

	//-----------Threading Stuff|

	//_listenerThread = std::make_unique<std::thread>([this] { _canListener.start(); });
	signal_on_message_received->connect(sigc::mem_fun(*this, &Monitor::notify));
	_canListener.start();
}

void Monitor::notify() {

	// Tell the worker thread to stop sending us signals because we are already processing one
	_canListener.isRequestingData(true);
	auto buffer = _canListener.getTrameReceived();
	onListenerNotification(buffer, false);
	_canListener.isRequestingData(false);
}


void Monitor::onListenerNotification(const std::deque<Trame>& buffer, const bool& colored) const {
	this->handleTrame(buffer, colored);
}

std::string Monitor::convertToHexadecimal(const unsigned int& number) const {
	std::stringstream stream;
	stream << std::setfill('*') << std::hex << std::showbase << number;
	return stream.str();
}


void Monitor::updateInterface(const bool& colored,
                              const std::string& id,
                              const std::string& cmd,
                              const std::string& time,
                              const std::string& data) const {

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

	// this->queue_draw();
}

void Monitor::sendMessage() {
	std::unique_ptr<Trame> message;
	auto sendMessage = true;
	try {
		message = std::make_unique<Trame>(this->buildTrameFromInput());
	} catch(std::runtime_error& e) {
		Gtk::MessageDialog dialog(*this, "Error : no input to build a trame from.");
		dialog.run();
		sendMessage = false;
	}

	if(sendMessage) {
		std::deque<Trame> result;
		result.push_front(*message.release());
		_canListener.sendMessage(result.front());
		this->handleTrame(result, true);
	}
}


Trame Monitor::buildTrameFromInput() const {

	if(this->checkInputs()) {
		int id = std::stoi(_trameId.get_buffer()->get_text(), nullptr, 16);
		int cmd = std::stoi(_trameType.get_buffer()->get_text(), nullptr, 16);
		std::vector<uint8_t> data = buildTrameData(_trameData.get_buffer()->get_text());

		Trame result = make_trame(id, cmd, data);

		return result;
	} else {
		throw std::runtime_error("Please input some data in all fields.");
	}
}

bool Monitor::checkInputs() const {
	return !(_trameId.get_buffer()->get_text().empty() | _trameData.get_buffer()->get_text().empty() |
	         _trameType.get_buffer()->get_text().empty());
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

void Monitor::handleTrame(const std::deque<Trame>& buffer, const bool& isColored) const {

	for(auto Trame : buffer) {
		auto id = convertToHexadecimal(Trame.getId());
		auto cmd = convertToHexadecimal(Trame.getCmd());
		auto time = this->getLocalTime();
		std::string data;
		for(int i = 0; i < Trame.getNbDonnees(); i++) {
			data += convertToHexadecimal(Trame.getDonnee(i)) + " ";
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
