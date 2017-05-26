//
// Created by paul on 15/08/16.
//

#include "Monitor.h"
#include "Preset.h"
#include <iomanip>
#include <log/Log.h>

Monitor::Monitor(std::string& port)
        : Gtk::Window()
        , signal_on_message_received(std::make_shared<Glib::Dispatcher>())
        , _stopListenerThread(false)
        , _canListener(port, signal_on_message_received, _stopListenerThread)
        , _path(".can_interface_preset")
        , _tramePreset(_path)
        , _toggleAllIDs("Toggle all")
        , _pingAllIDs("Ping all IDs")
        , _pauseButton("Pause")
        , _sendTrameButton("Send Trame")
        , _saveTramePreset("Save Trame Preset")
        , _labelTrameId("Trame ID")
        , _labelTrameType("Trame Cmd")
        , _labelTrameData("Trame Data")


{

	//-----------------GUI Stuff|

	this->set_size_request(800, 600);
	this->set_decorated(true);

	// Connecting the button to their designated function
	_sendTrameButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::sendMessage));
	_saveTramePreset.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::saveTramePreset));
	_pauseButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::tooglePauseMode));
	_toggleAllIDs.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::onToggleAllClicked));
	_pingAllIDs.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::pingAll));

	// Setting up the two frames
	_rightFrame.set_label("Hide messages and ping cards");
	_rightFrame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_rightFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	_rightFrame.set_border_width(5);

	_middleFrame.set_label("Send a message");
	_middleFrame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
	_middleFrame.set_border_width(5);

	// Pre-Labelling the input fields
	_trameType.get_buffer()->set_text("0x00");

	// Setting up the grid for the trameData input
	for(int k = 0; k < 6; k++) {
		std::shared_ptr<Gtk::Entry> entry = std::make_shared<Gtk::Entry>();
		entry->get_buffer()->set_text("0x00");
		entry->set_max_length(4);
		entry->set_width_chars(4);
		_sendMessageLayout.attach(*entry.get(), k, 10, 1, 1);
		_dataArray.push_back(entry);
	}

	// Setting up the Grid for sending messages
	_sendMessageLayout.attach(_pauseButton, 0, 1, 7, 1);
	_sendMessageLayout.attach(_labelTrameId, 0, 2, 4, 1);
	_sendMessageLayout.attach(_labelTrameType, 4, 2, 3, 1);
	_sendMessageLayout.attach(_trameId, 0, 3, 4, 1);
	_sendMessageLayout.attach(_trameType, 4, 3, 3, 1);
	_sendMessageLayout.attach(_labelTrameData, 0, 4, 7, 1);

	_sendMessageLayout.attach(_sendTrameButton, 0, 11, 7, 1);
	_sendMessageLayout.attach(_saveTramePreset, 0, 12, 7, 1);

	_trameId.set_adjustment(Gtk::Adjustment::create(0, 0, 1, 1, 1, 1));
	_trameId.set_increments(1, 5);
	_trameId.set_range(0, 10);
	_trameId.set_digits(0);

	_trameType.set_width_chars(4);
	_trameType.set_max_length(4);

	_lowLevelWindow.add(_messageTree);
	_lowLevelWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	// Setting up the messgae filter buttons
	for(int i = 0; i < 10; i++) {
		std::string buttonLabel = "Show ID " + std::to_string(i);
		std::shared_ptr<ButtonWithId> button = std::make_shared<ButtonWithId>(buttonLabel, i);
		_buttonLayout.attach(*button->_button.get(), 1, 1 + i, 1, 1);
		_buttonIdList.push_back(button);


		std::shared_ptr<Gtk::Label> label = std::make_shared<Gtk::Label>("No pong !", 0, 0);
		label->override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);
		_buttonLayout.attach(*label.get(), 2, 1 + i, 1, 1);
		_pingStatus.push_back(label);

		_internalCardData.emplace(i, true);
	}
	_buttonLayout.attach(_toggleAllIDs, 1, 11, 1, 1);
	_buttonLayout.attach(_pingAllIDs, 2, 11, 1, 1);


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

	//_lowLevelWindow.event

	// Tree of Widgets
	_middleFrame.add(_sendMessageLayout);
	_rightFrame.add(_buttonLayout);

	_frameContainer.add1(_middleFrame);
	_frameContainer.add2(_rightFrame);


	_topLevelBox.add1(_lowLevelWindow);
	_topLevelBox.add2(_frameContainer);
	_topLevelBox.set_position(350);


	this->set_title("Can Monitor");
	this->show_all();
	this->show_all_children();

	this->pingAll();

	//-----------Threading Stuff|
	signal_on_message_received->connect(sigc::mem_fun(*this, &Monitor::notify));
	_canListener.start();
}


