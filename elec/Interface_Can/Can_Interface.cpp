//
// Created by paul on 18/07/16.
//

//Lib nécessaires pour la compilation : libgtk-3-dev

#include "Application.h"
#include "Header.h"

int main(int argc, char** argv){

    std::string const& usb_port = argv[1];
#ifdef ACTIVATE_CAN
    Application app = Application(usb_port, argc, argv);
#else
    Application app = {argc, argv};//Application(argc, argv);
#endif
};
