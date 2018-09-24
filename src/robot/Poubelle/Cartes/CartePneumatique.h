/************************************************************
 * CartePneumatique.h *
 * Spécifie la classe gérant la carte PNEUMATIQUE *
 ************************************************************/

#ifndef _CARTE_PNEUMATIQUE_H_
#define _CARTE_PNEUMATIQUE_H_

#include "Carte.h"

class CartePneumatique : public Carte {
public:
	CartePneumatique(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	bool estTimerDemarre();

	void demarrerTimer();

private:
	std::atomic_bool _timerDemarre;
};

#endif
