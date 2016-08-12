//
// Created by paul on 05/08/16.
//

#include "Start_Screen.h"
#include <iostream>
#include <dirent.h>
#include <cstring>

Start_Screen::Start_Screen() : Gtk::Window() , _serialList({}){

    this->set_border_width(0);
    this->set_decorated(false);
    this->set_size_request(800, 450);
    this->set_position(Gtk::WindowPosition::WIN_POS_CENTER_ALWAYS);
    this->set_resizable(false);

}

Start_Screen::~Start_Screen() {

    //gtk_widget_destroy(_window);
    //gtk_widget_destroy(_startbutton);

}


void Start_Screen::scanSerialConnection() {

    _serialList.clear();

    //temp file for storing everything that is in /dev;
    std::vector<std::string> files_in_dev= {};

    //open a directory and read it
    auto open_dir = opendir("/dev");
    auto dev_dir = readdir(open_dir);

    //populate files_in_dev with all the files in /dev that matches "tty"
    while (dev_dir != NULL) {
        std::string string_filename = dev_dir->d_name;
        if (string_filename .substr(0,3) == "tty") {
            files_in_dev.push_back(string_filename );
        }
        dev_dir = readdir(open_dir);
    }

    _serialList = files_in_dev;
    for (auto &string : _serialList) {
        std::cout << string << std::endl;
    }
}
