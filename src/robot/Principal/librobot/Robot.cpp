/* RobotPrincipal2015.cpp */

#include "Robot.h"
#include "MecaManager.h"
#include <string>

using namespace ConstantesPrincipal;
using namespace IDCartesPrincipal;

const AngularSpeed RobotPrincipal::VITESSE_ASCENSEUR_DEFAUT = 50_rad_s;
const AngularSpeed RobotPrincipal::VITESSE_RECALAGE_ASCENSEUR = 50_rad_s;

const Distance RobotPrincipal::RAYON_ROBOT_RECALAGE = 150_mm;
const Distance RobotPrincipal::RAYON_ROBOT_RECALAGE_X = 260_mm;

const float RobotPrincipal::COEFF_FIT_MULT = 51.179035;
const float RobotPrincipal::COEFF_FIT_POW = -1.008083;

///////////////////////////////////////////// Constructeur //////////////////////////////////////////////////////
RobotPrincipal::RobotPrincipal(std::vector<std::string> const& args, Speed vitesseDeplacement, AngularSpeed vitesseAngulaire)
        : RobotAnnee(std::make_unique<ConstantesCommunes>(), std::make_unique<ConstantesRobotPrincipal>(), args, vitesseDeplacement, vitesseAngulaire) {
	// this->setDebugCAN(true);
	assignerCartes();
	this->initialisation();
}

////////////////////////////////////////// Initialise le robot ////////////////////////////////////////////////////
void RobotPrincipal::initialisation() {
	logDebug2("Début de la fonction : initialisation");

	setAllPinged(pinguerCartes());

	//	logInfo("Cartes pinguées");
	this->getCarte<DEPLACEMENT>().activerEnvoiAuto(true);


	// Recuperation de l'etat actuel des contacteurs
	// Tout changement sera actualise automatiquement par la carte
	logDebug2("Actualisation des contacteurs");
	logDebug2("Contacteurs actualises");

	this->getCarte<EVITEMENT>().activerEnvoiMesureAdvAuto();
}

Vector2m RobotPrincipal::getPositionTourelle() const {
	return POSITION_TOURELLE;
}

StrategiePrincipal& RobotPrincipal::getStrategie() {
	return static_cast<StrategiePrincipal&>(*_strategie);
}

//////////////////////////////////// Desactive le robot en fin de match /////////////////////////////////////////
void RobotPrincipal::desactivation() {
	logInfo("desactivation du robot");

	this->getCarte<DEPLACEMENT>().activerEnvoiAuto(false);
	this->getCarte<DEPLACEMENT>().arretUrgence();

	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::DROITE, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::GAUCHE, false);

	this->getCarte<EVITEMENT>().desactiverEnvoiMesureAdvAuto();

	getStrategie().getMecaManager().couperMeca();
}

///////////////// Fonction privée, appelée par le constructeur, qui assigne les cartes à leurs IDs //////////////////
void RobotPrincipal::assignerCartes() {
	// this->assignerCarteHelper<CAN_USB>();
	this->assignerCarteHelper<DEPLACEMENT>();
	this->assignerCarteHelper<IO>();
	this->assignerCarteHelper<EVITEMENT>();
	// this->assignerCarteHelper<ASCENSEUR>();
	this->assignerCarteHelper<SERVOS>();
	this->assignerCarteHelper<MOTEURS>();
	// TODO : on ajoute toutes les cartes du robot de l'année

	_cartesInitialisees.notify_all();
	_cartesInitNotified = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
