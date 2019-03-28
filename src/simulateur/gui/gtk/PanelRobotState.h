#ifndef ROOT_PANELROBOTSTATE_H
#define ROOT_PANELROBOTSTATE_H

#include "GtkInclude.h"
#include <Json.h>

// template <class UnaryFunction>
class PanelRobotState : public Gtk::Frame {
public:
	PanelRobotState();
	void refreshPanel(JSON json);
	JSON generateTestJSON();

private:
	template <class UnaryFunction>
	void setElement(JSON json, UnaryFunction f);
};


#endif // ROOT_PANELROBOTSTATE_H
