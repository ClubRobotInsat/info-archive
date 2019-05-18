#include "PanelCentral.h"

#include <resources/EmbeddedFiles.h>

PanelCentral::PanelCentral(GtkSimuContext& context) : _context(context) {

	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/central-panel.glade"));

	_builder->get_widget("reset", _resetButton);
	_builder->get_widget("name", _nameEntry);
	_builder->get_widget("color", _colorCombo);
	_builder->get_widget("enableWorld", _worldCheck);
	_builder->get_widget("enablePhysics", _physicsCheck);

	Gtk::Widget* root;
	_builder->get_widget("root", root);
	add(*root);

	// widget initialization
	// TODO l'accès aux reset data n'est pas thread-safe
	ResetData startData = _context.getGuiClient().getResetData();

	_nameEntry->set_text(startData.name);

	for(auto& color : _context.getGuiClient().getRobotColors()) {
		_colorCombo->append(color);
	}
	_colorCombo->set_active(0);

	_worldCheck->set_active(startData.enableWorld);
	_physicsCheck->set_active(startData.enablePhysics);

	// signals
	_resetButton->signal_clicked().connect(sigc::mem_fun(this, &PanelCentral::onReset));
	_physicsCheck->signal_clicked().connect(sigc::mem_fun(this, &PanelCentral::onPhysicsChange));
}

void PanelCentral::onReset() {
	std::string nameTxt = _nameEntry->get_text();
	std::string colorTxt = _colorCombo->get_active_text();
	bool enableWorld = _worldCheck->get_active();
	bool enablePhysics = _physicsCheck->get_active();

	_context.queueAction([this, nameTxt, colorTxt, enableWorld, enablePhysics] {
		_context.getGuiClient().resetWorld(ResetData{nameTxt, colorTxt, enableWorld, enablePhysics});
	});
}

void PanelCentral::onPhysicsChange() {
	bool enablePhysics = _physicsCheck->get_active();

	_context.queueAction([this, enablePhysics] { _context.getGuiClient().setPhysicsEnabled(enablePhysics); });
}