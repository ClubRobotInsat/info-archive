/*******************************************************************
 * CarteDebug2009.h *
 * Spécifie la classe gérant les actions de *
 * très haut niveau sur le robot 2009 (ramasser, déposer, etc...) *
 *******************************************************************/

#ifndef _CARTE_DEBUG_2009_H_
#define _CARTE_DEBUG_2009_H_

#include "Carte.h"

class CarteDebug2009 : public Carte {
public:
	CarteDebug2009(Commun::CAN& can, uint8_t id) : Carte(can, id) {}

	virtual void traiterMessage(Trame const& message) override;

	// Demande si un objet est à ramasser ou pas
	void demanderPaletARamasser();

	// Vérifie que la carte a répondu par un pong
	bool verifierPaletARamasser();

	// Ramasser un palet dans la colonne no x (1 ou 2)
	void ramasserPalet(uint8_t no_colonne);

	// Poser la colonne no x (1 ou 2) a une certaine hauteur (en nombre de palets)
	void poserColonne(uint8_t no_colonne, uint8_t hauteur);

	// Poser le linteau a une certaine hauteur (en nombre de palets)
	void poserLinteau(uint8_t hauteur);

	// ramasser un linteau
	void ramasserLinteau();

private:
	std::atomic_bool _paletARamasser;
};

#endif
