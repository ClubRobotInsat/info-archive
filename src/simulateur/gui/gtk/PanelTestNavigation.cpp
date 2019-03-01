#include "PanelTestNavigation.h"

#include "resources/EmbeddedFiles.h"

PanelTestNavigation::PanelTestNavigation(GtkSimuContext& context) : _context(context) {
	_builder = Gtk::Builder::create_from_string(EmbeddedFiles::readText("glade/test-navigation-panel.glade"));

	_builder->get_widget("forward", _forwardCheck);
	_builder->get_widget("distance", _distanceText);
	_builder->get_widget("turn", _turnCheck);
	_builder->get_widget("angle", _angleText);
	_builder->get_widget("validate", _validateButton);

	Widget* root;
	_builder->get_widget("root", root);
	add(*root);

	// Signals and IO
	_validateButton->signal_clicked().connect(sigc::mem_fun(*this, &PanelTestNavigation::onValidate));
}

void PanelTestNavigation::onValidate() const {
	bool forward = _forwardCheck->get_active();
	std::string distanceStr = _distanceText->get_text();
	bool turn = _turnCheck->get_active();
	std::string angleStr = _angleText->get_text();

	_context.queueAction([this, forward, distanceStr, turn, angleStr]() {

		if(forward) {
			try {
				double distance = std::stod(distanceStr);
				_context.getGuiClient().testNavigationForward(Distance::makeFromM(distance));
			} catch(std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		} else if(turn) {
			try {
				double angle = std::stod(angleStr);
				_context.getGuiClient().testNavigationTurn(Angle::makeFromDeg(angle));
			} catch(std::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		}
	});
}

Gtk::Button& PanelTestNavigation::getValidateButton() {
	return *_validateButton;
}