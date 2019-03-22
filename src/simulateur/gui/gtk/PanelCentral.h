#ifndef ROOT_PANELCENTRAL_H
#define ROOT_PANELCENTRAL_H

#include "GtkInclude.h"

#include "GtkSimuContext.h"

class PanelCentral : public Gtk::Frame {
public:
	PanelCentral(GtkSimuContext& context);

private:
	GtkSimuContext& _context;

	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::Button* _resetButton;
	Gtk::ComboBoxText* _colorCombo;


	void onReset();
};

#endif // ROOT_PANELCENTRAL_H
