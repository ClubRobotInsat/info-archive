
#include "IATest.h"

void getChar();

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot...Veuillez patienter...");
	IATestDeplacement strategie({argv, argv + argc});

	logDebug6("Démarre la stratégie");
	strategie.demarrer(MATCH_DURATION * 100);

	return 0;
}

void getChar() {
	logInfo("Appuyez sur une touche");
	getchar();
}

void IATestDeplacement::testCarre() {
	ResultatAction res;
	Coordonnees coord;
	std::vector<Vector2m> positions = {Vector2m{800_mm, 1500_mm},
	                                   Vector2m{1500_mm, 1500_mm},
	                                   Vector2m{1500_mm, 900_mm},
	                                   Vector2m{800_mm, 900_mm}};
	std::vector<Angle> angles = {0_PI, 0.5_PI, 1_PI, -0.5_PI};

	unsigned int i = 0;
	for(i = 0; i < positions.size(); i++) {
		logDebug("On va à la position ", positions[i]);
		res = _dep->allerA(positions[i], SensAvance::Avant);
		if(res != ResultatAction::REUSSI) {
			coord = _robot->lireCoordonnees();
			logWarn(res, "! On est à la position x=", coord.getX(), " y=", coord.getY());
		}

		//		logDebug("On se tourne vers l'angle ", angles[i]);
		//		res = _dep->tournerAbsolu(angles[i]);
		//		if (res != ResultatAction::REUSSI) {
		//			coord = _robot->lireCoordonnees();
		//			logWarn(res, "! On est à la position theta=", coord.theta);
		//		}
	}
}

void IATestDeplacement::testLigneDroite() {
	ResultatAction res;
	Coordonnees coord;
	// Vector2m DEPART = Vector2m{250_cm, 1000_cm};
	// Vector2m ARRIVEE = Vector2m{2600_cm, 1000_cm};
	logDebug3("On avance de 1m");
	res = _dep->avancer(1000_mm, SensAvance::Avant);
	logDebug3("Avancée terminée :", res);
	sleep(3_s);
	logDebug3("On recule de 1m");
	res = _dep->avancer(1000_mm, SensAvance::Arriere);
	logDebug3("Recul terminé :", res);
	sleep(3_s);

	//	res = _dep->allerA(DEPART, SensAvance::Avant);
	//	if (res != ResultatAction::REUSSI) {
	//		coord = _robot->lireCoordonnees();
	//		logWarn(res, "! On est à la position x=", coord.x, " y=", coord.y);
	//	}
	//
	//	getChar();
	//	res = _dep->allerA(ARRIVEE, SensAvance::Avant);
	//	if (res != ResultatAction::REUSSI) {
	//		coord = _robot->lireCoordonnees();
	//		logWarn(res, "! On est à la position x=", coord.x, " y=", coord.y);
	//	}
}

void IATestDeplacement::testAnglesAbsolus() {
	ResultatAction res;
	Coordonnees coord;

	logDebug("Test des angles dans le sens trigo");
	for(int i = 8; i >= 0; i--) {
		res = _dep->tournerAbsoluOriente(0_PI + i * 0.25_PI, SensRotation::Trigo);
		if(res != ResultatAction::REUSSI) {
			coord = _robot->lireCoordonnees();
			logWarn(res, "! On est à l'angle theta=", coord.getAngle());
		}
		getChar();
	}

	logDebug("Test des angles dans le sens horaire");
	for(int i = 0; i <= 8; i++) {
		res = _dep->tournerAbsoluOriente(0_PI + i * 0.25_PI, SensRotation::Horaire);
		if(res != ResultatAction::REUSSI) {
			coord = _robot->lireCoordonnees();
			logWarn(res, "! On est à l'angle theta=", coord.getAngle());
		}
		getChar();
	}

	//		_dep->tournerAbsoluOriente(0.01_PI, SensRotation::Trigo);
	//	    getChar();
	//	    _dep->tournerAbsoluOriente(1.99_PI, SensRotation::Trigo);
	//	    getChar();
	//	    _dep->tournerAbsoluOriente(0.25_PI, SensRotation::Horaire);
	//	    getChar();
	//	    _dep->tournerAbsoluOriente(-3_PI/2, SensRotation::Trigo);
	//		getChar();
	//		_dep->tournerAbsoluOriente(0_PI, SensRotation::Horaire);
	//		getChar();
	//		_dep->tournerAbsoluOriente(-1_PI, SensRotation::Trigo);
	//		getChar();
}

void IATestDeplacement::testAnglesRelatifs() {
	_dep->tournerRelatif(0.25_PI);
	_dep->tournerRelatif(-0.50_PI);
	_dep->tournerRelatif(0.25_PI);
	_dep->tournerRelatif(3_PI);
	_dep->tournerRelatif(-3_PI);
}

