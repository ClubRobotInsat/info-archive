//
// Created by paul on 15/08/16.
//

#include "Monitor.h"
#include <iomanip>
#include <iostream>

Monitor::Monitor(std::string& port)
        : Gtk::Window()
        , _listenerThread(nullptr)
        , _stopListnenerThread(false)
        , _canListener(port, this, _stopListnenerThread)
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

    _listenerThread = std::make_unique<std::thread>([this] { _canListener.start(); });

}

void Monitor::notify(Trame trame) {

	onListenerNotification(trame, false);
}


void Monitor::onListenerNotification(Trame trame, bool colored) {

	this->handleTrame(trame, colored);
}

std::string Monitor::convertToHexadecimal(unsigned int number) {
	std::stringstream stream;
	stream << "0x" << std::hex << number;
	return stream.str();
}


void Monitor::updateInterface(bool colored, std::string id, std::string cmd, std::string time, std::string data) {

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

	this->queue_draw();
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
		Trame result = *message.release();
		_canListener.sendMessage(result);
		this->handleTrame(result, true);
	}
}


Trame Monitor::buildTrameFromInput() {

	if(this->checkInputs()) {
		int id = std::stoi(_trameId.get_buffer()->get_text(), nullptr, 16);
		int cmd = std::stoi(_trameType.get_buffer()->get_text(), nullptr, 16);
		int data = std::stoi(_trameData.get_buffer()->get_text(), nullptr, 16);

		Trame result = make_trame(id, cmd, data);

		return result;
	} else {
		throw std::runtime_error("Some fields are empty...");
	}
}

bool Monitor::checkInputs() {
	return !(_trameId.get_buffer()->get_text().empty() | _trameData.get_buffer()->get_text().empty() |
	         _trameType.get_buffer()->get_text().empty());
}

std::string Monitor::getLocalTime() {
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

Monitor::~Monitor() {

}

void Monitor::scrollToTop() {
	_lowLevelWindow.get_focus_vadjustment()->set_value(_lowLevelWindow.get_vadjustment()->get_upper());
}

void Monitor::handleTrame(Trame& Trame, bool isColored) {

	auto id = convertToHexadecimal(Trame.getId());
	auto cmd = convertToHexadecimal(Trame.getCmd());
	auto time = this->getLocalTime();
	std::string data;
	for(int i = 0; i < Trame.getNbDonnees(); i++) {
		data += convertToHexadecimal(Trame.getDonnee(i)) + " ";
	}

	this->updateInterface(isColored, id, cmd, time, data);
}

void Monitor::endListenerThread() {

    _stopListnenerThread = true;
    sleep(120_ms);
    _listenerThread->join();

}
