//
// Created by paul on 15/08/16.
//

#include <iostream>
#include "Monitor.h"
#include "../../robot/Commun/TCPIP.h"

Monitor::Monitor(std::string& port) : Gtk::Window(), _listenerThread(nullptr), _canListener(port) {

    //Threading Stuff

    _dispatcher.connect(sigc::mem_fun(*this, &Monitor::onListenerNotification));

    _listenerThread = std::make_unique<std::thread>([this]{
        _canListener.start(this);
    });




    //Graphic Stuff

    this->set_size_request(800, 600);
    this->set_decorated(true);

    _lowLevelWindow.add(_messageTree);
    _lowLevelWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    _refTreeModel = Gtk::ListStore::create(_message);

    _messageTree.set_model(_refTreeModel);

    /*
    Gtk::TreeModel::Row row = *(_refTreeModel->append());

    row[_message.m_col_id] = 1;
    row[_message.m_col_name] = "Billy Bob";
    row[_message.m_col_number] = 10;
    row[_message.m_col_percentage] = 15;
    */

    _messageTree.append_column("ID", _message._id);
    _messageTree.append_column("Data", _message._data);
    _messageTree.append_column("Ack Received", _message._ackReceived);

    this->add(_topLevelBox);

    _topLevelBox.pack_start(_lowLevelWindow);

    this->set_title("Can Monitor");

    this->show_all();

    this->show_all_children();

}


void Monitor::onListenerNotification() {

    this->updateInterface();

}



void Monitor::updateInterface() {

    std::cout << "Handling Trame" << std::endl;

    auto TrameToHandle = _canListener.getOldestMessage();

    Gtk::TreeModel::Row row = *(_refTreeModel->append());

    row[_message._id] = TrameToHandle.getId();
    row[_message._data] = *TrameToHandle.getDonnees();
    row[_message._ackReceived] = false;

    this->queue_draw();
}