void IATestDeplacement::testPointerVers() {
	_dep->pointerVers(0_mm, 0_mm, SensRotation::Horaire);
	getChar();
	_dep->pointerVers(0_mm, 1500_mm, SensRotation::Trigo);
	getChar();
	_dep->pointerVers(3000_mm, 0_mm, SensRotation::Horaire);
	getChar();
	_dep->pointerVers(3000_mm, 1500_mm, SensRotation::Trigo);
	getChar();
}

void IATestDeplacement::testAllerA() {
	//	_dep->allerA({1000_mm, 800_mm});
	//	//getChar();
	//	_dep->allerA({1000_mm, 1200_mm});
	//	//getChar();
	//	_dep->allerA({300_mm, 1200_mm});
	//	//getChar();
	//	_dep->allerA({300_mm, 800_mm});
	//	//getChar();
	_dep->setRepere(Vector2m(28_cm, 99_cm), 0_deg);
	// Tests aux limites
	auto coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("Coordonnées : ", coord);
	// Vector2m(28_m*gridSize, 50_m*gridSize)
	_dep->avancer(25_cm, SensAvance::Avant);
	getchar();
	coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("Coordonnées : ", coord);
	_dep->allerA(Vector2m(28_cm, 50_cm), 30_s);
	getchar();
	coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("Coordonnées : ", coord);
	_dep->allerA(Vector2m(150_cm, 100_cm), 30_s);
	logDebug3("Deuxième aller A fini MAGGLE ALLEZ LA PUTAIN !");
	_dep->allerA(Vector2m(151_cm, 99_cm), 30_s);
	getchar();
	coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("Coordonnées : ", coord);
	_dep->allerA(Vector2m(100_cm, 100_cm), 30_s);
	getchar();
	coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("Coordonnées : ", coord);
	_dep->allerA(Vector2m(150_cm, 50_cm), 30_s);
	getchar();
	coord = _robot->actualiserEtLireCoordonnees();
	logDebug2("Coordonnées : ", coord);
	_dep->allerA(Vector2m(50_cm, 90_cm), 30_s);
	getchar();
}

void IATestDeplacement::testTrajectoireComplexe() {
	// TODO
}

void IATestDeplacement::testTournerAbsoluDeterminationSens() {
	_dep->tournerAbsolu(0_PI);
	getchar();
	_dep->tournerAbsolu(0.25_PI);
	getchar();
	_dep->tournerAbsolu(-0.25_PI);
	getchar();
	_dep->tournerAbsolu(0.75_PI);
	getchar();
	_dep->tournerAbsolu(-0.75_PI);
}

