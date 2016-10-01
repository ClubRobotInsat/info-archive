//
// Created by paul on 15/08/16.
//

#include <iostream>
#include "Monitor.h"
#include <iomanip>

Monitor::Monitor(std::string& port) : Gtk::Window(), _listenerThread(nullptr), _canListener(port), _sendTrameButton("Envoyer la trame") {

    //-----------Threading Stuff|

    //_dispatcher.connect(sigc::mem_fun(*this, &Monitor::onListenerNotification));

    _listenerThread = std::make_unique<std::thread>([this]{
        _canListener.start(this);
    });




    //-----------------GUI Stuff|

    this->set_size_request(800, 600);
    this->set_decorated(true);

    _sendTrameButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::sendMessage));

    _frame.set_label("Send a message");
    _frame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
    _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _frame.set_border_width(15);

    _sendMessageBox.set_row_homogeneous(true);
    _sendMessageBox.set_column_homogeneous(true);
    _sendMessageBox.set_column_spacing(15);

    _sendMessageBox.add(_trameData);
    _sendMessageBox.add(_trameId);
    _sendMessageBox.add(_trameType);
    _sendMessageBox.attach_next_to(_sendTrameButton, _trameId, Gtk::POS_BOTTOM, 1,1);

    _lowLevelWindow.add(_messageTree);
    _lowLevelWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    _refTreeModel = Gtk::ListStore::create(_message);
    _messageTree.set_model(_refTreeModel);
    _messageTree.append_column("Date", _message._time);
    _messageTree.append_column("ID", _message._id);
    _messageTree.append_column("Cmd", _message._cmd);
    _messageTree.append_column("Data", _message._data);


    this->add(_topLevelBox);

    _frame.add(_sendMessageBox);
    _topLevelBox.add1(_lowLevelWindow);
    _topLevelBox.add2(_frame);
    _topLevelBox.set_position(200);



    this->set_title("Can Monitor");
    this->show_all();
    this->show_all_children();

}

void Monitor::notify() {

    //_dispatcher.emit();
    onListenerNotification();

}


void Monitor::onListenerNotification() {

    this->updateInterface();

}

std::string Monitor::convertToHexadecimal(unsigned int number) {
    std::stringstream stream;
    stream << "0x" << std::hex << number;
    return stream.str();
}



void Monitor::updateInterface() {

    auto TrameToHandle = _canListener.getOldestMessage();

    Gtk::TreeModel::Row row = *(_refTreeModel->append());

    row[_message._id] = convertToHexadecimal(TrameToHandle.getId());
    row[_message._cmd] = convertToHexadecimal(TrameToHandle.getCmd());
    row[_message._time] = this->getLocalTime();
    std::string finalData;

    for (int i =0; i < TrameToHandle.getNbDonnees(); i++) {
         finalData += convertToHexadecimal(TrameToHandle.getDonnee(i)) + " " ;
    }

    row[_message._data] = finalData;

    this->queue_draw();
}

void Monitor::sendMessage() {
    /*
    std::vector<Trame> test;
    test.push_back(make_trame(0x00_b, 0x00_b, 0x55_b));
    test.push_back(make_trame(0x01_b, 0x00_b, 0x55_b));
    test.push_back(make_trame(0x02_b, 0x00_b, 0x55_b));
    test.push_back(make_trame(0x03_b, 0x00_b, 0x55_b));
    test.push_back(make_trame(0x04_b, 0x00_b, 0x55_b));
     */
    std::unique_ptr<Trame> message;
    auto sendMessage = true;
    try {
        message = std::make_unique<Trame>(this->buildTrameFromInput());
    }
    catch (std::runtime_error &e) {
        Gtk::MessageDialog dialog(*this, "Erreur, vous n'avez pas fourni de valeur pour une entrée");
        dialog.run();
        sendMessage = false;
    }
    if (sendMessage) {
        Trame &result = *message.release();
        _canListener.sendMessage(result);
    }
}



Trame Monitor::buildTrameFromInput() {

    if (checkInputs()) {
        Trame result = make_trame(std::stoi(_trameId.get_buffer()->get_text()),
                                  std::stoi(_trameType.get_buffer()->get_text()),
                                  std::stoi(_trameData.get_buffer()->get_text()));

        return result;
    }
    else {
        throw std::runtime_error("Some fields are empty...");
    }
}

bool Monitor::checkInputs() {
    return !(_trameId.get_buffer()->get_text().empty() |
            _trameData.get_buffer()->get_text().empty() |
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
