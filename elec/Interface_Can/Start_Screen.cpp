//
// Created by paul on 05/08/16.
//

#include "Start_Screen.h"

Start_Screen::Start_Screen() {

    _window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_show(_window);

    g_signal_connect(_window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

}
