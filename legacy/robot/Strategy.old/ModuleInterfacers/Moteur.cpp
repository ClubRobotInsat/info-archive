//
// Created by terae on 4/20/18.
//

#include "Moteur.h"
#include <utility>

namespace Commun {
	ResultatAction MoteurManager::positionnerNMoteursBloquants(std::vector<uint8_t> moteurs, Angle pos) {
		auto TIMEOUT_MOTEUR = 1_s;
		auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

		for(uint8_t motor : moteurs) {
			_module.position_angle(motor, pos);
		}

		do {
			if(TimePoint::now() >= timeoutDate) {
				logWarn("TIMEOUT !");
				return ResultatAction::TIMEOUT;
			}

			bool fini = true;
			for(uint8_t motor : moteurs) {
				fini = fini && _module.is_position_finished(motor);
			}

			if(fini) {
				return ResultatAction::REUSSI;
			}
			sleep(50_ms);
		} while(true);
	}

	ResultatAction MoteurManager::positionnerNMoteursBloquants(std::vector<std::pair<uint8_t, Motors2019::RotatingDirection>> moteurs,
	                                                           std::size_t nbr_tours) {
		auto TIMEOUT_MOTEUR = 1_s * nbr_tours;
		auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

		for(auto motor : moteurs) {
			_module.position_turns(motor.first, static_cast<uint8_t>(nbr_tours), motor.second);
		}

		do {
			if(TimePoint::now() >= timeoutDate) {
				logWarn("TIMEOUT !");
				return ResultatAction::TIMEOUT;
			}

			bool fini = true;
			for(const auto& motor : moteurs) {
				fini = fini && _module.is_position_finished(motor.first);
			}

			if(fini) {
				return ResultatAction::REUSSI;
			}
			sleep(50_ms);
		} while(true);
	}

	ResultatAction MoteurManager::tournerPlusieursOn(std::vector<std::pair<uint8_t, Motors2019::RotatingDirection>> moteurs) {
		for(auto motor : moteurs) {
			_module.activate_uncontrolled_motor(motor.first, motor.second);
		}
		return ResultatAction::REUSSI;
	}

	ResultatAction MoteurManager::tournerPlusieursOff(std::vector<uint8_t> moteurs) {
		for(uint8_t motor : moteurs) {
			_module.deactivate_uncontrolled_motor(motor);
		}
		return ResultatAction::REUSSI;
	}

	ResultatAction MoteurManager::positionnerMoteurBloquant(uint8_t moteur, Angle pos) {
		/*std::vector<uint8_t > vec_moteur;
		vec_moteur.insert(moteur);*/
		return this->positionnerNMoteursBloquants({moteur}, pos);
		/*
		    // TODO cf avec Paul Meis
		    auto TIMEOUT_MOTEUR = 1_s;
		    auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

		    _module.position_angle(moteur, pos);

		    while(true) {
		        if(TimePoint::now() >= timeoutDate) {
		            logWarn("TIMEOUT !");
		            return ResultatAction::TIMEOUT;
		        }

		        if(_module.is_position_finished(moteur) == true) {
		            return ResultatAction::REUSSI;
		        }
		        sleep(50_ms);
		    }*/
	}

	ResultatAction MoteurManager::positionnerMoteurBloquant(uint8_t moteur, std::size_t nbr_tours, Motors2019::RotatingDirection sens) {
		return this->positionnerNMoteursBloquants({std::make_pair(moteur, sens)}, nbr_tours);
		// return this->positionnerMoteursBloquants({std::make_pair<uint8_t, Motors2019::RotatingDirection>(moteurs)},
		// nbr_tours);
		// return this->positionnerMoteursBloquants(moteurs,nbr_tours);

		/*
		auto TIMEOUT_MOTEUR = 1_s * nbr_tours;
		auto timeoutDate = TimePoint::now() + TIMEOUT_MOTEUR;

		bool Position = _module.is_position_finished(moteur);
		_module.position_turns(moteur, static_cast<uint8_t>(nbr_tours), sens);

		while(true) {
		    if(TimePoint::now() >= timeoutDate) {
		        logWarn("TIMEOUT !");
		        return ResultatAction::TIMEOUT;
		    }

		    if(_module.is_position_finished(moteur) == true) {
		        return ResultatAction::REUSSI;
		    }
		    sleep(50_ms);
		}
		 */
	}


	ResultatAction MoteurManager::tournerOn(uint8_t moteur, Motors2019::RotatingDirection sens) {
		return this->tournerPlusieursOn({std::make_pair(moteur, sens)});
	};

	ResultatAction MoteurManager::tournerOff(uint8_t moteur) {
		return this->tournerPlusieursOff({moteur});
	};

	ResultatAction MoteurManager::tournerOnBrushless(uint8_t moteur) {
		_module.activate_brushless(moteur);
		return ResultatAction::REUSSI;
	}

	ResultatAction MoteurManager::tournerOffBrushless(uint8_t moteur) {
		_module.deactivate_brushless(moteur);
		return ResultatAction::REUSSI;
	}
} // namespace Commun
