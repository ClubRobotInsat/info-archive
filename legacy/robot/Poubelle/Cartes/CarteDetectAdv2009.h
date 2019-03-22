/**********************************************************************************************
 * CarteDetectAdv2009.h *
 * Spécifie la classe gérant la balise rotative pour la detection de l'adversaire 2008/2009 *
 ***********************************************************************************************/

#ifndef _CARTE_DETECTADV_2009_H_
#define _CARTE_DETECTADV_2009_H_

#include "Carte.h"
#include "CarteAsservissement2009.h"
#include "CarteDeplacementCommun.h"

class CarteDetectAdv2009 : public CarteAsservissement2009 {
public:
	CarteDetectAdv2009(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	void actualiserMesureAdv();

	bool verifierMesureAdvActualise();

	Angle getDiametreAngulaire();

	// compris entre -PI et PI
	Angle getAngle();

	// Pour le debug au simu
	Distance getDistance();

	void activerEnvoiMesureAdvAuto();

	void desactiverEnvoiMesureAdvAuto();

private:
	Angle _diametreAngulaire; // angle sous lequel est vu le catadioptre
	Angle _angle;
	Distance _distance;
	std::atomic_bool _mesureAdvActualise;
};

#endif
