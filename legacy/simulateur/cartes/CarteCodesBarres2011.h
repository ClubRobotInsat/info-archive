// CarteCodesBarres2011.h

#ifndef CARTECODESBARRES2011_H_
#define CARTECODESBARRES2011_H_

#include "../simulateur/Robot/Carte.h"
#include "ICodesBarres.h"

#define NOMBRE_CODE_BARRE 2 // doit etre inferieur ou egal a 8 car tout est fait en unsigned char.

class CarteCodesBarres2011 : public Carte {
public:
	CarteCodesBarres2011(unsigned char id, Robot& robot, ICodesBarres& iCodesBarres);

	virtual ~CarteCodesBarres2011();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoie l'etat des codes barres...
	virtual void envoyerEtatCodeBarre(unsigned char numCodeBarre);

	// verifie si les contacteurs on ete modifies et envoie une trame si besoin
	virtual void update(Duration elapsed) override;

	// erreur le numero du code barre est trop grand
	class ErreurNumContact : public std::runtime_error {
	public:
		ErreurNumContact(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " +
		                             to_string((short)NOMBRE_CODE_BARRE)) {}
	};

	// erreur le Nombre de code barre est trop grand
	class ErreurNbrContact : public std::runtime_error {
	public:
		ErreurNbrContact()
		        : std::runtime_error("le nombre de contacteurs (" + to_string((short)NOMBRE_CODE_BARRE) +
		                             ") est trop grand pour etre stocker sur un char ") {}
	};

private:
	ICodesBarres::MessageLu _codesBarres[NOMBRE_CODE_BARRE];
	ICodesBarres& _iCodesBarres;
};

#endif /*CARTECODESBARRES2011_H_*/
