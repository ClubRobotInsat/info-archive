/************************************************************
 * CartePneumatique2014.h *
 * Spécifie la classe gérant la carte PNEUMATIQUE *
 ************************************************************/

#ifndef _CARTE_PNEUMATIQUE2014_H_
#define _CARTE_PNEUMATIQUE2014_H_

#include "Carte.h"

class CartePneumatique2014 : public Carte {
public:
	CartePneumatique2014(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Active la pompe
	void activerPompe();

	// Stoppe la pompe
	void desactiverPompe();

	// Demande l'actualisation de l'état de la pompe
	void actualiserEtatPompe();

	// Retourne vrai si la pompe est activée
	bool getPompeActivee();

private:
	std::atomic_bool _pompeActivee;
};

#endif
