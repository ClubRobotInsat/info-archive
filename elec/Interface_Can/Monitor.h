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

    void onListenerNotification();

protected:

    void updateInterface();

private:

    Glib::Dispatcher _dispatcher;

    std::unique_ptr<std::thread> _listenerThread;

    CanListener _canListener;

    Gtk::Box _topLevelBox;

    Gtk::ScrolledWindow _lowLevelWindow;

    Glib::RefPtr<Gtk::ListStore> _refTreeModel;

    Gtk::TreeView _messageTree;

    Message _message;

};


#endif //ROOT_MONITOR_H
