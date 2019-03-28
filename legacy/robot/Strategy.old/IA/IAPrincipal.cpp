#include "IAPrincipal.h"
#include "../../Principal/petri/Deplacement.h"
#include "../../Principal/petri/Servo.h"
#include "../../Principal/petri/src/IA2018.h"
#include "Parsing.h"
#include <AsciiArt.hpp>
#include <petrilab/Cpp/PetriLab.h>

#define CHECK_LIST 0
int main(int argc, char* argv[]) {
	AsciiArt art(std::cout);
	art.print_chocobot();

	auto parsing = ia_parsing::parsing_function(argc, argv);

	std::cout << parsing << std::endl;

	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IAPrincipal strategie({argv, argv + argc}, parsing.color, parsing.debug_mode, parsing.magic_strategy);

	logDebug6("Démarre la stratégie");
	if(!parsing.debug_mode) {
		strategie.demarrer(MATCH_DURATION);
	} else {
		strategie.demarrer(MATCH_DURATION * 100);
	}
	logDebug9("fin initialisation");
	return 0;
}

IAPrincipal::IAPrincipal(std::vector<std::string> const& args, Constantes::RobotColor color, bool debugMode, bool magicStrategy)
        : StrategiePrincipal(args, color), _debugMode(debugMode), _magicStrategy(magicStrategy) {
	// main --> IAPrincipal() --> initialisation --> attendre tirette -> démarrer
	this->initialisation();
#ifdef HOMOLOGATION
	_petri = Petri::Generated::Homologation::createLib(".");
#else
	_petri = Petri::Generated::IA2018::createLib(".");
#endif
	_petri->load();
}

void IAPrincipal::initialisation() {
#if CHECK_LIST
	logCyan("-----------------------");
	logCyan("Carte pinguées ?");
	getchar();
	logCyan("La tourelle tourne-t-elle ? ");
	getchar();
	logCyan("Cadre recalages placés ?");
	getchar();
	logCyan("Couleur robot : ", this->getCouleurEquipe());
	getchar();
	logCyan("-----------------------");
#endif
	//_robot->getCAN().setDebug(true);
	// Recallage du robot (manuel)
	// Test pour voir si on a tout ping
	logDebug9("Initialisation");

	if(!_debugMode) {
		// se mettre en position de départ : sortirBrasGaucheMax(); ...

		//		sortirParasol();
		//		sleep(50_ms);
		//		rentrerParasol();
	}


	logCyan("Couleur robot : ", this->getCouleurEquipe());
	logDebug("Recalage du robot");

	const repere::Repere& repere_match = (getCouleurEquipe() == RobotColor::Orange ? REFERENCE_ORANGE : REFERENCE_GREEN);
	repere::Coordonnees robot_coords(START_ROBOT_POSITION, START_ROBOT_ANGLE, repere_match);

	getDeplacement().setRepere(robot_coords);


	logDebug("StrategyGenerator");
	// Action definitions
	auto action_bee = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(12_s, 50, StrategyGenerator::Element(StrategyGenerator::ElementType::BEE, coords), {});
	};

	auto action_switch = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(3_s, 25, StrategyGenerator::Element(StrategyGenerator::ElementType::SWITCH, coords), {});
	};

	auto action_get_cube = [&repere_match](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(35_s,
		                                 0,
		                                 StrategyGenerator::Element(StrategyGenerator::ElementType::GET_CUBE, coords),
		                                 {StrategyGenerator::Element(StrategyGenerator::ElementType::PUT_CUBE,
		                                                             Coordonnees({20_cm, 1.45_m}, 135_deg, repere_match))});
	};

	auto action_put_cube = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		// Un immeuble de X étages rapporte X^2 points (on mise sur le maximum)
		return StrategyGenerator::Action(5_s, 25, StrategyGenerator::Element(StrategyGenerator::ElementType::PUT_CUBE, coords), {});
	};

	auto action_get_sphere = [&repere_match](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(10_s,
		                                 0,
		                                 StrategyGenerator::Element(StrategyGenerator::ElementType::GET_SPHERE, coords),
		                                 {StrategyGenerator::Element(StrategyGenerator::ElementType::PUT_SPHERE,
		                                                             Coordonnees({55_cm, 60_cm}, -90_deg, repere_match))});
	};

	auto action_put_sphere = [](repere::Coordonnees coords) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(10_s, 30, StrategyGenerator::Element(StrategyGenerator::ElementType::PUT_SPHERE, coords), {});
	};

	// Element definitions
	std::function<bool()> always_possible = []() -> bool { return true; };
	std::function<bool()> get_cube_is_possible = [this]() -> bool { return _meca->nbrPlaceAscenseur() == 0; };
	std::function<bool()> get_sphere_is_possible = [this]() -> bool {
		return _meca->turbineDLibre() && _meca->turbineGLibre();
	};

	_strategy.associate_element(StrategyGenerator::ElementType::BEE, action_bee, always_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::SWITCH, action_switch, always_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::GET_CUBE, action_get_cube, get_cube_is_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::PUT_CUBE, action_put_cube, always_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::GET_SPHERE, action_get_sphere, get_sphere_is_possible);
	_strategy.associate_element(StrategyGenerator::ElementType::PUT_SPHERE, action_put_sphere, always_possible);

	// Table generation
	StrategyGenerator::Table table;
	table.set_robot_coords(robot_coords);

	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::BEE, Coordonnees({61_cm, 46_cm}, -90_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::SWITCH, Coordonnees({1.13_m, 1.6_m}, 90_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::GET_CUBE, Coordonnees({60_cm, 1.46_m}, 0_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::GET_CUBE, Coordonnees({55_cm, 81_cm}, 180_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::GET_CUBE, Coordonnees({1.1_m, 75_cm}, -90_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::GET_SPHERE, Coordonnees({61_cm, 1.36_m}, -135_deg, repere_match))));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::GET_SPHERE, Coordonnees({28_cm, 30_cm}, -45_deg, repere_match))));

	_strategy.initialize(table);

