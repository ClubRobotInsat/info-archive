//
// Created by terae on 4/20/18.
//

#ifndef ROOT_MOTEUR_H
#define ROOT_MOTEUR_H

#include "../Cartes/CarteMoteurs2018.h"
#include "Robot.h"
#include <vector>

// @Denis : Manager à écrire (exemple depuis 'Servo.h'), après avoir écrit la CarteMoteur2018.h
namespace Commun {
	class MoteurManager {
	public:
		// TODO
		MoteurManager(CarteMoteurs2018& carteMoteur) : _carte(carteMoteur){};

		// fonctions pour activer plusieurs moteurs
		ResultatAction positionnerMoteursBloquants(std::vector<uint8_t> moteurs, Angle pos);
		ResultatAction positionnerMoteursBloquants(std::vector<uint8_t> moteurs, std::size_t nbr_tours, CarteMoteurs2018::SensRotation sens) {


			// fonctions pour moteurs asservis
			ResultatAction positionnerMoteurBloquant(uint8_t moteur, Angle pos);
			ResultatAction positionnerMoteurBloquant(uint8_t moteur, std::size_t nbr_tours, CarteMoteurs2018::SensRotation);

			// fonctions pour moteurs non asservis
			ResultatAction tournerOn(uint8_t moteur, CarteMoteurs2018::SensRotation);
			ResultatAction tournerOff(uint8_t moteur);

			ResultatAction tournerOnBrushless(uint8_t moteur);


		private:
			CarteMoteurs2018& _carte;
		};
}

#endif // ROOT_MOTEUR_H
