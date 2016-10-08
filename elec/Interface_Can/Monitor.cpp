//
// Created by paul on 15/08/16.
//

#include <iostream>
#include "Monitor.h"
#include <iomanip>

Monitor::Monitor(std::string& port) : Gtk::Window(),
                                      _listenerThread(nullptr),
                                      _canListener(port),
                                      _sendTrameButton("Envoyer la trame"),
                                      _pauseButton("Pause"),
                                      _labelTrameId("Trame ID"),
                                      _labelTrameType("Trame Type"),
                                      _labelTrameData("Trame Data"){

    //-----------Threading Stuff|

    //_dispatcher.connect(sigc::mem_fun(*this, &Monitor::onListenerNotification));

    _listenerThread = std::make_unique<std::thread>([this]{
        _canListener.start(this);
    });




    //-----------------GUI Stuff|

    this->set_size_request(800, 600);
    this->set_decorated(true);

    //Connecting the button to their designated function
    _sendTrameButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::sendMessage));
    _pauseButton.signal_clicked().connect(sigc::mem_fun(*this, &Monitor::tooglePauseMode));

    _frame.set_label("Send a message");
    _frame.set_label_align(Gtk::ALIGN_CENTER, Gtk::ALIGN_START);
    _frame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
    _frame.set_border_width(15);

    //Setting up the Grid for sending messages
    _sendMessageContainer.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    _sendMessageContainer.attach(_pauseButton, 0 , 1 , 2, 1);

    _sendMessageContainer.attach(_labelTrameId, 15, 1, 7, 1);
    _sendMessageContainer.attach_next_to(_trameId, _labelTrameId, Gtk::POS_BOTTOM,7,1);
    _sendMessageContainer.attach_next_to(_labelTrameType, _trameId, Gtk::POS_BOTTOM, 7,1);
    _sendMessageContainer.attach_next_to(_trameType, _labelTrameType, Gtk::POS_BOTTOM, 7, 1);
    _sendMessageContainer.attach_next_to(_labelTrameData,_trameType, Gtk::POS_BOTTOM, 7,1);
    _sendMessageContainer.attach_next_to(_trameData, _labelTrameData, Gtk::POS_BOTTOM, 7, 1);
    _sendMessageContainer.attach_next_to(_sendTrameButton, _trameData, Gtk::POS_BOTTOM, 7, 1);


    _lowLevelWindow.add(_messageTree);
    _lowLevelWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    //Creating and initializing the refTreeModel
    _refTreeModel = Gtk::ListStore::create(_message);
    _messageTree.set_model(_refTreeModel);
    _messageTree.append_column("Date", _message._time);
    _messageTree.append_column("ID", _message._id);
    _messageTree.append_column("Cmd", _message._cmd);
    _messageTree.append_column("Data", _message._data);




    this->add(_topLevelBox);
    _frame.add(_sendMessageContainer);
    _topLevelBox.add1(_lowLevelWindow);
    _topLevelBox.add2(_frame);
    _topLevelBox.set_position(200);



    this->set_title("Can Monitor");
    this->show_all();
    this->show_all_children();

}

void Monitor::notify() {

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

    auto TrameToHandle = _canListener.getMessage(false);

    Gtk::TreeModel::Row row = *(_refTreeModel->prepend());

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
    std::cout << "Je passe" << std::endl;
    std::unique_ptr<Trame> message;
    auto sendMessage = true;
    try {
        message = std::make_unique<Trame>(this->buildTrameFromInput());
    }
    catch (std::runtime_error &e) {
        Gtk::MessageDialog dialog(*this, "Error : no input to build a trame from.");
        dialog.run();
        sendMessage = false;
    }
    std::cout << "SendMessage is : " << sendMessage << std::endl;
    if (sendMessage) {
        Trame &result = *message.release();
        _canListener.sendMessage(result);
        this->notify();
        std::cout << "Message sent succesfully.";
    }

}



Trame Monitor::buildTrameFromInput() {

    if (this->checkInputs()) {
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

void Monitor::tooglePauseMode() {
    this->_canListener.getBuffer().toogleAcceptNewMessage();
}

Monitor::~Monitor() {

    //_listenerThread->join();
    _listenerThread.release();
    _refTreeModel.release();

}
