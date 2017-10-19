//
//  Ascenseur.cpp
//  Club Robot
//
//  Created by Rémi on 03/03/2015.
//

#include "Ascenseur.h"
#include "../Cartes/CarteAsservissement.h"

namespace Commun {
	const Duration Ascenseur::TIMEOUT = 2_s;

	Ascenseur::Ascenseur(CarteAsservissement& carteAsservissement, std::vector<Angle> const& positions)
	        : _carteAsservissement(carteAsservissement), _positions(positions) {
		_carteAsservissement.activerAsservissementAngle(true);
	}

	ResultatAction Ascenseur::incrementer() {
		return this->positionnerAscBloquant(_position + 1);
	}

	ResultatAction Ascenseur::decrementer() {
		return this->positionnerAscBloquant(_position - 1);
	}

	ResultatAction Ascenseur::positionnerAscBloquant(std::size_t pos) {
		std::lock_guard<std::mutex> lock(_mutexEnMouvement);

		positionnerAsc(pos);
		return attendreAscAPos();
	}

	ResultatAction Ascenseur::allerAngleBloquant(Angle angle) {
		std::lock_guard<std::mutex> lock(_mutexEnMouvement);
		logDebug("L'ascenseur va à l'angle ", angle.toDeg());
		_carteAsservissement.tournerAbsolu(angle);

		return attendreAscAPos();
	}

	ResultatAction Ascenseur::positionnerAsc(std::size_t pos) {
		Angle angle = _positions.at(pos) + _offset;
		logDebug("L'ascenseur va à la position ", angle.toDeg());

		_position = pos;
		_carteAsservissement.tournerAbsolu(angle);

		return ResultatAction::REUSSI;
	}

	ResultatAction Ascenseur::attendreAscAPos() {
		StopWatch compteur;
		while(!verifierPositionnerAsc() && compteur.getElapsedTime() < TIMEOUT) {
			if(verifierAscBloque()) {
				_carteAsservissement.arreter();
				return ResultatAction::BLOQUE;
			}
			sleep(20_ms);
		}

		if(compteur.getElapsedTime() >= TIMEOUT) {
			_carteAsservissement.arreter();
			return ResultatAction::TIMEOUT;
		}

		return verifierPositionnerAsc() ? ResultatAction::REUSSI : ResultatAction::RATE;
	}

	bool Ascenseur::verifierPositionnerAsc() {
		return _carteAsservissement.verifierDeplacementTermine() || _carteAsservissement.verifierPrecisionAtteinte();
	}

	bool Ascenseur::verifierAscBloque() {
		return _carteAsservissement.verifierBlocage();
	}

	ResultatAction Ascenseur::initialiser(SensRotation /*sens*/) {
		_carteAsservissement.tournerAbsolu(-10000_rad);
		auto res = attendreAscAPos();
		_carteAsservissement.definirAngle(0_deg);
		return res;
	}

	Angle Ascenseur::getAngleBloquant() {
		_carteAsservissement.actualiserAngle();
		while(!_carteAsservissement.angleActualise())
			sleep(1_ms);
		return _carteAsservissement.lireAngle();
	}

	void Ascenseur::setVitesse(AngularSpeed v) {
		_carteAsservissement.reglerParametre(CarteAsservissement::Angulaire_VitesseMax, v.toMilliRad_s<float>());
	}
}
