/************************************************************
 * CarteXbee2010.h *
 * Spécifie la classe gérant les Xbee 2009/2010 *
 ************************************************************/

#ifndef _CARTE_XBEE_2010_H_
#define _CARTE_XBEE_2010_H_

#include "Carte.h"

class CarteXbee2010 : public Carte {
public:
	CarteXbee2010(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Envoie d'un octet au xbee
	void envoyerOctet(uint8_t octet);

	// retourne vrai si il y a un octet recu
	bool aOctetRecu();

	// Retourne l'octet recu
	uint8_t getOctetRecu();

private:
	std::atomic_bool _aOctetRecu;
	std::atomic<uint8_t> _octetRecu;
};

#endif
