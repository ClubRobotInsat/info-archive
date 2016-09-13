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
     * either /dev/
     * ttyUSB37
     */
    Monitor(std::string& port);

    void notify();

    void sendMessage();

protected:

    void onListenerNotification();

    void updateInterface();

    std::string convertToHexadecimal(unsigned int number);

    Trame buildTrameFromInput();

    /**
     * Will look at @var _trameData @var _trameType @var _trameId and verify that the fields are not empty.
     * @return True if there is something as an input, false is one of the 3 is empty.
     */
    bool checkInputs();

    template<typename... Args>
    Trame make_trame(uint8_t id, uint8_t cmd, Args &&... donnees) {
        Trame t(id, cmd);
        t.addDonnees(donnees...);
        return t;
    }

private:

    //Glib::Dispatcher _dispatcher;

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

    Gtk::Entry _filter;

};


#endif //ROOT_MONITOR_H
