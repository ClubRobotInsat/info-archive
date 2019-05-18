#ifndef ROOT_PANELCENTRAL_H
#define ROOT_PANELCENTRAL_H

#include "GtkInclude.h"

#include "GtkSimuContext.h"

/** Panneau permettant de changer les paramètres du simulateur et de le
 * reinitialiser si besoin. */
class PanelCentral : public Gtk::Frame {
public:
	PanelCentral(GtkSimuContext& context);

private:
	GtkSimuContext& _context;

	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::Button* _resetButton;
	Gtk::Entry* _nameEntry;
	Gtk::ComboBoxText* _colorCombo;
	Gtk::CheckButton* _worldCheck;
	Gtk::CheckButton* _physicsCheck;


	void onReset();

	void onPhysicsChange();
};

#endif // ROOT_PANELCENTRAL_H
