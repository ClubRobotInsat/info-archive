// CarteBaliseInertielle2010.cpp

#include "CarteBaliseInertielle2010.h"

CarteBaliseInertielle2010::CarteBaliseInertielle2010(unsigned char id, Robot& robot, IBaliseInertielle* robotAdverse)
        : Carte(id, robot), _robotAdverse(robotAdverse), _envoiAutomatique(false) {}

CarteBaliseInertielle2010::~CarteBaliseInertielle2010() {}

void CarteBaliseInertielle2010::setAdversaire(IBaliseInertielle* robot) {
	_robotAdverse = robot;
}

// Traite les donnees fournies par l'IA
void CarteBaliseInertielle2010::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			// Si ping
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// repondre un pong
				sendPong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			// Si demande position
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				sendPositionAdversaire();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x02_b:
			// Si demande angle
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				sendAngleAdversaire();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x03_b:
			// Si demande vitesse
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				sendVitesseAdversaire();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x04_b:
			// Si demande vitesse angulaire
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				sendVitesseAngulaireAdversaire();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x06_b:
			// Active ou desactive l'envoi automatique
			if(t.getNbDonnees() == 1) {
				// Mise a jour du booleen d'envoi automatique
				_envoiAutomatique = t.getDonnee(0);
			}

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteBaliseInertielle2010::sendPong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	logInfo("CarteBaliseInertielle2010::pong ping -> pong");
}

void CarteBaliseInertielle2010::sendPositionAdversaire() {
	Trame aEnvoyer(this->getId(), 0x01_b);
	aEnvoyer.addDonnees(_robotAdverse->getPositionAdv().x.toMm<short>(),   // position selon x
	                    -_robotAdverse->getPositionAdv().z.toMm<short>()); // position selon y
	this->envoyer(aEnvoyer);
} // format trame : 0x01:xl:xh:yl:yh

void CarteBaliseInertielle2010::sendAngleAdversaire() {
	Trame aEnvoyer(this->getId(), 0x02_b);
	aEnvoyer.addDonnees((_robotAdverse->getAngle2dAdv() - 0.5_PI).toMilliRad<short>()); // angle
	this->envoyer(aEnvoyer);
} // format trame : 0x02:al:ah

void CarteBaliseInertielle2010::sendVitesseAdversaire() {
	Trame aEnvoyer(this->getId(), 0x03_b);
	aEnvoyer.addDonnees(_robotAdverse->getLinearVelocityAdv().toMm_s<short>()); // vitesse lineaire
	this->envoyer(aEnvoyer);
} // format trame : 0x03:vl:vh

void CarteBaliseInertielle2010::sendVitesseAngulaireAdversaire() {
	Trame aEnvoyer(this->getId(), 0x04_b);
	aEnvoyer.addDonnees(_robotAdverse->getAngularVelocityAdv().toMilliRad_s<short>()); // vitesse angulaire
	this->envoyer(aEnvoyer);
} // format trame : 0x04:wl:wh

void CarteBaliseInertielle2010::update(Duration elapsed) {
	Carte::update(elapsed);
	static Duration tempsDernierEnvoie = 0.0_s;
	static Duration tempsActuel = 0.0_s;
	tempsActuel += elapsed;
	if(tempsActuel - tempsDernierEnvoie > 0.2_s && _envoiAutomatique) {
		tempsDernierEnvoie = tempsActuel;

		// trame contenant la position et l'angle du robot adverse
		Trame tramePosition(this->getId(), 0x05_b);
		tramePosition.addByte(0x01_b);
		tramePosition.addDonnees((_robotAdverse->getPositionAdv().x.toMm<short>()),              // position selon x
		                         (-_robotAdverse->getPositionAdv().z.toMm<short>()),             // position selon y
		                         (_robotAdverse->getAngle2dAdv() - 0.5_PI).toMilliRad<short>()); // angle

		// trame contenant les vitesses lineaire et angulaire
		Trame trameVitesse(this->getId(), 0x05_b);
		trameVitesse.addByte(0x02_b);
		trameVitesse.addDonnees(_robotAdverse->getLinearVelocityAdv().toMm_s<short>(),         // vitesse lineaire
		                        _robotAdverse->getAngularVelocityAdv().toMilliRad_s<short>()); // vitesse angulaire

		// envoi des deux trames
		this->envoyer(tramePosition); // format trame : 0x05:0x01:xl:xh:yl:yh:al:ah
		this->envoyer(trameVitesse);  // format trame : 0x06:0x02:vl:vh:wl:wh
	}
}
