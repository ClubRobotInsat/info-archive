#include "Utils.h"
#include "../../../Principal/librobot/libRobot.h"

void initUtilsPetri(StrategiePrincipal& strategie);

namespace {
	StrategiePrincipal* _strategie;

	RobotPrincipal& robot() {
		return _strategie->getRobot();
	}
} // namespace

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

bool couleurVerte() {
	return _strategie->getCouleurEquipe() == Constantes::RobotColor::Green;
}

bool couleurOrange() {
	return _strategie->getCouleurEquipe() == Constantes::RobotColor::Orange;
}

bool couleurUndef() {
	return _strategie->getCouleurEquipe() == Constantes::RobotColor::Undef;
}

ResultatAction setPoints(int points) {
	_strategie->setPoints(points);
	return ResultatAction::REUSSI;
}

ResultatAction addPoints(int points) {
	static std::mutex _mutex_points;
	std::lock_guard<std::mutex> lk(_mutex_points);
	_strategie->setPoints(_strategie->getPoints() + points);
	return ResultatAction::REUSSI;
}

int getPoints() {
	return _strategie->getPoints();
}