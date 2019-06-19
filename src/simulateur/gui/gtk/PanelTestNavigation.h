#ifndef ROOT_PANELTESTNAVIGATION_H
#define ROOT_PANELTESTNAVIGATION_H

#include "GtkInclude.h"

#include "GtkSimuContext.h"

class PanelTestNavigation : public Gtk::Frame {
public:
	PanelTestNavigation(GtkSimuContext& context);

	/** Applique les parametres actuels sur le controller passé
	 * en paramètres.
	 * Cette méthode est thread safe. */
	void onValidate() const;

	Gtk::Button& getValidateButton();

private:
	GtkSimuContext& _context;

	Glib::RefPtr<Gtk::Builder> _builder;

	Gtk::RadioButton* _forwardCheck;
	Gtk::Entry* _distanceText;
	Gtk::RadioButton* _turnCheck;
	Gtk::Entry* _angleText;
	Gtk::Button* _validateButton;
};

#endif // ROOT_PANELTESTNAVIGATION_H