/// Recalage
/*_dep->avancer(3_cm, SensAvance::Avant, 500_ms);
sleep(1_s);
_dep->avancer(3_cm, SensAvance::Arriere, 500_ms);*/
#if CHECK_LIST
	logCyan("-----------------------");
	logCyan("Cadre décalage enlevé ?");
	getchar();
	logCyan("-----------------------");
#endif

	// Attente pour le départ
	logDebug0("Tirette");

	if(!_debugMode) {
		if(_robot->getModeConnexion() == Commun::ModeConnexion::LOCAL) {
			// Attente en mode simu
			// TODO faire une tirette en mode simu ?
			std::cout << "Mode local : Appuyez sur une touche." << std::endl;
			getchar();
		} else {
			attendreTirette();
		}
	}
}

void IAPrincipal::executer() {
	_dep->setDebugState(true);

	if(!_debugMode) {
		_points_printer = std::thread([this]() {
			AsciiArt art(std::cout);

			while(true) {
				art.print_chocobot();
				sleep(2_s);
				art.print_number(_nbr_points);
				sleep(2_s);
				art.print_string("INSA", AsciiArt::COLOR_RED, AsciiArt::COLOR_RED);
				sleep(2_s);
			}
		});
	}

	logDebug5("Debut exécuter");
#ifdef HOMOLOGATION
	_robot->setAngleDetectionAdv(0.5_PI);
#else
	_robot->setAngleDetectionAdv(0.4_PI);
#endif

	if(_magicStrategy) {
		_strategy.run(*_dep, *_petri, 500_ms);
		_nbr_points = _strategy.get_total_points();
	} else {
		lancerPetri(_debugMode);
	}

	/*std::thread([=]() {
	    logDebug(_robot->getPositionAdversaire());
	});*/
}

void IAPrincipal::funnyAction() {
	//_meca->lancerEnginSpatial();
}

ResultatAction IAPrincipal::lancerPetri(bool debug) {
	if(debug) {
		Petri::DebugServer debugSession(*_petri);
		debugSession.start();
		debugSession.join();
	} else {
		auto ia = _petri->createPetriNet();
		ia->run();

		// Garde fou pour les 90s de match
		while(getTempsRestant() > 0_s) {
			sleep(50_ms);
		}

		ia->stop();
	}

	return ResultatAction::REUSSI;
}
