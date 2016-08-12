//
// Created by paul on 05/08/16.
//

#ifndef ROOT_START_SCREEN_H
#define ROOT_START_SCREEN_H


#include <bits/unique_ptr.h>
#include <vector>
#include "Header.h"

class Start_Screen : public Gtk::Window {

    public:

    Start_Screen();

    virtual ~Start_Screen();

    /**
     * @brief Will update _serialList with all the file in /dev that are relevant (aka /dev/ttyUSB0, etc.)
     * @warning Does not throw any exception
     */
    void scanSerialConnection();

private:

    std::unique_ptr<std::string> _canAdress;

    std::vector<std::string> _serialList;

};


#endif //ROOT_START_SCREEN_H
