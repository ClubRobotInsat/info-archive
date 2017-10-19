#include "IATest.h"
#include "Parsing.h"

using namespace ConstantesPrincipal;

void getChar();
float lireParametre(CarteInfo<DEPLACEMENT>::typeCarte& dep, uint8_t p);

ia_parsing::TestMeca objet;
int main(int argc, char* argv[]) {
	objet = ia_parsing::parseTestMeca(argc, argv);

	Log::open(argc, argv, false);

	logDebug6("Initialisation du robot…Veuillez patienter…");
	IATestMeca strategie({argv, argv + argc});

	logDebug6("Démarre la stratégie");
	strategie.demarrer(MATCH_DURATION * 100);

	return 0;
}

void getChar() {
	logInfo("Appuyez sur une touche");
	getchar();
}

float lireParametre(CarteInfo<DEPLACEMENT>::typeCarte& dep, uint8_t p) {
	dep.demanderParametre(p);

	while(!dep.parametreActualise())
		sleep(100_ms);

	auto val = dep.lireParametre();
	logDebug0("[Check] Param: ", p, " value= ", val);

	return val;
}

void IATestMeca::executer() {
	_robot->actualiserEtLireCoordonnees();

	// On peut régler la position angulaire de tous les servos et de l'ascenseur en fonction des arguments
	switch(objet) {
		case ia_parsing::TestMeca::ASCENSEUR:
			logDebug2("Lancement du test de l'ASCENSEUR");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->getAscenseur().allerAngleBloquant(value),
				          ", angle : ",
				          _meca->getAscenseur().getAngleBloquant().toDeg());
			}
			break;
		case ia_parsing::TestMeca::LACET:
			logDebug2("Lancement du test du LACET");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->orienterPinceDe(value), ", angle : ", _meca->getPositionServo(2, enumToInt(Servo::LACET)));
			}
			break;
		case ia_parsing::TestMeca::TANGAGE:
			logDebug2("Lancement du test du TANGAGE");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->pencherPindeDe(value), ", angle : ", _meca->getPositionServo(2, enumToInt(Servo::TANGAGE)));
			}
			break;
		case ia_parsing::TestMeca::FUSEE:
			logDebug2("Lancement du test de la FUSEE");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->bougerFuseeDe(value), ", angle : ", _meca->getPositionServo(2, enumToInt(Servo::FUSEE)));
			}
			break;
		case ia_parsing::TestMeca::GAUCHE:
			logDebug2("Lancement du test de la fesse GAUCHE");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->ouvrirFesseGaucheDe(value), ", angle : ", _meca->getPositionServo(2, enumToInt(Servo::GAUCHE)));
			}
			break;
		case ia_parsing::TestMeca::DROITE:
			logDebug2("Lancement du test de la fesse DROITE");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->ouvrirFesseDroiteDe(value), ", angle : ", _meca->getPositionServo(2, enumToInt(Servo::DROITE)));
			}
			break;
		case ia_parsing::TestMeca::PINCE:
			logDebug2("Lancement du test de la PINCE");
			for(int i = 1; i < 1000; i++) {
				int raw;
				std::cout << "Entrez une valeur : ";
				std::cin >> raw;
				Angle value = Angle::makeFromDeg(raw);

				logDebug1(_meca->ouvrirPinceDe(value), ", angle : ", _meca->getPositionServo(2, enumToInt(Servo::PINCE)));
			}
			break;
		case ia_parsing::TestMeca::STOCKER:
			logDebug2("Lancement du test STOCKER");
			for(int i = 1; i <= 3; ++i) {
				getChar();
				logDebug1(_meca->stockerCylindre(), " (cylindre n°", i, ")");
			}
			break;
		case ia_parsing::TestMeca::DESTOCKER:
			logDebug2("Lancement du test DESTOCKER");
			_meca->setTousCylindresStockes();
			for(int i = 3; i >= 1; --i) {
				getChar();
				logDebug1(_meca->destockerCylindre(), " (cylindre n°", i, ")");
			}
			break;
		case ia_parsing::TestMeca::SEQUENCE:
			logDebug2("Lancement du test SEQUENCE");
			for(int i = 1; i < 1000; i++) {
				std::string entry;
				std::cout << "Quelle séquence à tester? <stocker|S,destocker|D> : ";
				std::cin >> entry;
				_meca->orienterPinceAttraper();
				_meca->ouvrirPince();
				_meca->ascenseurCylindreSol();
				sleep(1_s);

				if(entry == "stocker" || entry == "S" || entry == "s")
					logDebug1("stockage : ", _meca->stockerCylindre());
				else if(entry == "destocker" || entry == "D" || entry == "d")
					logDebug1("destockage : ", _meca->destockerCylindre());
				else if(entry == "exit" || entry == "quit" || entry == "e" || entry == "q")
					exit(0);
				else
					logWarn("Usage : <stocker|S,destocker|D>");
				logDebug1("Il y a ", _meca->getNbrCylindres(), " cylindres stockes");
			}
			break;
		default:;
	}


	// _env->actualiserEnvironnement();

	//	_meca->testerPetri();

	//	while(1) {
	//        testHauteursAscenseur();
	//	}
	/*
	    CarteInfo<DEPLACEMENT>::typeCarte& _deplacement = this->getRobot().getCarte<DEPLACEMENT>();
	    bool reversed = false;
	    while(1) {
	        //        logDebug(_dep->avancer(100_cm, SensAvance::Avant));
	        //        logDebug(_dep->tournerRelatif(reversed ? -1.0_PI : 1.0_PI));
	        logDebug(_dep->tournerRelatif(2_PI));
	        getchar();
	        getChar();
	        logDebug(_dep->tournerRelatif(-2_PI));

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

	        reversed = !reversed;
	    }
	*/
	//    logDebug("Lancement du test de la carte IO : appuyez sur entrée");
	//    getchar();
	//    CarteInfo<IO>::type &_io = getRobot().getCarte<IO>();
	//    while (1) {
	//        for (int i=1; i<11; i++) {
	//            logDebug("[", i, "] Val = ", _io.actualiserPuisLireUnCapteur(i));
	//        }
	//        logDebug("Num de l'entrée: ");
	//        uint8_t nb;
	//        std::cin >> nb;
	//        logDebug("Val = ", _io.actualiserPuisLireUnCapteur(nb));
	//        getchar();
	//    }

	//
	//    while (1) {
	//        logDebug("Adversaire devant? ", getRobot().adversairePresent(SensAvance::Avant));
	//        logDebug("Adversaire derrière? ", getRobot().adversairePresent(SensAvance::Arriere));
	//        getchar();
	//    }

	//    _meca->bougerCoude(CoteAscenseur::DROIT, PositionCoude::GOBELET);
	//    _meca->bougerCoude(CoteAscenseur::GAUCHE, PositionCoude::GOBELET);
	//
	//    auto recalageAscenseur = [this](CoteAscenseur cote) {
	//        auto &asc = _meca->getAscenseur(cote);
	//        while(1) {
	//            logDebug("Sol ", enumToInt(cote), " : ", asc.allerAuSol());
	//            getchar();
	//            logDebug("Stockage ", enumToInt(cote), " : ", asc.allerStockage());
	//            getchar();
	//        }
	//    };
	//    auto t1 = std::thread(recalageAscenseur, CoteAscenseur::GAUCHE);
	//    auto t2 = std::thread(recalageAscenseur, CoteAscenseur::DROIT);
	//    t1.join();
	//    t2.join();
}
