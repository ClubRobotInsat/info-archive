#include "PanelCentral.h"

#include <resources/EmbeddedFiles.h>

PanelCentral::PanelCentral(GtkSimuContext& context) : _context(context) {

	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/central-panel.glade"));

	_builder->get_widget("reset", _resetButton);
	_builder->get_widget("color", _colorCombo);

	Gtk::Widget* root;
	_builder->get_widget("root", root);
	add(*root);

	// widget initialization
	for(auto& color : _context.getGuiClient().getRobotColors()) {
		_colorCombo->append(color);
	}

	// signals
	_resetButton->signal_clicked().connect(sigc::mem_fun(this, &PanelCentral::onReset));
}

void PanelCentral::onReset() {
	std::string colorTxt = _colorCombo->get_active_text();

	_context.queueAction([this, colorTxt] { _context.getGuiClient().reset(ResetData{colorTxt}); });
}