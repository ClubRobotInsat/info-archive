/* RobotPrincipal2015.cpp */

#include "Robot.h"
//#include "MecaManager.h"
#include <string>

/*using namespace ConstantesPrincipal;
using namespace IDCartesPrincipal;

const AngularSpeed RobotPrincipal::VITESSE_ASCENSEUR_DEFAUT = 50_rad_s;
const AngularSpeed RobotPrincipal::VITESSE_RECALAGE_ASCENSEUR = 50_rad_s;

const Distance RobotPrincipal::RAYON_ROBOT_RECALAGE = 150_mm;
const Distance RobotPrincipal::RAYON_ROBOT_RECALAGE_X = 260_mm;

const float RobotPrincipal::COEFF_FIT_MULT = 51.179035;
const float RobotPrincipal::COEFF_FIT_POW = -1.008083;*/

///////////////////////////////////////////// Constructeur //////////////////////////////////////////////////////
RobotPrincipal::RobotPrincipal(std::shared_ptr<Commun::ModuleManager> module_manager, std::vector<std::string> const& args)
        : Robot(module_manager, std::make_unique<ConstantesCommunes>(), std::make_unique<ConstantesRobotPrincipal>(), args) {}

RobotPrincipal::RobotPrincipal(std::vector<std::string> const& args)
        : RobotPrincipal(std::make_shared<Commun::ModuleManager>(), args) {}

/*Vector2m RobotPrincipal::getPositionTourelle() const {
    return POSITION_TOURELLE;
}*/

/*StrategiePrincipal& RobotPrincipal::getStrategie() {
    return static_cast<StrategiePrincipal&>(*_strategie);
}*/

//////////////////////////////////// Desactive le robot en fin de match /////////////////////////////////////////
void RobotPrincipal::deactivation() {
	logInfo("desactivation du robot");

	/*this->getCarte<DEPLACEMENT>().activerEnvoiAuto(false);
	this->getCarte<DEPLACEMENT>().arretUrgence();

	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::DROITE, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::GAUCHE, false);*/

	// this->getCarte<EVITEMENT>().desactiverEnvoiMesureAdvAuto();

	// getStrategie().getMecaManager().couperMeca();
}

void RobotPrincipal::assign_modules() {
	_module_manager->add_module<Commun::Moving2019>(1);

	auto& servos = _module_manager->add_module<Commun::Servos2019>(2);

	_module_manager->add_module<Commun::IO2019>(3);

	auto& avoidance = _module_manager->add_module<Commun::Avoidance2019>(4);
	avoidance.set_position_turret(ConstantesPrincipal::POSITION_TOURELLE);

	auto& motors = _module_manager->add_module<Commun::Motors2019>(5);
}
