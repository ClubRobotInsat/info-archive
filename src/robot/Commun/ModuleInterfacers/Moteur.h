//
// Created by terae on 4/20/18.
//

#ifndef Club_Robot_Moteur_h
#define Club_Robot_Moteur_h

#include "../../Cartes/CarteMoteurs2018.h"
#include "../Robot.h"
#include <vector>

namespace Commun {
	class MoteurManager {
	public:
		explicit MoteurManager(Motors2019& module_motor) : _module(module_motor){};

		// fonctions pour activer plusieurs moteurs
		ResultatAction positionnerNMoteursBloquants(std::vector<uint8_t> moteurs, Angle pos);
		ResultatAction positionnerNMoteursBloquants(std::vector<std::pair<uint8_t, Motors2019::RotatingDirection>> moteurs,
		                                            std::size_t nbr_tours);

		ResultatAction tournerPlusieursOn(std::vector<std::pair<uint8_t, Motors2019::RotatingDirection>> moteurs);
		ResultatAction tournerPlusieursOff(std::vector<uint8_t> moteurs);


		// fonctions pour moteurs asservis
		ResultatAction positionnerMoteurBloquant(uint8_t moteur, Angle pos);

		ResultatAction positionnerMoteurBloquant(uint8_t moteur, std::size_t nbr_tours, Motors2019::RotatingDirection);

		// fonctions pour moteurs non asservis
		ResultatAction tournerOn(uint8_t moteur, Motors2019::RotatingDirection);

		ResultatAction tournerOff(uint8_t moteur);

		ResultatAction tournerOnBrushless(uint8_t moteur);

		ResultatAction tournerOffBrushless(uint8_t moteur);


	private:
		Motors2019& _module;
	};
} // namespace Commun

#endif // Club_Robot_Moteur_h
