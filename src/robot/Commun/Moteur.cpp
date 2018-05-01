//
// Created by terae on 4/20/18.
//

#include "Moteur.h"

ResultatAction Commun::MoteurManager::positionnerMoteurBloquant(uint8_t moteur, Angle pos) {

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
	}
}

ResultatAction Commun::MoteurManager::positionnerMoteurBloquant(uint8_t moteur, std::size_t nbr_tours, CarteMoteurs2018::SensRotation sens) {

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
}


ResultatAction Commun::MoteurManager::tournerOn(uint8_t moteur, CarteMoteurs2018::SensRotation sens) {
	_carte.activerMoteur(moteur, sens);
	return ResultatAction::REUSSI;
};

ResultatAction Commun::MoteurManager::tournerOff(uint8_t moteur) {
	_carte.desactiverMoteur(moteur);
	return ResultatAction::REUSSI;
};

ResultatAction Commun::MoteurManager::tournerOnBrushless(uint8_t moteur) {
	_carte.activerBrushless(moteur);
	return ResultatAction::REUSSI;
}
