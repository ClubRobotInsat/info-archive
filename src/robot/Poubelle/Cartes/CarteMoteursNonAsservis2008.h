/*******************************************************************
 * CarteMoteursNonAsservis2008.h *
 * Spécifie la classe gérant les moteurs 2007/2008 *
 *******************************************************************/

#ifndef _CARTE_MoteursNonAsservis_2008_H_
#define _CARTE_MoteursNonAsservis_2008_H_

#include "Carte.h"

class CarteMoteursNonAsservis2008 : public Carte {
public:
	static const unsigned char MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR = 3;
	static const unsigned char MOTEUR_NON_ASSERVIS_PUISSANCE_MAX = 100;

	CarteMoteursNonAsservis2008(Commun::CAN& can, uint8_t id) : Carte(can, id) {}

	virtual void traiterMessage(Trame const& message) override;

	// Règle la puissance d'un moteur
	void reglerPuissance(uint8_t moteur, int8_t puissance);

	// Vérifie qu'un ack ait été reçu
	bool ackRecu();

private:
	std::atomic_bool _ack;
};

#endif
