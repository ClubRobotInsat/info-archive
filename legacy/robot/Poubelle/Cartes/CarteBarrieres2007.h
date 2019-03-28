/*****************************************************************
 * CarteBarrieres2007.h *
 * Spécifie la classe gérant les barrières infrarouge 2006/2007 *
 *****************************************************************/

#ifndef _CARTE_BARRIERES_2007_H_
#define _CARTE_BARRIERES_2007_H_

#include "Carte.h"

class CarteBarrieres2007 : public Carte {
public:
	CarteBarrieres2007(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation des états des barrières
	void actualiserBarrieres();

	// retourne vrai si l'etat des barrières a ete actualise
	bool sontBarrieresActualisees();

	// Retourne l'état d'une des barrières
	bool getBarriere(int barriere);

private:
	std::atomic_bool _barrieresActualisees;
	std::atomic_bool _barrieres[2];
};

#endif
