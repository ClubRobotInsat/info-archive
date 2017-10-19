/************************************************************
 * CarteContacteurs2007.h *
 * Spécifie la classe gérant les contacteurs 2006/2007 *
 ************************************************************/

#ifndef _CARTE_CONTACTEURS_2007_H_
#define _CARTE_CONTACTEURS_2007_H_

#include "Carte.h"

class CarteContacteurs2007 : public Carte {
public:
	CarteContacteurs2007(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation des états des contacteurs
	void actualiserContacteurs();

	// retourne vrai si l'etat des contacteurs a ete actualise
	bool sontContacteursActualises();

	// Retourne l'état d'un des contacteurs
	bool getContacteur(int contacteur);

private:
	std::atomic_bool _contacteursActualises;
	std::atomic_bool _contacteurs[6];
};

#endif
