#include "Primary.h"
#include <Constants.h>
#include <string>

///////////////////////////////////////////// Constructeur //////////////////////////////////////////////////////
PrimaryRobot::PrimaryRobot(std::shared_ptr<Commun::ModuleManager> module_manager, std::vector<std::string> const& args)
        : Robot(module_manager, args) {}

PrimaryRobot::PrimaryRobot(std::vector<std::string> const& args)
        : PrimaryRobot(std::make_shared<Commun::ModuleManager>(), args) {}

//////////////////////////////////// Desactive le robot en fin de match /////////////////////////////////////////
void PrimaryRobot::deactivation() {
	logInfo("desactivation du robot");

	if(_module_manager->has_module<Commun::Moving2019>()) {
		// TODO : arrêt d'urgence
	}

	/*this->getCarte<DEPLACEMENT>().activerEnvoiAuto(false);
	this->getCarte<DEPLACEMENT>().arretUrgence();

	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::DROITE, false);
	this->getCarte<DEPLACEMENT>().activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::GAUCHE, false);*/

	// this->getCarte<EVITEMENT>().desactiverEnvoiMesureAdvAuto();

	// getStrategie().getMecaManager().couperMeca();
}

void PrimaryRobot::assign_modules() {
	for(auto module : GLOBAL_CONSTANTS.primary->get_modules()) {
		if(module.first == "moving") {
			_module_manager->add_module<Commun::Moving2019>(module.second);
		} else if(module.first == "servos") {
			// TODO : voir comment récupérer les servos à ajouter (`robot.ini` ou fichier .JSON ?)
			_module_manager->add_module<Commun::Servos2019>(module.second);
		} else if(module.first == "motors") {
			_module_manager->add_module<Commun::Motors2019>(module.second);
		} else if(module.first == "io") {
			_module_manager->add_module<Commun::IO2019>(module.second);
		} else if(module.first == "avoidance") {
			auto& avoidance = _module_manager->add_module<Commun::Avoidance2019>(module.second);
			avoidance.set_position_turret(GLOBAL_CONSTANTS.primary->get_turret_position());
		} else {
			throw std::runtime_error("The module named '" + module.first + "' (ID: " + std::to_string(module.second) + ") isn't known.");
		}
	}
}
