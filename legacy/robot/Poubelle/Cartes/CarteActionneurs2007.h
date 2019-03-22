/*******************************************************************
 * CarteActionneurs2007.h *
 * Spécifie la classe gérant les moteurs et servos 2006/2007 *
 *******************************************************************/

#ifndef _CARTE_ACTIONNEURS_2007_H_
#define _CARTE_ACTIONNEURS_2007_H_

#include "Carte.h"

class CarteActionneurs2007 : public Carte {
public:
	CarteActionneurs2007(Commun::CAN& can, uint8_t id) : Carte(can, id){};

	virtual void traiterMessage(Trame const& message) override;

	// Règle la position d'un servo
	void reglerPosition(uint8_t servo, uint8_t angle);

	// Règle la vitesse d'un moteur
	void reglerVitesse(uint8_t moteur, uint8_t vitesse);

	// Vérifie qu'un ack ait été reçu
	bool ackRecu();

private:
	std::atomic_bool _ack;
};

#endif
