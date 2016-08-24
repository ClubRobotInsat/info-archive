//
// Created by paul on 15/08/16.
//

#ifndef ROOT_MONITOR_H
#define ROOT_MONITOR_H


#include "Header.h"
#include "Message.h"
#include "CanListener.h"

class Monitor : public Gtk::Window {

public :

    /**
     *
     * @param port A string in the following form :
     * either TCPIP:MY.IP.ADRESS.CONNECTION (the port is setted to 1234)
     * either /dev/ttyUSB37
     */
    Monitor(std::string& port);

    void notify();

protected:

    void onListenerNotification();

    void updateInterface();

private:

    Glib::Dispatcher _dispatcher;

    std::unique_ptr<std::thread> _listenerThread;

    CanListener _canListener;

    Gtk::Paned _topLevelBox;

    Gtk::Grid _sendMessageBox;

    Gtk::Frame _frame;

    Gtk::Button _sendTrameButton;

    Gtk::Entry _trameData;

    Gtk::Entry _trameId;

    Gtk::Entry _trameType;

    Gtk::Entry _trameManual;

    Gtk::ScrolledWindow _lowLevelWindow;

    Glib::RefPtr<Gtk::ListStore> _refTreeModel;

    Gtk::TreeView _messageTree;

    Message _message;

};


#endif //ROOT_MONITOR_H
