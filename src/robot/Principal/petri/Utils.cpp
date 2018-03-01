#include "Utils.h"
#include "../librobot/libRobot.h"

void initUtilsPetri(StrategiePrincipal& strategie);

namespace {
	StrategiePrincipal* _strategie;

	RobotPrincipal& robot() {
		return _strategie->getRobot();
	}
}

void initUtilsPetri(StrategiePrincipal& strategie) {
	_strategie = &strategie;
}

Distance robotDx(Distance dx) {
	auto angleRobot = robot().lireCoordonnees().getAngle();
	logDebug("Angle robot: ", angleRobot);
	return dx * sin(angleRobot);
}

Distance robotDy(Distance dy) {
	auto angleRobot = robot().lireCoordonnees().getAngle();
	logDebug("Angle robot: ", angleRobot);
	return dy * cos(angleRobot);
}

Distance mm(int64_t d) {
	return Distance::makeFromMm(d);
}

bool couleurOrange() {
	return _strategie->getCouleurEquipe() == Constantes::RobotColor::Orange;
}