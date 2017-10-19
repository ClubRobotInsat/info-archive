/*************************************************************
 * CarteServos2007.h *
 * Spécifie la classe gérant les servos 2006/2007 *
 *************************************************************/

#ifndef _CARTE_SERVOS_2007_H_
#define _CARTE_SERVOS_2007_H_

#include "Carte.h"

class CarteServos2007 : public Carte {
public:
	CarteServos2007(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Règle la position d'un servo
	void reglerPosition(uint8_t servo, uint8_t angle);

	// Vérifie qu'un ack ait été reçu
	bool ackRecu();

private:
	std::atomic_bool _ack;
};

#endif
