#include "CarteDetectAdv2012.h"

CarteDetectAdv2012::CarteDetectAdv2012(Commun::CAN& can, uint8_t id) : Carte(can, id), _positionActualise(false) {}

void CarteDetectAdv2012::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x01_b:
			if(message.getNbDonnees() == 4) {
				// _position de l'adversaire xl:xh:yl:yh
				bloquerVariables();
				_positionActualise = true;
				_positionX = Distance::makeFromMm(message.getDonnee<std::int16_t>(0));
				_positionY = Distance::makeFromMm(message.getDonnee<std::int16_t>(2));
				debloquerVariables();
			}
			break;

		case 0x05_b:
			if(message.getNbDonnees() == 7 || message.getNbDonnees() == 5) {
				if(message.getDonnee(0) == 0x01_b) {
					// _position et angle de l'adversaire 01:xl:xh:yl:yh:al:ah
					bloquerVariables();
					_positionX = Distance::makeFromMm(message.getDonnee<std::int16_t>(1));
					_positionY = Distance::makeFromMm(message.getDonnee<std::int16_t>(3));
					debloquerVariables();
				}
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

bool CarteDetectAdv2012::estPositionActualise() {
	return _positionActualise;
}

void CarteDetectAdv2012::actualiserPosition() {
	_positionActualise = false;
	envoyerMessage(0x01_b, 0xFF_b);
}

Distance CarteDetectAdv2012::getPositionX() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _positionX;
}

Distance CarteDetectAdv2012::getPositionY() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _positionY;
}

void CarteDetectAdv2012::setModeAutomatique(bool mode) {
	envoyerMessage(0x06_b, mode);
}
