/************************************************************
 * CarteLed2008.cpp *
 * Implémente la classe gérant les LEDs Tunning 2007/2008 *
 ************************************************************/

#ifndef _CARTE_LED_2008_H_
#define _CARTE_LED_2008_H_

#include "Carte.h"

class CarteLed2008 : public Carte {
public:
	CarteLed2008(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Change la séquence affichée en execution automatique par la carte LED
	void reglerSequence(uint8_t numero_sequence);

	// Le PC prend le contrôle pour afficher sa propre sequence sur la carte LED
	void afficherSequence(uint16_t sequence);

	// Le PC rend la main au contrôle automatique des sequences apr la carte LED
	void rendreLaMain();
};

#endif