void IATestDeplacement::executer() {
	// Position initiale du robot :
	// testAnglesAbsolus();
	//	testAllerA();

	//	while(true) {
	//		logDebug7(_robot->getPositionAngulaireAdversaire());
	//		sleep(200_ms);
	//	}

	//    _dep->setDebugState(true);
	//    _dep->avancer(60_cm, SensAvance::Arriere);

	//    _deplacement.allerADecompose(1000_mm, 1000_mm, SensAvance::Avant);
	//    return;

	std::cout << "===== Déplacements simples =====\n" << std::endl;
	std::cout << "\navancerDe(10_cm) : " << std::flush;
	getchar();
	std::cout << toString(_dep->avancer(10_cm, SensAvance::Avant)) << std::endl;

	std::cout << "\nreculerDe(10_cm) : " << std::flush;
	getchar();
	std::cout << toString(_dep->avancer(10_cm, SensAvance::Arriere)) << std::endl;

	std::cout << "\ntournerAbsolu(45_deg) : " << std::flush;
	getchar();
	std::cout << toString(_dep->tournerAbsolu(45_deg)) << std::endl;

	std::cout << "\ntournerAbsolu(-45_deg) : " << std::flush;
	getchar();
	std::cout << toString(_dep->tournerAbsolu(-45_deg)) << std::endl;

	std::cout << "\ntournerRelatif(45_deg) : " << std::flush;
	getchar();
	std::cout << toString(_dep->tournerRelatif(45_deg)) << std::endl;

	std::cout << "\ntournerRelatif(-45_deg) : " << std::flush;
	getchar();
	std::cout << toString(_dep->tournerRelatif(-45_deg)) << std::endl;

	return;

	/*dep->avancer(100_cm, SensAvance::Avant);
	_dep->tournerRelatif(-0.5_PI);
	getchar();
	_dep->avancer(60_cm, SensAvance::Avant);
	_dep->tournerRelatif(-0.5_PI);
	getchar();
	_dep->avancer(100_cm, SensAvance::Avant);



	_dep->tournerAbsolu(0_PI);
	_dep->pushVitesseLineaire(100_cm_s);

	CarteInfo<DEPLACEMENT>::type &_deplacement = this->getRobot().getCarte<DEPLACEMENT>();
	_deplacement.passerPar(1000_mm, 1000_mm, _dep->getVitesseLineaire(), SensAvance::Avant);
	while (!_deplacement.verifierPrecisionAtteinte()) {
	    sleep(10_ms);
	}
	_deplacement.allerADecompose(1800_mm, 1000_mm, SensAvance::Avant);*/
	//	_robot->setAngleDetectionAdv(0.25_PI);
	//    while(1) {
	//        _dep->allerA(Vector2m{2000_mm, 1000_mm}, SensAvance::Avant);
	//        _dep->allerA(Vector2m{800_mm, 1000_mm}, SensAvance::Avant);
	//    }

	//    testTournerAbsoluDeterminationSens();
	//    _robot->setAngleDetectionAdv(1_PI);
	//    while(1) {
	//        //testCarre();
	//        logDebug("AVANT: ", _robot->adversairePresent(SensAvance::Avant));
	//        logDebug("ARRIERE: ", _robot->adversairePresent(SensAvance::Arriere));
	//        sleep(500_ms);
	//
	//    }

	/*CarteInfo<DEPLACEMENT>::type& _deplacement = this->getRobot().getCarte<DEPLACEMENT>();
	bool reversed = false;
	while(1) {
	    //        logDebug(_dep->avancer(100_cm, SensAvance::Avant));
	    //        logDebug(_dep->tournerRelatif(reversed ? -2.0_PI : 2.0_PI));
	    logDebug("Entrez le num du param [espace] sa valeur");
	    int param;
	    float val;
	    std::cin >> param >> val;
	    if(param > -1) {
	        for(int i = 0; i < 3; i++) {
	            auto p = param + i * 3;
	            logDebug("Param: ", p, " value= ", val);
	            _deplacement.reglerParametre(p, val);
	        }
	    }

	    testCarre();

	    reversed = !reversed;
	}*/


	//    _dep->tournerAbsoluOriente(-1_PI, SensRotation::Horaire);
	//    getChar();
	//    _dep->tournerAbsoluOriente(0_PI, SensRotation::Trigo);


	//    logDebug3("Equals ? ", equals(0.37_rad, 0.38_rad, 0.2_rad));
	//    logDebug3("Equals ? ", equals(355_deg, 0_deg, 10_deg));
	//    logDebug3("Equals ? ", equals(355_deg, 0_deg, 2_deg));
	//    while(true){
	//    	logDebug3("Angle actuel : ", _robot->lireCoordonnees().getAngle());
	//    }
	//_env->actualiserEnvironnement();

	/*logInfo(_dep->avancer(50_cm, SensAvance::Avant));
	    // TODO: remplacer ce chemin par ce qu'on veut cf au dessus
	logInfo(_dep->avancer(50_cm, SensAvance::Avant));
	getchar();
	logInfo(_dep->tournerRelatif(0.25_PI));
	getchar();
	logInfo(_dep->avancer(30_cm, SensAvance::Avant));
	getchar();
	logInfo(_dep->tournerRelatif(-0.25_PI));
	getchar();
	logInfo(_dep->avancer(30_cm, SensAvance::Avant));
	getchar();
	logInfo(_dep->tournerRelatif(0.5_PI));
	getchar();
	logInfo("Devant l'escalier : ", _dep->avancer(40_cm, SensAvance::Avant));
	getchar();
	logInfo(_dep->allerA({50_cm, -40_cm}, SensAvance::Avant));
	getchar();
	logInfo(_dep->tournerAbsolu(-0.5_PI));
	getchar();
	logInfo(_dep->tournerAbsolu(0_rad));*/

	// logInfo(_dep->avancer(100_cm, SensAvance::Avant));
	// logInfo(_dep->tournerRelatif(1_PI));

	//	_meca->testerPetri();
	//	return;
	//
	//	logInfo(_dep->avancer(50_cm, SensAvance::Avant));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Avant));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(-0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Avant));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Arriere));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Arriere));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(-0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Arriere));
	//	logInfo(_dep->avancer(50_cm, SensAvance::Avant));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Avant));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(-0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Avant));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Arriere));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Arriere));
	////	getchar();
	//	logInfo(_dep->tournerRelatif(-0.5_PI));
	////	getchar();
	//	logInfo(_dep->avancer(20_cm, SensAvance::Arriere));

	//_dep->avancer(100_cm, SensAvance::Avant);
	//_dep->tournerRelatif(-90.0_deg);

	// logDebug("Test");
}
