/*******************************************************************
 * CarteElectrovannes2016.h *
 * Spécifie la classe gérant les électrovannes 2016 *
 *******************************************************************/

#ifndef _CARTE_ELECTROVANNES_2016_H_
#define _CARTE_ELECTROVANNES_2016_H_

#include "Carte.h"


class CarteElectrovannes2016 : public Carte {
public:
	CarteElectrovannes2016(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Active la vanne d'id idVanne.
	void ouvrirVanne(uint8_t idVanne);

	// Désactive la vanne d'id idVanne.
	// 1-6 : correspond à une vanne, 7 : toutes les vannes
	void fermerVanne(uint8_t idVanne);

	// Obtient l'etat de la vanne.
	// 1-6 : correspond à une vanne, 7 : toutes les vannes
	bool etatVanne(uint8_t idVanne);

	// Vérifie qu'un ack ait été reçu
	bool ackRecu();


	static const int VANNES_COUNT = 6;

private:
	std::atomic_bool _ack;
	std::atomic_bool _states[VANNES_COUNT];
};

#endif
