/************************************************************
 * CarteIHM.h *
 * Spécifie la classe gérant la carte IHM *
 ************************************************************/

#ifndef _CARTE_IHM_H_
#define _CARTE_IHM_H_

#include "Carte.h"

class CarteIHM : public Carte {
public:
	CarteIHM(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation des états des contacteurs
	void actualiserContacteurs();

	// retourne vrai si l'etat des contacteurs a ete actualise
	bool sontContacteursActualises();

	// Retourne l'état d'un des contacteurs
	bool getContacteur(int contacteur);

private:
	std::atomic_bool _contacteursActualises;
	std::atomic_bool _contacteurs[2];
};

#endif
