/*
 * CarteColorimetre2014.h
 *
 *  Created on: 5 mai 2014
 *      Author: Romain Taprest
 */

#ifndef CARTECOLORIMETRE2014_H_
#define CARTECOLORIMETRE2014_H_

#include "Carte.h"


class CarteColorimetre2014 : public Carte {
public:
	enum CouleurColorimetre : uint8_t {
		COLORIMETRE_ROUGE,
		COLORIMETRE_JAUNE,
		COLORIMETRE_BLEU,
		COLORIMETRE_CYAN,
		COLORIMETRE_MAGENTA,
		COLORIMETRE_VERT,
		COLORIMETRE_INCONNUE
	};
	CarteColorimetre2014(Commun::CAN&, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Fonction qui renvoie la couleur détectée par le colorimètre
	CouleurColorimetre lireCouleurColorimetre();

	// Actualiser la valeur interne de la couleur detectée
	void actualiserCouleurColorimetre();

	// Vérifie que la variable interne de la couleur est bien actualisée
	bool couleurEstActualisee();

private:
	std::atomic<CouleurColorimetre> _couleur;
	std::atomic_bool _couleurActualisee;
};


#endif /* CARTECOLORIMETRE2014_H_ */
