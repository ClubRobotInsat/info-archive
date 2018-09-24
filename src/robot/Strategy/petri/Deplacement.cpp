//
// Created by graphman on 24/04/15.
//

#include "Deplacement.h"
#include "../../../Principal/librobot/libRobot.h"
#include "MathToolbox/Repere.h"
#include <Commun.h>
#include <map>

#define TEMPO_DEPLACEMENT 100_ms
void initDeplacementPetri(StrategiePrincipal& strategie);

namespace {
	StrategiePrincipal* _strategie;

	Commun::Deplacement& dep() {
		return _strategie->getDeplacement();
	}


	RobotPrincipal& robot() {
		return static_cast<RobotPrincipal&>(_strategie->getRobot());
	}

	MecaManagerPrincipal& meca() {
		return _strategie->getMecaManager();
	}
} // namespace

void initDeplacementPetri(StrategiePrincipal& strategie) {
	_strategie = &strategie;
}

// Déplacements sans limite de distance

ResultatAction avancerInfini(Duration timeout) {
	return dep().avancerInfini(SensAvance::Avant, timeout);
}

ResultatAction reculerInfini(Duration timeout) {
	return dep().avancerInfini(SensAvance::Arriere, timeout);
}

ResultatAction tournerAbsolu(Angle angle) {
	logInfo("Tourner Absolu ", angle.toDeg(), " _deg : ");
	Duration timeout = abs(angle) / dep().getVitesseAngulaire() + ADD_TIMEOUT_SECURITY;
	auto res = dep().tournerAbsolu(angle, timeout);
	if(res != ResultatAction::REUSSI) {
		logError("Échec de Tourner Absolu ", angle.toDeg(), " _deg : ", res);
	}

	sleep(TEMPO_DEPLACEMENT); // compensation carte elec
	return res;
}

ResultatAction tournerRelatif(Angle angle) {
	logInfo("Tourner Relatif ", angle.toDeg(), " _deg : ");
	Duration timeout = abs(angle) / dep().getVitesseAngulaire() + ADD_TIMEOUT_SECURITY;
	auto res = dep().tournerRelatif(angle, timeout);
	if(res != ResultatAction::REUSSI) {
		logError("Échec de Tourner Relatif ", angle.toDeg(), " _deg : ", res);
	}
	sleep(TEMPO_DEPLACEMENT); // compensation carte élec
	return res;
}

// A star
ResultatAction allerA(Distance x, Distance y, SensAvance sens) {
	return allerA_vec(Vector2m{x, y}, sens);
}

ResultatAction allerA(Distance x, Distance y, SensAvance sens, Duration timeout) {
	return allerA_vec({x, y}, sens, timeout);
}

ResultatAction allerA(Distance x, Distance y) {
	return allerA_vec({x, y}, SensAvance::Avant);
}

ResultatAction allerA_vec(Vector2m pos, SensAvance sens, Duration timeout) {
	logInfo("Aller A ", pos);
	setVitesseAngulaireLente();
	auto res = dep().allerA(pos, sens, timeout);
	if(res != ResultatAction::REUSSI && res != ResultatAction::POSITION_MODIFIEE) {
		logError("Échec de Aller A ", pos, " : ", res);
	}
	sleep(TEMPO_DEPLACEMENT); // compense la précision de la carte élec
	resetVitesseAngulaire();
	return res;
}

ResultatAction allerA_vec(Vector2m pos) {
	return allerA_vec(pos, SensAvance::Avant);
}

// Ces constantes permettent aux recallages d'etre immunisés à un cube farceur qui se glisse entre le robot et un mur
static constexpr Angle OFFSET_ANGLE_MAX_RECALLAGE = 8_deg;
static constexpr Distance OFFSET_DISTANCE_MAX_RECALLAGE = 5_cm;

