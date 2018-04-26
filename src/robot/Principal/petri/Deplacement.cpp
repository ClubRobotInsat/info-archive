//
// Created by graphman on 24/04/15.
//

#include "Deplacement.h"
#include "../librobot/libRobot.h"
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
}

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

ResultatAction recallageHaut(Distance y) {
	ResultatAction result = tournerAbsolu(-90_deg);
	if(result != ResultatAction::REUSSI) {
		resetVitesseLineaire();
		return result;
	}

	setVitesseLineaireLente();
	result = dep().avancerInfini(SensAvance::Arriere, 10_s);
	resetVitesseLineaire();

	if(result != ResultatAction::REUSSI && result != ResultatAction::BLOQUE) {
		dep().arreter();
		return result;
	}

	dep().setRepere({positionRobot().x, y}, -90_deg);
	return ResultatAction::REUSSI;
}

ResultatAction recallageBas(Distance y) {
	ResultatAction result = tournerAbsolu(90_deg);
	if(result != ResultatAction::REUSSI) {
		resetVitesseLineaire();
		return result;
	}

	setVitesseLineaireLente();
	result = dep().avancerInfini(SensAvance::Arriere, 10_s);
	resetVitesseLineaire();

	if(result != ResultatAction::REUSSI && result != ResultatAction::BLOQUE) {
		dep().arreter();
		return result;
	}

	dep().setRepere({positionRobot().x, y}, 90_deg);
	return ResultatAction::REUSSI;
}

ResultatAction recallageDroit(Distance x) {
	ResultatAction result = tournerAbsolu(180_deg);
	if(result != ResultatAction::REUSSI) {
		resetVitesseLineaire();
		return result;
	}

	setVitesseLineaireLente();
	result = dep().avancerInfini(SensAvance::Arriere, 10_s);
	resetVitesseLineaire();

	if(result != ResultatAction::REUSSI && result != ResultatAction::BLOQUE) {
		dep().arreter();
		return result;
	}

	dep().setRepere({x, positionRobot().y}, 180_deg);
	return ResultatAction::REUSSI;
}

ResultatAction recallageGauche(Distance x) {
	ResultatAction result = tournerAbsolu(0_deg);
	if(result != ResultatAction::REUSSI) {
		resetVitesseLineaire();
		return result;
	}

	setVitesseLineaireLente();
	result = dep().avancerInfini(SensAvance::Arriere, 10_s);
	resetVitesseLineaire();

	if(result != ResultatAction::REUSSI && result != ResultatAction::BLOQUE) {
		dep().arreter();
		return result;
	}

	dep().setRepere({x, positionRobot().y}, 0_deg);
	return ResultatAction::REUSSI;
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

double angleRobot() {
	return robot().lireCoordonnees().getAngle().toDeg();
}

double distanceRobotPosition(Distance x, Distance y) {
	return (positionRobot() - Position(x, y, robot().getStrategie().getReference()).getPos2D(ABSOLUTE_REFERENCE)).norm().toMm();
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
