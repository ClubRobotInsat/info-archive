/*****************************************************************
 * CarteAsservissement2008.cpp *
 * Implémente la classe gérant les moteurs asservis (pour le *
 * laser, le barillet...) *
 *****************************************************************/

#include "CarteAsservissement2008.h"
#include <cmath>

CarteAsservissement2008::CarteAsservissement2008(Commun::CAN& can, uint8_t id)
        : Carte(can, id), _vitesseAtteinte(false), _positionAtteinte(false) {}

void CarteAsservissement2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x04_b:
			if(message.getNbDonnees() == 1) {
				_vitesseAtteinte = true;
			}
			break;
		case 0x05_b:
			if(message.getNbDonnees() == 1) {
				_positionAtteinte = true;
			}
			break;

		case 0x0C_b:
			_bloque = true;
			printf("Barillet ou laser Bloque !\n");
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Retourne si le barillet est bouqle ou pas
bool CarteAsservissement2008::verifierBlocage() {
	return _bloque.exchange(false);
}

// Choix d'une vitesse angulaire (en mrad/s)
void CarteAsservissement2008::setVitesseAngulaire(std::int16_t vitesse_angulaire, SensRotation sens) {
	Trame t = this->make_trame(0x01_b);
	t.addDonnees(vitesse_angulaire);
	if(getId() == 2) {
		t.addDonnees(sens);
	}

	_vitesseAtteinte = false;
	_bloque = false;

	envoyerMessage(t);
}

// Indique si la vitesse angulaire demandée a été atteinte
bool CarteAsservissement2008::estVitesseAtteinte() {
	return _vitesseAtteinte;
}

// Choix d'une position angulaire (en mrad entre -PI et PI)
void CarteAsservissement2008::setPosition(std::int16_t angle) {
	angle = M_PI * 2000 - angle;

	_positionAtteinte = false;
	_bloque = false;

	envoyerMessage(this->make_trame(0x02_b, angle));
}

// Demander de s'arreter dans la position angulaire actuelle
void CarteAsservissement2008::stop() {
	_positionAtteinte = false;
	_bloque = false;

	envoyerMessage(0x03_b, 0xFF_b);
}

// Indique si la position demandée a été atteinte
bool CarteAsservissement2008::estPositionAtteinte() {
	return _positionAtteinte;
}

// redefinir la valeur de l'angle pour la position courante (en mrad)
void CarteAsservissement2008::initialiserPosition(std::int16_t angle) {
	_positionAtteinte = false;

	envoyerMessage(this->make_trame(0x06_b, angle));
}

void CarteAsservissement2008::reglerPID(TypePID type, std::int16_t kp, std::int16_t ki, std::int16_t kd) {
	envoyerMessage(this->make_trame(0x08_b, type, kp, ki, kd));
}

void CarteAsservissement2008::asservissementOnOff(bool active) {
	envoyerMessage(0x0A_b, active);
}
