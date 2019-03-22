/************************************************************
 * CarteCodesBarres2011.cpp *
 * Spécifie la classe gérant les codes barres 2011 *
 ************************************************************/

#ifndef _CARTE_CODES_BARRES_2011_H_
#define _CARTE_CODES_BARRES_2011_H_

#include "Carte.h"

class CarteCodesBarres2011 : public Carte {
public:
	CarteCodesBarres2011(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	bool estCodeBarreDetecte(int numeroCodeBarre);

	// Retourne l'état d'un des contacteurs
	unsigned char getCodeLu(int numeroCodeBarre);

private:
	bool _codesBarresActualises[2];
	unsigned char _codesBarres[2];
};

#endif
