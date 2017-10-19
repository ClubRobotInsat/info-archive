//
// Created by louis on 20/04/17.
//

#include "Evitement.h"

#include "Strategie.h"

#include "../../commun/RobotPrincipal/Constantes.h"

namespace Commun {

	Evitement::Evitement(Robot& robot, Environment& env)
	        : _robot(robot), _env(env), _turretPosition(_robot.getPositionTourelle()) {
		logDebug8("Position de la tourelle : ", _turretPosition);
	}

	void Evitement::setPositionCapteur(const Vector2m& pos) {
		_turretPosition = pos;
	}

	Coordonnees Evitement::getPositionAdversaire() {
		const repere::Repere& reference = _robot.getCarteDeplacement().getReference();

		Coordonnees start = _robot.lireCoordonnees();
		// L'angle est récupéré selon le repère de la carte déplacement
		Angle angleAdv = -_robot.getAngleAdversaireDetecte().toMinusPiPi();
		if(abs(angleAdv) < 0.001_deg) // si la carte nous dit qu'il n'y a pas d'adv
			return Coordonnees(Vector2m(-1_m, -1_m), 0_deg, reference);
		Distance dst = _env.getRobotRadius();

		// Repère du robot
		Vector2m robotDirection =
		    Vector2m(Distance::makeFromM(cos(start.getAngle())), Distance::makeFromM(sin(start.getAngle())));
		Vector2m robotNormal = Vector2m(Distance::makeFromM(cos(start.getAngle() + 90_deg)),
		                                Distance::makeFromM(sin(start.getAngle() + 90_deg)));

		// Projection de la direction de l'adv dans le repère du robot
		Vector2m advDirection = cos(angleAdv) * robotDirection + sin(angleAdv) * robotNormal;

		// Déplacement dû à la tourelle
		Vector2m positionOffset = (_turretPosition.x.toM() * robotDirection) + (_turretPosition.y.toM() * robotNormal);

		// On considère le robot adverse comme étant juste devant notre robot.
		Vector2m advPosition = start.getPos2D() + positionOffset + advDirection * 2.3 * dst.toM();

		return Coordonnees(advPosition, 0_deg, reference);
	}

	Angle Evitement::getPositionAngulaireAdversaire() {
		Angle angleAdv = _robot.getAngleAdversaireDetecte();

		if(angleAdv < 0.001_deg) {
			return angleAdv;
		} else {
			return shiftAngle(angleAdv);
		}
	}

	bool Evitement::adversaireDetecte() {
		return _robot.getAngleAdversaireDetecte().toMinusPiPi() >= 0.001_deg;
	}

	bool Evitement::adversairePresent(SensAvance sens) {
		Angle angleAdv = getPositionAngulaireAdversaire();
		Angle demiConeDetection = Angle::makeFromMilliRad(_robot.getAngleDetectionAdv());
		bool present =
		    abs(angleAdv) >= 0.001_deg && ((sens == SensAvance::Avant && abs(angleAdv) < demiConeDetection) ||
		                                   (sens == SensAvance::Arriere && abs(angleAdv) > (1_PI - demiConeDetection)));
		// logMagenta("Adv present : ", present);
		return present;
	}

	Angle Evitement::shiftAngle(Angle angle) {
		Distance distAdv = 2.3 * _env.getRobotRadius();
		Distance y = sin(angle) * distAdv + _turretPosition.y;
		Distance x = cos(angle) * distAdv + _turretPosition.x;
		return atan2(y, x);
	}
}