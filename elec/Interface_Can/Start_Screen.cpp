//
// Created by paul on 05/08/16.
//

#include "Start_Screen.h"
#include <iostream>
#include <dirent.h>
#include <cstring>

Start_Screen::Start_Screen() : Gtk::Window() , _serialList({}){
    /*
    _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    //Appearance of the window
    gtk_widget_set_size_request(_window, 800, 450);
    gtk_window_set_decorated(GTK_WINDOW (_window), FALSE);
    gtk_window_set_position(GTK_WINDOW (_window),GTK_WIN_POS_CENTER_ALWAYS);
    gtk_window_set_resizable(GTK_WINDOW (_window), FALSE);

    //Uncomment to add a backgroud picture
    //auto image = gtk_image_new_from_file("../Ressources/background.jpg");
    //gtk_container_add(GTK_CONTAINER(_window), image);

    //Start Button
    _startbutton = gtk_button_new_with_label("Start CAN");
    gtk_widget_set_size_request(_startbutton, 50, 100);

    gtk_container_add(GTK_CONTAINER(_window), _startbutton);

    gtk_widget_show_all(_window);

    //this->scanSerialConnection();

    g_signal_connect(_startbutton, "clicked", G_CALLBACK(&Start_Screen::scanSerialConnection), this);

    g_signal_connect(_window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);
                     */

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
