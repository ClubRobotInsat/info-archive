/************************************************************
 * POBEye2007.h *
 * Spécifie la classe de la POBEye 2006/2007 *
 ************************************************************/

#ifndef _POBEYE_2007_H_
#define _POBEYE_2007_H_

#include "Carte.h"
#include <cstring>

struct Pixel {
	uint8_t x;
	uint8_t y;
};

class POBEye2007 : public Carte {
public:
	enum { LARGEUR_CAPTURE = 88, HAUTEUR_CAPTURE = 120 };

	POBEye2007(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	/// Demande une mise a jour des positions des barycentres des canettes et bouteilles
	void actualiserBarycentres();

	/// Vérifie que la liste des barycentres est à jour
	bool barycentresActualises();

	/// Retourne le nombre de canettes vues
	int nombreCanettesVues();

	/// Retourne la position du barycentre d'une canette
	Pixel barycentreCanette(int canette);

	int nombreBouteillesVues();

	Pixel barycentreBouteille(int bouteille);

private:
	std::atomic_bool _reception;

	std::atomic<uint8_t> _nombreCanettes;
	Pixel _canettes[100];

	std::atomic<uint8_t> _nombreBouteilles;
	Pixel _bouteilles[100];
};

#endif
