//
// Created by paul on 18/07/16.
//

//Lib nécessaires pour la compilation : libgtkmm-3.0-dev

#include "Application.h"
#include "Header.h"

int main(int argc, char** argv){

    //std::string const& usb_port = argv[1];

#ifdef ACTIVATE_CAN

    Application app = Application(usb_port, argc, argv);

#else

    Application app(argc, argv, "paul.florence.clubrobot");

    //auto app = Gtk::Application::create(argc, argv, "test.paul.flo");

    //Start_Screen screen;

    //return app->run(screen);

#endif
};
