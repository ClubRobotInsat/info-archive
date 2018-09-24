/*
 * CarteBaliseInertielle2010.cpp
 *
 * Created on: 9 janv. 2010
 * Author: robot
 */

#include "CarteBaliseInertielle2010.h"

CarteBaliseInertielle2010::CarteBaliseInertielle2010(Commun::CAN& can, uint8_t id)
        : Carte(can, id)
        , _positionActualise(false)
        , _angleActualise(false)
        , _vitesseAngulaireActualise(false)
        , _vitesseLineaireActualise(false) {
	_positionX = 0_m;
	_positionY = 0_m;
	_angle = 0_rad;
	_vitesseAngulaire = 0_rad_s;
	_vitesseLineaire = 0_m_s;
}

void CarteBaliseInertielle2010::traiterMessage(Trame const& message) {
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
		case 0x02_b:
			if(message.getNbDonnees() == 2) {
				// _angle de l'adversaire al:ah
				bloquerVariables();
				_angleActualise = true;
				_angle = Angle::makeFromMilliRad(message.getDonnee<std::int16_t>(0));
				debloquerVariables();
			}
			break;
		case 0x03_b:
			if(message.getNbDonnees() == 2) {
				// _vitesse lineaire de l'adversaire vl:vh
				bloquerVariables();
				_vitesseAngulaireActualise = true;
				_vitesseLineaire = Speed::makeFromMm_s(message.getDonnee<std::int16_t>(0));
				debloquerVariables();
			}
			break;
		case 0x04_b:
			if(message.getNbDonnees() == 2) {
				// _vitesse angulaire de l'adversaire vl:vh
				bloquerVariables();
				_vitesseAngulaireActualise = true;
				_vitesseAngulaire = AngularSpeed::makeFromMilliRad_s(message.getDonnee<std::int16_t>(0));
				debloquerVariables();
			}
			break;
		case 0x05_b:
			if(message.getNbDonnees() == 7 || message.getNbDonnees() == 5) {
				if(message.getDonnee(0) == 0x01_b) {
					// _position et _angle de l'adversaire 01:xl:xh:yl:yh:al:ah
					bloquerVariables();
					_positionX = Distance::makeFromMm(message.getDonnee<std::int16_t>(1));
					_positionY = Distance::makeFromMm(message.getDonnee<std::int16_t>(3));
					_angle = Angle::makeFromMilliRad(message.getDonnee<std::int16_t>(5));
					debloquerVariables();
				} else if(message.getDonnee(0) == 0x02_b) {
					// _vitesse lineaire et _vitesse angulaire de l'adversaire 02:vl:vh:wl:wh
					bloquerVariables();
					_positionActualise = true;
					_vitesseLineaire = Speed::makeFromMm_s(message.getDonnee<std::int16_t>(1));
					_vitesseAngulaire = AngularSpeed::makeFromMilliRad_s(message.getDonnee<std::int16_t>(3));
					debloquerVariables();
				}
			}
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

bool CarteBaliseInertielle2010::estPositionActualise() {
	return _positionActualise;
}

bool CarteBaliseInertielle2010::estAngleActualise() {
	return _angleActualise;
}

bool CarteBaliseInertielle2010::estVitesseLineaireActualise() {
	return _vitesseLineaireActualise;
}

bool CarteBaliseInertielle2010::estVitesseAngulaireActualise() {
	return _vitesseAngulaireActualise;
}

void CarteBaliseInertielle2010::actualiserPosition() {
	_positionActualise = false;
	envoyerMessage(0x01_b, 0xFF_b);
}

void CarteBaliseInertielle2010::actualiserAngle() {
	_angleActualise = false;
	envoyerMessage(0x02_b, 0xFF_b);
}

void CarteBaliseInertielle2010::actualiserVitesseLineaire() {
	_vitesseLineaireActualise = false;
	envoyerMessage(0x03_b, 0xFF_b);
}

void CarteBaliseInertielle2010::actualiserVitesseAngulaire() {
	_vitesseAngulaireActualise = false;
	envoyerMessage(0x04_b, 0xFF_b);
}

Distance CarteBaliseInertielle2010::getPositionX() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _positionX;
}

Distance CarteBaliseInertielle2010::getPositionY() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _positionY;
}

Angle CarteBaliseInertielle2010::getAngle() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _angle;
}

Speed CarteBaliseInertielle2010::getVitesseLineaire() {
	return _vitesseLineaire;
}
AngularSpeed CarteBaliseInertielle2010::getVitesseAngulaire() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _vitesseAngulaire;
}

void CarteBaliseInertielle2010::setModeAutomatique(bool mode) {
	envoyerMessage(0x06_b, mode);
}

void CarteBaliseInertielle2010::calibration() {
	envoyerMessage(0x0f_b, '0');
}
