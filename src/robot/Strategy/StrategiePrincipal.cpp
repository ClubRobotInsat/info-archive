// StrategiePrincipal2015.cpp

#include "StrategiePrincipal.h"
#include "../../Commun/Deplacement.h"
#include "../Principal/librobot/Robot.h"
#include "MecaManager.h"
#include <cmath>

using namespace ConstantesPrincipal;
using namespace Constantes;
using namespace IDCartesPrincipal;

/*************** CONSTANTES ***************/

const Duration StrategiePrincipal::TEMPS_PRECISION_VIGIE_MAX = 1.5_s;
const Duration StrategiePrincipal::TEMPS_DEPLACEMENT_MAX = 3_s;

const int StrategiePrincipal::INCREMENT_BLOCAGE_LOGICIEL = 1;
const int StrategiePrincipal::INCREMENT_BLOCAGE_PHYSIQUE = 1;

const int StrategiePrincipal::NIVEAU_DANGER_LIGNE_DROITE = 3;
const std::size_t StrategiePrincipal::DISTANCE_LINEARISATION_MIN = 20;

const Angle StrategiePrincipal::TOLERANCE_ANGLE_TRAJECTOIRE_DEBUT = 20.0_deg;
const Angle StrategiePrincipal::TOLERANCE_ANGLE_EN_MOUVEMENT = 40.0_deg;
const Angle StrategiePrincipal::TOLERANCE_ANGLE_LINEARISATION = 10.0_deg;
const Angle StrategiePrincipal::ERREUR_ANGLE_TRAJECTOIRE_MAX = 100.0_deg;
const Duration StrategiePrincipal::TIMEOUT_FIN_DEPLACEMENT = 100_ms;
const Duration StrategiePrincipal::TIMEOUT_MAX = 10_s;
const Duration StrategiePrincipal::TIMEOUT_DEFAULT = 5_s;

const Duration StrategiePrincipal::DUREE_ATTENTE_ADVERSAIRE = 6_s;

extern void initDeplacementPetri(StrategiePrincipal& strategie);
extern void initUtilsPetri(StrategiePrincipal& strategie);
extern void initServoPetri(StrategiePrincipal& strategie);
extern void initMoteurPetri(StrategiePrincipal& strategie);

StrategiePrincipal::StrategiePrincipal(std::vector<std::string> const& args, Constantes::RobotColor const& color)
        : Strategie(std::make_unique<RobotPrincipal>(args), color) {
	// logDebug("Initialisation du MecaManager");
	_meca = std::make_unique<MecaManagerPrincipal>(getRobot());
	//_meca->setCouleurServos(color == RobotColor::Orange ? CarteServo::Couleur::ORANGE : CarteServo::Couleur::VERT);
	logDebug("Chargement des réseaux de Pétri…");
	try {
		initDeplacementPetri(*this);
		initUtilsPetri(*this);
		initServoPetri(*this);
		initMoteurPetri(*this);
	} catch(std::exception const& e) {
		std::cout << "ERREUR lors du lancement de petri : " << e.what() << std::endl;
	}

	_nbr_points = 0;
}

StrategiePrincipal::~StrategiePrincipal() = default;

void StrategiePrincipal::setPoints(int points) {
	_nbr_points = points;
}

int StrategiePrincipal::getPoints() const {
	return _nbr_points;
}


RobotPrincipal& StrategiePrincipal::getRobot() {
	return static_cast<RobotPrincipal&>(*_robot);
}


/*******************************************************************/
/*************************** UTILITAIRES ***************************/
/*******************************************************************/

/// Retourne vrai si l'adversaire est sur le chemin (ligne droite) menant à la cible xCible, yCible
bool StrategiePrincipal::adversaireVersCible(Vector2m const& cible) {
	Angle anglePointCible = cible.angle();
	Angle angleRobot = _robot->lireCoordonnees().getAngle();
	// Angle anglePointCibleRelatifRobot = anglePointCible - angleRobot;

	auto dataAdv = getRobot().getCarte<EVITEMENT>().lirePositionAdversaire();
	return dataAdv.first;

	/*if (dataAdv.first) {
	    Angle angleAdv = dataAdv.second;
	    angleAdv = angleAdv.toMod2Pi();
	}


	Angle angleAdv = getRobot().getCarte<EVITEMENT>().getAngle();

	angleAdv = angleAdv.toMod2Pi();
	anglePointCibleRelatifRobot = anglePointCibleRelatifRobot.toMod2Pi();

	if(abs(angleAdv) > 0.001_rad && angleAdv < (anglePointCibleRelatifRobot + 1_PI / 3.0) &&
	   angleAdv > (anglePointCibleRelatifRobot - 1_PI / 3.0)) {
	    return true;
	} else {
	    return false;
	}*/
}

void StrategiePrincipal::funnyAction() {
	//_meca->lancerEnginSpatial();
}

/******* Placement du robot et definition des coordonnees avant le debut du match *******/
void StrategiePrincipal::recalageZoneDepart() {
	static const Distance AVANCEE_RECALAGE_Y = 20_cm;
	Angle angle;
	ResultatAction res;

	_dep->pushVitesseLineaire(VITESSE_LINEAIRE_RECALAGE);

	// On recule jusqu'a toucher le bord du terrain (coté arbre)
	logDebug("avancerInfini");
	res = _dep->avancerInfini(SensAvance::Arriere);

	logDebug("DefinirCoordonnees");
	_dep->setRepere({RobotPrincipal::RAYON_ROBOT_RECALAGE_X, _robot->actualiserEtLireCoordonnees().getY()}, 0_PI);

	sleep(500_ms);
	Coordonnees coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("tu es en x=", coord.getX(), " y=", coord.getY(), " theta=", coord.getAngle());
	sleep(500_ms);
	// On avance un peu
	res = _dep->avancer(AVANCEE_RECALAGE_Y, SensAvance::Avant, 4_s);
	logError(res);
	// On tourne
	logDebug("Tourner");
	_dep->tournerRelatif(-0.5_PI);

	// On recule jusqu'a toucher l'autre bord du terrain
	logDebug("avancerInfini");
	_dep->avancerInfini(SensAvance::Arriere);

	// On recale Y
	logDebug("DefinirCoordonnees");
	_dep->setRepere({_robot->actualiserEtLireCoordonnees().getX(), 1.22_m - RobotPrincipal::RAYON_ROBOT_RECALAGE}, -0.5_PI);

	sleep(200_ms);

	// Retour à la vitesse normale
	logDebug("ReglerParametre : vitesse");
	_dep->popVitesseLineaire();

	coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("tu es en x=", coord.getX(), " y=", coord.getY(), " theta=", coord.getAngle());
}