#ifndef ROOT_PANELROBOTSTATE_H
#define ROOT_PANELROBOTSTATE_H

#include <Json.h>
#include "GtkInclude.h"

class PanelRobotState : public Gtk::Frame {
public:
	PanelRobotState();

    void refreshPanel(JSON json);
    JSON generateTestJSON();
private:
    Glib::RefPtr<Gtk::Builder> _builder;
};


#endif // ROOT_PANELROBOTSTATE_H
