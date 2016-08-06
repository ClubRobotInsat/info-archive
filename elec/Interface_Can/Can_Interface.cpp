//
// Created by paul on 18/07/16.
//

//Lib nécessaires pour la compilation : libgtk-3-dev

#include "Application.h"
#include "Header.h"

int main(int , char** argv){

    std::string const& usb_port = argv[1];
#ifdef ACTIVATE_CAN
    Application app = Application(usb_port);
#else
    Application app = Application();
#endif
    gtk_main();
};
