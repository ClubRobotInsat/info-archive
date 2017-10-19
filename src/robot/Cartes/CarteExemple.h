#ifndef _CARTE_EXEMPLE_H_
#define _CARTE_EXEMPLE_H_

#include "Carte.h"

class CarteExemple : public Carte {
public:
	CarteExemple(Commun::CAN& can, uint8_t id);

	// Ici des methodes pour faire des choses avec la carte

	/// Traite les messages de la carte
	virtual void traiterMessage(Trame const& message) override;

private:
	// Ici des variables membres
};

#endif
