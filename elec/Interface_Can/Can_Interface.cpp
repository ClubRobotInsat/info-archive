//
// Created by paul on 18/07/16.
//

//Lib nécessaires pour la compilation : libgtk-3-dev


#include "Application.h"
#include <gtk-3.0/gtk/gtk.h>

int main(int , char** argv){

    std::string const& usb_port = argv[1];
    Application app = Application(usb_port);

};