bool Monitor::on_key_release_event(GdkEventKey* event) {
	if(event->keyval == 32 and not(this->isEntryFieldFocused())) {
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


void Monitor::onListenerNotification(const std::deque<std::pair<Trame, std::chrono::milliseconds>>& buffer, bool colored) {
	this->handleTrame(buffer, colored);
}

std::string Monitor::convertToHexadecimal(const unsigned int& number, const bool addPrefix) const {
	std::stringstream stream;
	if(addPrefix) {
		stream << "0x";
	}
	stream << std::setfill('*') << std::hex << number;
	return stream.str();
}


void Monitor::updateInterface(bool colored, const std::string& id, const std::string& cmd, const std::string& time, const std::string& data) {

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

	if(sendMessage) {
		std::deque<std::pair<Trame, std::chrono::milliseconds>> temp;
		std::chrono::milliseconds time =
		    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
		temp.push_front(std::pair<Trame, std::chrono::milliseconds>(*message.release(), time));
		_canListener.sendMessage(temp.front().first);
		this->handleTrame(temp, true);
	}
}

// for the Test of the button call the function, it only print "reussi" when the button is clicked
void Monitor::saveTramePreset() {

	Gtk::ComboBoxText _displayList;
	for(auto preset : _tramePreset.getArray()) {
		_displayList.append(preset.toString());
		_displayList.set_active_text(preset.toString());
	}
	std::cout << "reussi" << std::endl;
};


Trame Monitor::buildTrameFromInput() const {

	if(this->checkInputs()) {
		try {
			const int id = std::stoi(_trameId.get_buffer()->get_text(), nullptr, 10);
			const int cmd = std::stoi(_trameType.get_buffer()->get_text(), nullptr, 16);


			// Build the data string
			std::vector<uint8_t> data;
			for(auto entry : _dataArray) {
				uint8_t d = std::stoi(entry->get_buffer()->get_text(), nullptr, 16);
				data.push_back(d);
			}

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
	const bool testId = _trameId.get_buffer()->get_text().empty();
	const bool testCmd = _trameType.get_buffer()->get_text().empty();
	return !(testId or testCmd);
}

std::string Monitor::getLocalTime() const {
	const auto t = std::time(nullptr);
	const auto tm = *std::localtime(&t);

	std::ostringstream oss;
	oss << std::put_time(&tm, "%H:%M:%S.%%06u");
	auto str = oss.str();
	return str;
}

void Monitor::tooglePauseMode() {
	this->_canListener.toogleAcceptNewMessage();
	this->_pingAllIDs.set_sensitive(not _pingAllIDs.get_sensitive());
}

Monitor::~Monitor() {}

void Monitor::handleTrame(const std::deque<std::pair<Trame, std::chrono::milliseconds>>& buffer, bool isColored) {

	for(const auto pair : buffer) {
		const Trame& trame = pair.first;
		if(trame.getCmd() == 0x00_b and trame.getDonnee(0) == 0xaa_b) {
			this->onPongReceived(trame.getId());
		}

		const auto id = convertToHexadecimal(trame.getId(), true);
		const auto cmd = convertToHexadecimal(trame.getCmd(), true);
		std::string data;
		for(int i = 0; i < trame.getNbDonnees(); i++) {
			data += convertToHexadecimal(trame.getDonnee(i), i == 0) + " ";
		}
		this->updateInterface(isColored, id, cmd, std::to_string(pair.second.count()), data);
	}
}

std::deque<std::pair<Trame, std::chrono::milliseconds>>
    Monitor::filterBuffer(const std::deque<std::pair<Trame, std::chrono::milliseconds>>& buffer,
                          const std::set<int>& acceptableIDs) const {

	std::deque<std::pair<Trame, std::chrono::milliseconds>> result;

	for(auto& pair : buffer) {
		// Verify if the ID is in the list
		if(acceptableIDs.find(pair.first.getId()) != acceptableIDs.end()) {
			result.push_back(pair);
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


void Monitor::sendPing(uint8_t id) {
	auto trame = make_trame(id, 0x00_b, 0x55_b);
	_pingStatus[id]->set_label("No Pong !");
	_pingStatus[id]->override_color(Gdk::RGBA("red"), Gtk::STATE_FLAG_NORMAL);


	_canListener.sendMessage(trame);
	_internalCardData[id] = false;
}

void Monitor::onPongReceived(uint8_t id) {

	_pingStatus[id]->set_label("Connected !");
	_pingStatus[id]->override_color(Gdk::RGBA("green"), Gtk::STATE_FLAG_NORMAL);
	_internalCardData[id] = true;
}

void Monitor::pingAll() {
	for(uint8_t i = 0; i < 10; i++) {
		this->sendPing(i);
	}
}


bool Monitor::isEntryFieldFocused() const {
	bool result = false;
	for(const auto entry : _dataArray) {
		result = result or entry->property_is_focus().get_value();
	}
	result = result or _trameId.property_is_focus().get_value() or _trameType.property_is_focus().get_value();
	return result;
}
