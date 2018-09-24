/**********************
 * CarteMoteurs2018.h*
 * définit la classe gérant les moteurs 2018*
 ***********************/

#ifndef _CARTE_MOTEURS_2018_H
#define _CARTE_MOTEURS_2018_H

#define NB_MAX_MOTEURS 16

#include "Carte.h"

class CarteMoteurs2018 : public Carte {

public:
	CarteMoteurs2018(Commun::CAN& can, uint8_t ID) : Carte(can, ID) {
		std::fill(std::begin(_termine), std::end(_termine), false);
	};


	enum SensRotation : uint8_t { Horaire = 0, Trigo = 1 };

	enum Commande {
		POSITION_ANGLE = 0x01,
		TOUR = 0x02,
		ACTIVER = 0x03,
		DESACTIVER = 0x04,
		ACTIVER_BRUSHLESS = 0x05,
		POSITION_TERMINEE = 0x06
	};

	virtual void traiterMessage(Trame const& message) override;

	void activerMoteur(uint8_t moteur, SensRotation);

	void desactiverMoteur(uint8_t moteur);

	void activerBrushless(uint8_t moteur);

	// float Position_Angle_Moteur (IDMoteurs ID);
	void positionAngle(uint8_t moteur, Angle angle);

	// void Tourner_Moteur (IDMoteurs ID);
	void positionTour(uint8_t moteur, uint8_t nbr_tours, SensRotation);

	bool lirePosition(uint8_t moteur);

private:
	std::mutex _mutexPositions;
	std::atomic<bool> _termine[NB_MAX_MOTEURS];
};

#endif