// @param isX : true si en fonction de X, false si en fonction de Y
ResultatAction recallageHelper(SensAvance sens, Distance D, std::pair<Angle, Angle> angles, bool isX) {
	Angle angle = (sens == SensAvance::Avant ? angles.first : angles.second);
	ResultatAction result = tournerAbsolu(angle);
	if(result != ResultatAction::REUSSI) {
		resetVitesseLineaire();
		return result;
	}

	setVitesseLineaireLente();
	result = dep().avancerInfini(sens, 10_s);
	resetVitesseLineaire();

	if(result != ResultatAction::REUSSI && result != ResultatAction::BLOQUE) {
		dep().arreter();
		return result;
	}

	Vector2m pos_old = positionRobot();
	Angle angle_old = angleRobot();

	Distance distance_old = (isX ? pos_old.x : pos_old.y);

	Distance variation_distance = (distance_old >= D ? distance_old - D : D - distance_old);
	Angle variation_angle = (angle_old >= angle ? angle_old - angle : angle - angle_old).toMinusPiPi();

	if(variation_distance > OFFSET_DISTANCE_MAX_RECALLAGE) {
		logWarn("Problème lors du recallage détecté : variation entre la position actuelle et la position attendue en ",
		        (isX ? "X" : "Y"),
		        " de ",
		        variation_distance);
		return ResultatAction::BLOQUE;
	}
	if(variation_angle > OFFSET_ANGLE_MAX_RECALLAGE) {
		logWarn("Problème lors du recallage détecté : variation entre l'angle actuel et la position attendue de ", variation_angle);
		return ResultatAction::BLOQUE;
	}

	if(isX) {
		dep().setRepere({D, distance_old}, angle);
	} else {
		dep().setRepere({distance_old, D}, angle);
	}
	return ResultatAction::REUSSI;
}

ResultatAction recallageHaut(SensAvance sens, Distance y) {
	return recallageHelper(sens, y, std::make_pair(90_deg, -90_deg), false);
}

ResultatAction recallageBas(SensAvance sens, Distance y) {
	return recallageHelper(sens, y, std::make_pair(-90_deg, 90_deg), false);
}

ResultatAction recallageDroit(SensAvance sens, Distance x) {
	return recallageHelper(sens, x, std::make_pair(0_deg, 180_deg), true);
}

ResultatAction recallageGauche(SensAvance sens, Distance x) {
	return recallageHelper(sens, x, std::make_pair(180_deg, 0_deg), true);
}

// Déplacements avec détection de l'adversaire


ResultatAction avancerDe(Distance distance) {
	return avancerDe(distance, SensAvance::Avant);
}

ResultatAction reculerDe(Distance distance) {
	return avancerDe(distance, SensAvance::Arriere);
}

ResultatAction avancerDe(Distance distance, SensAvance sens) {
	Duration timeout = distance / dep().getVitesseLineaire() + ADD_TIMEOUT_SECURITY;
	auto res = dep().avancerDe(distance, sens, timeout);
	sleep(TEMPO_DEPLACEMENT); // permet d'atteindre que le déplacement soit réellement terminé.
	return res;
}

// Arrêt du robot

ResultatAction arreter() {
	dep().arretUrgence();
	return ResultatAction::REUSSI;
}

// Fonctions utilitaires

Vector2m positionRobot() {
	return robot().lireCoordonnees().getPos2D();
}

Angle angleRobot() {
	return robot().lireCoordonnees().getAngle();
}

Distance distanceRobotPosition(Distance x, Distance y) {
	return (positionRobot() - Position(x, y, robot().getStrategie().getReference()).getPos2D(ABSOLUTE_REFERENCE)).norm();
}

bool advProche(Distance distance) {
	Vector2m posAdv = _strategie->getEvitement().getPositionAdversaire().getPos2D();
	return pow(posAdv.x.toM() - positionRobot().x.toM(), 2) + pow(posAdv.y.toM() - positionRobot().x.toM(), 2) <=
	       pow(distance.toM(), 2);
}

// Changements d'allure

ResultatAction setVitesseAngulaireLente() {
	dep().pushVitesseAngulaire(dep().getVitesseAngulaire() / 2);
	return ResultatAction::REUSSI;
}

ResultatAction setVitesseAngulaireRapide() {
	dep().pushVitesseAngulaire(dep().getVitesseAngulaire() * 2);
	return ResultatAction::REUSSI;
}

ResultatAction resetVitesseAngulaire() {
	dep().popVitesseAngulaire();
	return ResultatAction::REUSSI;
}

ResultatAction setVitesseAngulaire(AngularSpeed s) {
	dep().pushVitesseAngulaire(s);
	return ResultatAction::REUSSI;
}

AngularSpeed getVitesseAngulaire() {
	return dep().getVitesseAngulaire();
}

ResultatAction setVitesseLineaireLente() {
	dep().pushVitesseLineaire(dep().getVitesseLineaire() * .75);
	return ResultatAction::REUSSI;
}

ResultatAction setVitesseLineaireRapide() {
	dep().pushVitesseLineaire(dep().getVitesseLineaire() / .4);
	return ResultatAction::REUSSI;
}

ResultatAction resetVitesseLineaire() {
	dep().popVitesseLineaire();
	return ResultatAction::REUSSI;
}

ResultatAction setVitesseLineaire(Speed s) {
	dep().pushVitesseLineaire(s);
	return ResultatAction::REUSSI;
}

Speed getVitesseLineaire() {
	return dep().getVitesseLineaire();
}
