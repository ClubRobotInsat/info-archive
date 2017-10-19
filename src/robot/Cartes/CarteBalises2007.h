/*********************************************************
 * CarteBalises2007.h *
 * Spécifie la classe de la carte des balises 2006/2007 *
 *********************************************************/

#ifndef _CARTE_BALISES_2007_H_
#define _CARTE_BALISES_2007_H_

#include "Carte.h"

class CarteBalises2007 : public Carte {
public:
	CarteBalises2007(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une mise à jour des récepteurs
	void actualiserRecepteurs();

	// Vérifie que les récepteurs sont à jour
	bool recepteursActualises();

	// Liste les récepteurs captant une balise donnée
	unsigned short lireRecepteurs(int balise);

private:
	unsigned short _recepteurs[4];
	bool _recepteursOk[2];
};

#endif
