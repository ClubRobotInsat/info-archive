//
// Created by terae on 4/20/18.
//

#ifndef ROOT_MOTEUR_H
#define ROOT_MOTEUR_H

// TODO: #include "../Cartes/CarteMoteurCommun.h"
#include "Robot.h"

// @Denis : Manager à écrire (exemple depuis 'Servo.h'), après avoir écrit la CarteMoteur2018.h
namespace Commun {
	class MoteurManager {
	public:
		enum class SensRotation : uint8_t { Horaire = 0, Trigo = 1 };

		// TODO
		// MoteurManager(CarteMoteur& carteMoteur) : _carte(carteMoteur);

		// fonctions pour moteurs asservis
		ResultatAction positionnerMoteurBloquant(uint8_t moteur, Angle pos);
		ResultatAction tournerMoteurNTours(uint8_t moteur, int nbr_tours);

		// fonctions pour moteurs non asservis
		ResultatAction tournerOn(uint8_t moteur, SensRotation);
		ResultatAction tournerOff(uint8_t moteur);

	private:
		// CarteMoteur& _carte;
	};
}

#endif // ROOT_MOTEUR_H
