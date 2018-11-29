#ifndef ROOT_GTKSIMUAPPLICATION_H
#define ROOT_GTKSIMUAPPLICATION_H

#include <gtkmm.h>

class GtkSimuApplication : public Gtk::Application {
public:
    GtkSimuApplication(int argc, char** argv, std::string id);

    ~GtkSimuApplication();
};


#endif //ROOT_GTKSIMUAPPLICATION_H
