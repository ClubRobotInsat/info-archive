//
// Created by terae on 4/20/18.
//

#include "Moteur.h"
#include <utility>


ResultatAction Commun::MoteurManager::positionnerNMoteursBloquants(std::vector<uint8_t> moteurs, Angle pos) {
	auto TIMEOUT_MOTEUR = 1_s;
	auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

	for(uint8_t motor : moteurs) {
		_carte.positionAngle(motor, pos);
	}

	do {
		if(TimePoint::now() >= timeoutDate) {
			logWarn("TIMEOUT !");
			return ResultatAction::TIMEOUT;
		}

		bool fini = true;
		for(uint8_t motor : moteurs) {
			fini = fini && _carte.lirePosition(motor);
		}

		if(fini) {
			return ResultatAction::REUSSI;
		}
		sleep(50_ms);
	} while(true);
}

ResultatAction Commun::MoteurManager::positionnerNMoteursBloquants(std::vector<std::pair<uint8_t, CarteMoteurs2018::SensRotation>> moteurs,
                                                                   std::size_t nbr_tours) {
	auto TIMEOUT_MOTEUR = 1_s * nbr_tours;
	auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

	for(auto motor : moteurs) {
		_carte.positionTour(motor.first, static_cast<uint8_t>(nbr_tours), motor.second);
	}

	do {
		if(TimePoint::now() >= timeoutDate) {
			logWarn("TIMEOUT !");
			return ResultatAction::TIMEOUT;
		}

		bool fini = true;
		for(std::pair<uint8_t, CarteMoteurs2018::SensRotation> motor : moteurs) {
			fini = fini && _carte.lirePosition(motor.first);
		}

		if(fini) {
			return ResultatAction::REUSSI;
		}
		sleep(50_ms);
	} while(true);
}

ResultatAction Commun::MoteurManager::tournerPlusieursOn(std::vector<std::pair<uint8_t, CarteMoteurs2018::SensRotation>> moteurs) {
	for(auto motor : moteurs) {
		_carte.activerMoteur(motor.first, motor.second);
	}
	return ResultatAction::REUSSI;
}

ResultatAction Commun::MoteurManager::tournerPlusieursOff(std::vector<uint8_t> moteurs) {
	for(uint8_t motor : moteurs) {
		_carte.desactiverMoteur(motor);
	}
	return ResultatAction::REUSSI;
}

ResultatAction Commun::MoteurManager::positionnerMoteurBloquant(uint8_t moteur, Angle pos) {
	/*std::vector<uint8_t > vec_moteur;
	vec_moteur.insert(moteur);*/
	return this->positionnerNMoteursBloquants({moteur}, pos);
	/*
	    // TODO cf avec Paul Meis
	    auto TIMEOUT_MOTEUR = 1_s;
	    auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

	    _carte.positionAngle(moteur, pos);

	    while(true) {
	        if(TimePoint::now() >= timeoutDate) {
	            logWarn("TIMEOUT !");
	            return ResultatAction::TIMEOUT;
	        }

	        if(_carte.lirePosition(moteur) == true) {
	            return ResultatAction::REUSSI;
	        }
	        sleep(50_ms);
	    }*/
}

ResultatAction Commun::MoteurManager::positionnerMoteurBloquant(uint8_t moteur, std::size_t nbr_tours, CarteMoteurs2018::SensRotation sens) {

	return this->positionnerNMoteursBloquants({std::make_pair(moteur, sens)}, nbr_tours);
	// return this->positionnerMoteursBloquants({std::make_pair<uint8_t, CarteMoteurs2018::SensRotation>(moteurs)},
	// nbr_tours);
	// return this->positionnerMoteursBloquants(moteurs,nbr_tours);

	/*
	auto TIMEOUT_MOTEUR = 1_s * nbr_tours;
	auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

	bool Position = _carte.lirePosition(moteur);
	_carte.positionTour(moteur, static_cast<uint8_t>(nbr_tours), sens);

	while(true) {
	    if(TimePoint::now() >= timeoutDate) {
	        logWarn("TIMEOUT !");
	        return ResultatAction::TIMEOUT;
	    }

	    if(_carte.lirePosition(moteur) == true) {
	        return ResultatAction::REUSSI;
	    }
	    sleep(50_ms);
	}
	 */
}


ResultatAction Commun::MoteurManager::tournerOn(uint8_t moteur, CarteMoteurs2018::SensRotation sens) {
	return this->tournerPlusieursOn({std::make_pair(moteur, sens)});
	//_carte.activerMoteur(moteur, sens);
	// return ResultatAction::REUSSI;
};

ResultatAction Commun::MoteurManager::tournerOff(uint8_t moteur) {
	//_carte.desactiverMoteur(moteur);
	// return ResultatAction::REUSSI;
	return this->tournerPlusieursOff({moteur});
};

ResultatAction Commun::MoteurManager::tournerOnBrushless(uint8_t moteur) {
	_carte.activerBrushless(moteur);
	return ResultatAction::REUSSI;
}
