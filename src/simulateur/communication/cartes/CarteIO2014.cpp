/*
 * CarteIO2014.cpp
 *
 *  Created on: 20 mars 2014
 *  Last update : 16 octobre 2014
 *      Author: taprest
 */

#include "CarteIO2014.h"
#include "../../core/Simulateur.h"
#include <cstdint>
#include <cstring>

CarteIO2014::CarteIO2014(unsigned char id, RobotLogic& robot, IContacteursV2& iContacteurs)
        : Carte(id, robot), _iContacteurs(iContacteurs) {}

// Traite les données fournies par l'IA
void CarteIO2014::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// ping => répondre un pong
				Trame aEnvoyer(this->getId(), 0);
				aEnvoyer.addByte(0xAA_b);
				this->envoyer(aEnvoyer);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			throw Trame::ErreurTrameNonTraitee(t);

		case 0x02_b:
			this->envoyerValeurCapteur(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteIO2014::envoyerValeurCapteur(Trame const& t) {
	//	Renvoie la couleur du côté du robot
	if(t.getDonnee(0) == 1) {
		Trame aEnvoyer(this->getId(), 2);
		aEnvoyer.addByte(t.getDonnee(0));
		aEnvoyer.addByte(Simulateur::getInstance().getRobotColor() == Constantes::RobotColor::Blue ? 0x01_b : 0x00_b);
		this->envoyer(aEnvoyer);
	}
	//	Renvoie la tirette
	if(t.getDonnee(0) == 2) {
		Trame aEnvoyer(this->getId(), 2);
		aEnvoyer.addByte(t.getDonnee(0));
		aEnvoyer.addByte(Simulateur::getInstance().getRobotColor() == Constantes::RobotColor::Blue ? 0x01_b : 0x00_b);
		this->envoyer(aEnvoyer);
	} // Envoie de l'état du capteur demandé
	else if(t.getNbDonnees() == 1 && t.getDonnee(0) <= NBCAPTEURS &&
	        t.getDonnee(0) > 0) { // les capteurs vont de 1 à 12
		Trame aEnvoyer(this->getId(), 2);
		aEnvoyer.addByte(t.getDonnee(0));
		aEnvoyer.addByte(_iContacteurs.getContactorState(t.getDonnee(0), this->getId()));
		this->envoyer(aEnvoyer);
	} else if(t.getNbDonnees() == 1) {
		Trame aEnvoyer(this->getId(), 2);
		aEnvoyer.addByte(0xFF_b);
		this->envoyer(aEnvoyer);
	} else
		throw Trame::ErreurTrameNonTraitee(t);
}
