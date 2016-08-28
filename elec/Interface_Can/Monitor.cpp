//
// Created by paul on 15/08/16.
//

#include <iostream>
#include "Monitor.h"
#include "../../robot/Commun/TCPIP.h"

Monitor::Monitor(std::string& port) : Gtk::Window(), _listenerThread(nullptr), _canListener(port), _sendTrameButton("Envoyer la trame") {

    //-----------Threading Stuff|

    _dispatcher.connect(sigc::mem_fun(*this, &Monitor::onListenerNotification));

    _listenerThread = std::make_unique<std::thread>([this]{
        _canListener.start(this);
    });




    //-----------------GUI Stuff|

    this->set_size_request(800, 600);
    this->set_decorated(true);

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

    _dispatcher.emit();

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
    std::string finalData;
    for (int i =0; i<TrameToHandle.getNbDonnees(); i++) {
         finalData += convertToHexadecimal(TrameToHandle.getDonnee(i)) + " " ;
    }

    row[_message._data] = finalData;

    this->queue_draw();
}
