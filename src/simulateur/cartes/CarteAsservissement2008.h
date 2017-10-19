// CarteAsservissement2008.h

#ifndef CARTEASSERVISSEMENT2008_H_
#define CARTEASSERVISSEMENT2008_H_

#include "../simulateur/Robot/Carte.h"
#include "IMoteurAsservi.h"

class CarteAsservissement2008 : public Carte {
public:
	static const AngularSpeed ASSERVIS_VITESSE_MAX;
	static const Angle ASSERVIS_EPSILON;

	enum TypeAsservis { ASSERVIS_POSITION, ASSERVIS_VITESSE };

public:
	// constructeur
	CarteAsservissement2008(uint8_t id, Robot& robot, IMoteurAsservi& iMoteurAsservi);

	// destructeur
	virtual ~CarteAsservissement2008();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoi reponce a un ping
	virtual void envoyerPong();

	// Envoi vitesse atteint
	virtual void envoyerVitesseAtteint();

	// Envoi position atteint
	virtual void envoyerPositionAtteint();

	// si le mode d'asservissement est position regule la position jusqu'a l'atteindre
	virtual void update(Duration elapsed) override;

private:
	// moteur a commander
	IMoteurAsservi& _iMoteurAsservi;

	// type d'asservissement courant
	TypeAsservis _typeAsservis;

	// valeur a atteindre
	AngularSpeed _vitesse;
	Angle _angle;

	// decalage de l'angle entre celui de l'ia et celui mesuré sur le robot
	Angle _decalageAngle;
};

#endif /*CARTEASSERVISSEMENT2008_H_*/
