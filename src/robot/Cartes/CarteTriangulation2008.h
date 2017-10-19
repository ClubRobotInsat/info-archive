/************************************************************
 * CarteTriangulation2008.h *
 ************************************************************/

#ifndef _CARTE_EXEMPLE_H_
#define _CARTE_EXEMPLE_H_

#include "Carte.h"

class CarteTriangulation2008 : public Carte {
public:
	CarteTriangulation2008(Commun::CAN& can, uint8_t id) : Carte(can, id), _x(0), _y(0) {}

	virtual void traiterMessage(Trame const& message) override;

	// Récupère des coordonnées
	void recupereCoordonnees(std::int16_t* x, std::int16_t* y);

private:
	std::int16_t _x;
	std::int16_t _y;
};

#endif
