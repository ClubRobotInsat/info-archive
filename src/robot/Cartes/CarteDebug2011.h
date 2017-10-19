/*******************************************************************
 * CarteDebug2011.h *
 * Spécifie la classe gérant les actions de *
 * très haut niveau sur le robot 2011 (ramasser, déposer, etc...) *
 *******************************************************************/

#ifndef _CARTE_DEBUG_2011_H_
#define _CARTE_DEBUG_2011_H_

#include "Carte.h"

class CarteDebug2011 : public Carte {
public:
	CarteDebug2011(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Lecture de l'état des contacteurs des bras
	void actualiserContacteursBras();
	bool sontContacteursBrasActualises();

	// Lecture de l'état du contacteur des pinces
	void actualiserContacteurPinces();
	bool estContacteurPincesActualise();
	bool aPionEntresPinces();

	// Ramassage d'un pion avec les pinces
	void ramasserPionPinces();

	// Dépose d'un pion avec les pinces
	void deposerPionPinces();

private:
	std::atomic_bool _aPionSousBras[2];
	std::atomic_bool _contacteursBrasActualises;

	std::atomic_bool _aPionEntrePinces;
	std::atomic_bool _contacteurPincesActualise;
};

#endif
