// CarteContacteurs2011.h

#ifndef CarteContacteurs2011_H_
#define CarteContacteurs2011_H_

#include "../simulateur/robot/Carte.h"
#include "IContacteursV2.h"

#define NOMBRE_CONTACTEURS 6 // doit etre inferieur ou egal a 8 car tout est fait en unsigned char.

class CarteContacteurs2011 : public Carte {
public:
	CarteContacteurs2011(unsigned char id, RobotLogic& robot, IContacteursV2& iContacteurs);

	virtual ~CarteContacteurs2011();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoie l'etat des contacteurs...
	virtual void envoyerEtatContacteurs();

	// verifie si les contacteurs on ete modifies et envoie une trame si besoin
	virtual void update(Duration elapsed) override;

	// erreur le numero du contacteur est trop grand
	class ErreurNumContact : public std::runtime_error {
	public:
		ErreurNumContact(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " +
		                             to_string((short)NOMBRE_CONTACTEURS)) {}
	};

	// erreur le Nombre de contacteur est trop grand
	class ErreurNbrContact : public std::runtime_error {
	public:
		ErreurNbrContact()
		        : std::runtime_error("le nombre de contacteurs (" + to_string((short)NOMBRE_CONTACTEURS) +
		                             ") est trop grand pour etre stocker sur un char ") {}
	};

private:
	bool _contacteurs[NOMBRE_CONTACTEURS];
	IContacteursV2& _iContacteurs;
};

#endif /*CarteContacteurs2011_H_*/
