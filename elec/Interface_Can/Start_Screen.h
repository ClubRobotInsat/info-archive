//
// Created by paul on 05/08/16.
//

#ifndef ROOT_START_SCREEN_H
#define ROOT_START_SCREEN_H


#include <bits/unique_ptr.h>
#include <vector>
#include "Header.h"
#include "../../robot/Commun/TCPIP.h"
#include <sigc++/sigc++.h>


class Start_Screen : public Gtk::Window{

    public:


    /**
     *
     */
    Start_Screen();

    virtual ~Start_Screen();

    /**
     * @brief The callback function for _launchCanMonitor
     */
    void onLaunchCanMonitorClicked();


    /**
     *
     */
    typedef sigc::signal<void, std::string> type_startScreenSignalOnExit;
    type_startScreenSignalOnExit startScreenSignalOnExit();

    /**
     * @brief Send a signal through _startScreenSignalOnExit with _canAdress as member of it
     */
    void emitLaunchCanSignal();

protected:

    /**
     * @brief Will update _serialList with all the file in /dev that are relevant (aka /dev/ttyUSB0, etc.)
     * @warning Does not throw any exception
     * @return True if there is some changes in /dev, false otherwise
     */
    bool scanSerialConnection();


    /**
     * @brief Update _displayedList() and ask gtkmm for a redraw
     */
    void updateComboBoxList();

    /**
     * @brief The main loop of the Start_Screen wich consists in scanning /dev and updating _displayedList
     */
    void mainLoop();

    /**
     * @brief Scan 127.0.0.1:1234 looking for a listening socket
     */
    void scanTCPIPConnection();

    private:

    std::string _canAdress;

    std::vector<std::string> _serialList;

    Gtk::ComboBoxText _displayedList;

    Gtk::Button _launchCanMonitor;

    Gtk::Button _refreshList;

    Gtk::Box _container;

    bool _canListeningOnTCPIP;

    sigc::signal<void, std::string> _startScreenSignalOnExit;

};


#endif //ROOT_START_SCREEN_H
