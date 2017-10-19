/************************************************************
 * CarteALIMENTATION2010.h *
 * Spécifie la classe gérant les ALIMENTATIONS 2009/2010 *
 ************************************************************/

#ifndef _CARTE_ALIMENTATION_2010_H_
#define _CARTE_ALIMENTATION_2010_H_

#include "Carte.h"

class CarteAlimentation2010 : public Carte {
public:
	CarteAlimentation2010(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Envoie l'ordre d'actualiser la tension a la carte
	void actualiserTension();

	// Verifie si la tension a ete actualisee
	bool verifierTensionActualisee();

	// Retourne la tension
	int getTension();


private:
	std::atomic_int _tension;
	std::atomic_bool _tensionActualisee;
};

#endif
