#ifndef ROOT_PANELROBOTSTATE_H
#define ROOT_PANELROBOTSTATE_H

#include "GtkInclude.h"
#include <Json.h>

template <class UnaryFunction>

class PanelRobotState : {
public:
	PanelRobotState();
	void refreshPanel(JSON json);
	JSON generateTestJSON();

private:
	void setElement(JSON json, UnaryFunction f);
};


#endif // ROOT_PANELROBOTSTATE_H
