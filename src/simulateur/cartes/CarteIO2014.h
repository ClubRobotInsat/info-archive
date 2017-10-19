/*
 * CarteIO2014.h
 *
 *  Created on: 20 mars 2014
 *  Last update : 16 octobre 2014
 *      Author: taprest
 */

#ifndef CARTEIO2014_H_
#define CARTEIO2014_H_

#include "../simulateur/robot/Carte.h"
#include "IContacteursV2.h"

#define NBCAPTEURS 6

class CarteIO2014 : public Carte {

public:
	CarteIO2014(unsigned char id, RobotLogic& robot, IContacteursV2& iContacteurs);

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoie l'etat des contacteurs...
	virtual void envoyerValeurCapteur(Trame const&);

	// erreur le numero du contacteur est trop grand
	class ErreurNumContact : public std::runtime_error {
	public:
		ErreurNumContact(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " + to_string((short)NBCAPTEURS)) {}
	};

	// erreur le Nombre de contacteur est trop grand
	class ErreurNbrContact : public std::runtime_error {
	public:
		ErreurNbrContact()
		        : std::runtime_error("le nombre de contacteurs (" + to_string((short)NBCAPTEURS) +
		                             ") est trop grand pour etre stocker sur un char ") {}
	};


private:
	IContacteursV2& _iContacteurs;
};

#endif /* CARTEIO2014_H_ */
