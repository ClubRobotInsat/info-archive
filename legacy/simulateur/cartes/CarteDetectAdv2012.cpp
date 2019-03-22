#include "CarteDetectAdv2012.h"

CarteDetectAdv2012::CarteDetectAdv2012(unsigned char id, Robot& robot, ICoordonneesAdversaire* robotAdverse)
        : Carte(id, robot), _robotAdverse(robotAdverse), _envoiAutomatique(false) {}

CarteDetectAdv2012::~CarteDetectAdv2012() {}

void CarteDetectAdv2012::setAdversaire(ICoordonneesAdversaire* robot) {
	_robotAdverse = robot;
}

// Traite les donnees fournies par l'IA
void CarteDetectAdv2012::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			// Si ping
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// repondre un pong
				sendPong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x06_b:
			// Si demande position
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				sendPositionAdversaire();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x07_b:
			// Active ou desactive l'envoi automatique
			if(t.getNbDonnees() == 4) {
				// Mise a jour du booleen d'envoi automatique
				_envoiAutomatique = t.getDonnee(0);
			}
			break;

		case 0x03_b:
			// Active ou desactive l'envoi automatique
			sendEstPret();
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteDetectAdv2012::sendPong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	logInfo("CarteDetectAdv2012::pong ping -> pong");
}

void CarteDetectAdv2012::sendPositionAdversaire() {
	Trame aEnvoyer(this->getId(), 0x05_b);
	aEnvoyer.addDonnees(_robotAdverse->getPositionAdversaire().x.toMm<float>(),   // position selon x
	                    -_robotAdverse->getPositionAdversaire().z.toMm<float>()); // position selon y
	this->envoyer(aEnvoyer);
} // format trame : 0x01:xl:xh:yl:yh


void CarteDetectAdv2012::sendEstPret() {
	Trame aEnvoyer(this->getId(), 0x03_b);
	aEnvoyer.addByte(1);
	this->envoyer(aEnvoyer);
}

void CarteDetectAdv2012::update(Duration elapsed) {
	Carte::update(elapsed);
	static auto tempsDernierEnvoie = 0.0_s;
	static auto tempsActuel = 0.0_s;
	tempsActuel += elapsed;
	if(tempsActuel - tempsDernierEnvoie > 0.2_s && _envoiAutomatique) {
		tempsDernierEnvoie = tempsActuel;

		sendPositionAdversaire();
	}
}
