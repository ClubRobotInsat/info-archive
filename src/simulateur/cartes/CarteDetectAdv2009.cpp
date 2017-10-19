// CarteDetectAdv2009.cpp

#include "CarteDetectAdv2009.h"
#include <cmath>

const bool CarteDetectAdv2009::DEFAULT_ENVOI_AUTO = false;
const Duration CarteDetectAdv2009::PERIODE_ENVOI_AUTO = 0.07_s; //
const Distance CarteDetectAdv2009::DISTANCE_MAX = 55_cm;        // Distance à partir de laquelle on ne voit rien, en dm

CarteDetectAdv2009::CarteDetectAdv2009(unsigned char id, RobotLogic& robot, IDeplacement& iDeplacement, ICoordonneesAdversaire& iCoordsAdv)
        : Carte(id, robot)
        , _iDeplacement(iDeplacement)
        , _iCoordsAdv(iCoordsAdv)
        , _envoiAuto(DEFAULT_ENVOI_AUTO)
        , _angleDefini(false)
        , _tourneInfini(false) {
	_lastTime = 0.0_s;
}

CarteDetectAdv2009::~CarteDetectAdv2009() {}

// Envoyer une trame de pong
void CarteDetectAdv2009::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
}

// Envoi de la distance et de l'angle de l'adversaire
void CarteDetectAdv2009::envoyerDistAngleAdv() {
	// Récupération de la distance et de l'angle par rapport à l'adversaire :
	Vector2m robotToAdv = _iCoordsAdv.getPositionAdversaire() - _iDeplacement.getPosition();

	Distance distance = robotToAdv.norm();
	Angle angle = -(atan2(robotToAdv.y, robotToAdv.x) - 0.5_PI + _iDeplacement.getAngle()).toMod2Pi();

	// Si l'adversaire est trop loin, on envoie des 0
	if(distance >= DISTANCE_MAX) {
		distance = 0.0_m;
		angle = 0.0_rad;
	}

	// Construction et envoi de la trame :
	Trame t(this->getId(), 0x0E_b);
	// logDebug("envoie distance", (float)distance);
	t.addDonnees(distance.toM<float>(), angle.toRad<float>());
	this->envoyer(t);
}

// Traite les données fournies par l'IA
void CarteDetectAdv2009::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// Ping/pong
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				envoyerPong();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// L'IA définit la valeur de l'angle du moteur : on assume que c'est la bonne
		// et on enregistre le fait que cette valeur soit définie.
		case 0x03_b:
			if(t.getNbDonnees() == 4)
				_angleDefini = true;
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// L'IA fait un TensionMoteur() :
		case 0x04_b:
			if(t.getNbDonnees() == 2)
				_tourneInfini = true;
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Partie spécifique à la carte CarteDetectAdv2009 :
		case 0x0E_b:
			if(t.getNbDonnees() == 1) {
				unsigned char donnee = t.getDonnee(0);

				// Activation/désactivation envoi auto
				if(donnee == 0_b)
					_envoiAuto = false;
				else if(donnee == 1_b)
					_envoiAuto = true;

				// Envoi de la distance et de l'angle
				else if(donnee == 2_b)
					envoyerDistAngleAdv();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Envoi de la distance et de l'angle de l'adversaire toutes les 200 ms, sous
// certaines conditions
void CarteDetectAdv2009::update(Duration elapsed) {
	Carte::update(elapsed);

	if(_clock.getElapsedTime() - _lastTime >= PERIODE_ENVOI_AUTO) {
		_lastTime = _clock.getElapsedTime();

		if(_envoiAuto) {
			envoyerDistAngleAdv();
		}
	}
}
