/************************************************************
 * CarteColorimetres2008.h *
 * Spécifie la classe gérant les colorimetres 2007/2008 *
 ************************************************************/

#ifndef _CARTE_COLORIMETRES_2008_H_
#define _CARTE_COLORIMETRES_2008_H_

#include "Carte.h"
#include <cstring>

#define NOMBRE_DE_CAPTEURS 2
#define NOMBRE_DE_COULEURS 3

class CarteColorimetres2008 : public Carte {
public:
	CarteColorimetres2008(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation des etats des colorimetres
	void actualiserCapteur(uint8_t capteur);

	// Demande si le capteur a bien ete actualise
	bool capteurActualise(uint8_t capteur);

	// modifie le tableau de 3 shorts passé en paramètre représentant l'état d'un des colorimètres
	void getCapteur(uint8_t capteur, uint16_t* rgb);

	// accesseur sur une des couleurs d'un des colorimetres
	uint16_t getCapteur(uint8_t capteur, uint8_t couleur);

	// accesseur sur les couleurs d'un des colorimetres
	Vector3us getCapteur(uint8_t capteur);

private:
	std::atomic<uint16_t> _capteurs[NOMBRE_DE_CAPTEURS][NOMBRE_DE_COULEURS];
	std::atomic_bool _ack[NOMBRE_DE_CAPTEURS];
};

#endif
