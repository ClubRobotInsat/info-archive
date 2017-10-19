// CarteDebugDessin.h

#ifndef CARTEDEBUGDESSIN_H_
#define CARTEDEBUGDESSIN_H_

#include "../simulateur/robot/Carte.h"
#include "IDebugDessin.h"

class CarteDebugDessin : public Carte {
public:
	enum TypeDessin { LIGNES, DISQUES, TRIANGLES, QUADS };

public:
	CarteDebugDessin(unsigned char id, RobotLogic& robot, IDebugDessin& iDebugDessin);

	virtual ~CarteDebugDessin();

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// mise a jour de la carte
	virtual void update(Duration elapsed) override;

	// envoyer activation/desactivation de l'affichage des dessin
	virtual void envoyerActivationAffichage(bool activation);

private:
	IDebugDessin& _iDebugDessin;

	// taille de la grille (en nbr case)
	uint16_t _grilleTailleX, _grilleTailleY;

	// indique si l'affiche des dessin de debug sont active
	bool _activationDessin;
};

#endif /*CARTEDEBUGDESSIN_H_*/
