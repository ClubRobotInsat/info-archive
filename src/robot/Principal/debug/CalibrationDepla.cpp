// CalibrationDepla.cpp

#include "CalibrationDepla.h"
#include <cstdio>
#include <cstdlib>

#define HAUTEUR_TABLE 2100
#define LARGEUR_TABLE 3002

using ParametresCarte = CarteInfo<DEPLACEMENT>::typeCarte::TypeParametre;

int main(int argc, char* argv[]) {
	logDebug6("Initialisation du robot...Veuillez patienter...");
	CalibrationDepla strategie(argc, argv);

	logDebug6("Changement de la precision angulaire de fin de mouvement...");
	strategie.getRobot().getCarte<DEPLACEMENT>().reglerParametre(ParametresCarte::MouvementActuel_PrecisionAngulaire,
	                                                             10 * M_PI / 180.0);

	logDebug6("Attente 1 sec...");
	sleep(1_s);

	// Recalage au départ
	logDebug6("Recalage du robot");
	// strategie.recalageZoneDepart();

	// Attente tirette
	logDebug6("Attente tirette pour lancer le match");
	logWarn("Attente tirette pour lancer le match DESACTIVE");
	// strategie.getRobot().attenteTirette();

	strategie.demarrer(MATCH_DURATION * 10);
}

void CalibrationDepla::attendreFinDeplacement(Duration timeout) {
	StopWatch clock;
	unsigned int comptBlocage = 0;

	while(!_deplacement.verifierDeplacementTermine()) {
		if(clock.getElapsedTime() > timeout) {
			_deplacement.arretUrgence();
			std::cout << clock.getElapsedTime() << std::endl;
			return;
		}

		if(_deplacement.verifierNouveauBlocage())
			comptBlocage++;

		if(comptBlocage > Constantes::NOMBRE_BLOCAGE_PHYSIQUE_MAX) {
			return;
		}

		sleep(20_ms);
	}
}

void CalibrationDepla::attendreBlocage() {
	while(!_deplacement.verifierNouveauBlocage()) {
		sleep(10_ms);
	}
}

void CalibrationDepla::executer() {

	//	_meca->positionnerPinceAscG(CarteInfoPOSITION_PINCE_ASC_OUVERTE);
	//	sleep(500_ms);
	//	_meca->positionnerPinceAscD(CarteInfoPOSITION_PINCE_ASC_OUVERTE);
	//	sleep(500_ms);
	//	_meca->positionnerAscDroite(CarteInfoASC_MILIEU);
	//	_meca->positionnerAscGauche(CarteInfoASC_MILIEU);
	// TODO : petit tutoriel explicatif avant.
	printf("\033[1;32m---------------------------------------------------------\n");
	printf("------------ Programme de calibration depla -------------\n");
	printf("---------------------------------------------------------\033[0m\n");
	printf("\033[1;31mCalibration PID :\033[0m\n-- Pour régler le PID du robot.\n");
	printf(
	    "\033[1;31mCalibration facteur d'echelle :\033[0m\n-- si le robot ne parcours pas les distances demandees.\n");
	printf("\033[1;31mCalibration entre axe :\033[0m\n-- si l'asserv angulaire n'est pas précis.\n\
-- Entraxe = distance entre les 2 roues codeuses.\n");
	printf("\033[1;31mCalibration diametres :\033[0m\n-- a faire dans tous les cas.\n\
--Correspond au réglage des diamètres des roues.\n");
	printf("\033[1;31mDANS TOUS LES CAS :\033[0m\n\
-- régler les valeurs initiales (dans setDefaultParams) aux valeur mesurées sur le robot\n\
=> \033[0;33mMesurer entre axe entre les roues codeuses\033[0m (+ diametre des roues si elles ont changé) \n\
-- et reporter les valeurs dans setDefaultParams\n");
	printf("Les valeur données pour entre axe, diamètre roue D et G sont à flasher dans le pic de la carte déplacement\
une fois TOUT le programme de calibration terminé, pour que ces valeurs soient utilisées dans les IA.\n \
Ce programme utilise les valeurs dans setDefaultParams() lorsqu'il démarre, pas les valeurs de la carte déplacement !\
Il est possible d'enchainer plusieurs étapes de calibration (Diametres, Facteur d'echelle etc...) sans devoir modifier\
le pic ou les valeurs dans setDefaultParams.\n");

	while(1) {
		printf("\
     0 : CALIBRATION PID\n\
	 1 : CALIBRATION DIAMETRES\n\
	 2 : CALIBRATION FACTEUR D ECHELLE\n\
	 3 : CALIBRATION ENTRE AXE\n\
	 4 : TEST DEPLACEMENT\n\
	 5 : CALIBRATION AUTO\n\
	 Votre Choix :");
		std::cin >> _choix;

		this->setDefaultParams();
		// this->displayCurrentSate();

		switch(_choix) {
			case 0:
				this->pid();
				break;
			case 1:
				this->diametres();
				break;
			case 2:
				this->facteurEchelle();
				break;
			case 3:
				this->entreAxes();
				break;
			case 4:
				this->testDeplacement();
				break;
			case 5:
				this->diametresAuto();
				this->displayCurrentSate();
				this->facteurEchelleAuto();
				this->displayCurrentSate();
				this->entreAxesAuto();
				_deplacement.allerADecompose(2.7_m, 1_m, SensAvance::Arriere);
				this->attendreFinDeplacement();
				_deplacement.tournerAbsolu(-0.5_PI, SensRotation::Trigo);
				break;
		}
		this->displayCurrentSate();
		this->saveDatas();
	}
}

void CalibrationDepla::displayCurrentSate() {
	logDebug0("Parametres mecaniques actuels");
	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueD);
	while(!_deplacement.parametreActualise()) {
		sleep(10_ms);
		logDebug0("on attend");
	}
	logDebug0("Diam D actuel = ", _deplacement.lireParametre());
	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	logDebug0("Diam G actuel = ", _deplacement.lireParametre());
	_deplacement.demanderParametre(ParametresCarte::Mecanique_EntreAxe);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	logDebug0("Entre axe actuel = ", _deplacement.lireParametre());
	sleep(500_ms);
}

void CalibrationDepla::setDefaultParams() {
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueD,
	                             0.0638808); // Secondaire : 0. | Principal : 0.0636288
	sleep(100_ms);
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueG,
	                             0.0639245); // Secondaire : 0. | Principal : 0.0639554
	sleep(100_ms);
	_deplacement.reglerParametre(ParametresCarte::Mecanique_EntreAxe,
	                             0.292404); // Secondaire : 0. | Principal : 0.292692
	sleep(100_ms);
}

void CalibrationDepla::pid() {
	logDebug0("DEBUT CALIBRATION DU PID");
	_choix = 1;
	int valeur;
	while(_choix != 0) {
		printf("\n\n\
     0 : Sortir\n\
	 1 : PidPositionLongitudinale_Kp\n\
	 2 : PidPositionLongitudinale_Ki\n\
	 3 : PidPositionLongitudinale_Kd\n\
	 4 : PidPositionLongitudinale_KpArret\n\
	 5 : PidPositionLongitudinale_KiArret\n\
     6 : PidPositionLongitudinale_KdArret\n\
	 7 : PidPositionLongitudinale_KpRoule\n\
	 8 : PidPositionLongitudinale_KiRoule\n\
	 9 : PidPositionLongitudinale_KdRoule\n\
     10 : PidPositionAngulaire_Kp\n\
     11 : PidPositionAngulaire_Ki\n\
     12 : PidPositionAngulaire_Kd\n\
     13 : PidPositionAngulaire_KpArret\n\
     14 : PidPositionAngulaire_KiArret\n\
     15 : PidPositionAngulaire_KdArret\n\
     16 : PidPositionAngulaire_KpRoule\n\
     17 : PidPositionAngulaire_KiRoule\n\
     18 : PidPositionAngulaire_KdRoule\n\
     19 : PidVitesseLongitudinale_Kp\n\
     20 : PidVitesseLongitudinale_Ki\n\
     21 : PidVitesseLongitudinale_Kd\n\
     22 : PidVitesseLongitudinale_KpArret\n\
     23 : PidVitesseLongitudinale_KiArret\n\
     24 : PidVitesseLongitudinale_KdArret\n\
     25 : PidVitesseLongitudinale_KpRoule\n\
     26 : PidVitesseLongitudinale_KiRoule\n\
     27 : PidVitesseLongitudinale_KdRoule\n\
     28 : PidVitesseAngulaire_Kp\n\
     29 : PidVitesseAngulaire_Ki\n\
     30 : PidVitesseAngulaire_Kd\n\
     31 : PidVitesseAngulaire_KpArret\n\
     32 : PidVitesseAngulaire_KiArret\n\
     33 : PidVitesseAngulaire_KdArret\n\
     34 : PidVitesseAngulaire_KpRoule\n\
     35 : PidVitesseAngulaire_KiRoule\n\
     36 : PidVitesseAngulaire_KdRoule\n\
     37 : PidPositionD_Kp\n\
     38 : PidPositionD_Ki\n\
     39 : PidPositionD_Kd\n\
     40 : PidPositionD_KpArret\n\
     41 : PidPositionD_KiArret\n\
     42 : PidPositionD_KdArret\n\
     43 : PidPositionD_KpRoule\n\
     44 : PidPositionD_KiRoule\n\
     45 : PidPositionD_KdRoule\n\
     46 : PidPositionG_Kp\n\
     47 : PidPositionG_Ki\n\
     48 : PidPositionG_Kd\n\
     49 : PidPositionG_KpArret\n\
     50 : PidPositionG_KiArret\n\
     51 : PidPositionG_KdArret\n\
     52 : PidPositionG_KpRoule\n\
     53 : PidPositionG_KiRoule\n\
     54 : PidPositionG_KdRoule\n\
     55 : PidVitesseD_Kp\n\
     56 : PidVitesseD_Ki\n\
     57 : PidVitesseD_Kd\n\
     58 : PidVitesseD_KpArret\n\
     59 : PidVitesseD_KiArret\n\
     60 : PidVitesseD_KdArret\n\
     61 : PidVitesseD_KpRoule\n\
     62 : PidVitesseD_KiRoule\n\
     63 : PidVitesseD_KdRoule\n\
     64 : PidVitesseG_Kp\n\
     65 : PidVitesseG_Ki\n\
     66 : PidVitesseG_Kd\n\
     67 : PidVitesseG_KpArret\n\
     68 : PidVitesseG_KiArret\n\
     69 : PidVitesseG_KdArret\n\
     70 : PidVitesseG_KpRoule\n\
     71 : PidVitesseG_KiRoule\n\
     72 : PidVitesseG_KdRoule\n\n\
	 Votre Choix :\n");
		std::cin >> _choix;

		switch(_choix) {
			case 0:
				break;
			case 1:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 2:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 3:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 4:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 5:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 6:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 7:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 8:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 9:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionLongitudinale_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionLongitudinale_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionLongitudinale_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 10:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 11:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 12:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 13:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 14:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 15:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 16:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 17:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 18:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionAngulaire_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionAngulaire_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionAngulaire_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 19:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 20:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 21:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 22:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 23:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 24:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 25:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 26:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 27:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseLongitudinale_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseLongitudinale_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseLongitudinale_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 28:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 29:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 30:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 31:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 32:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 33:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 34:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 35:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 36:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseAngulaire_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseAngulaire_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseAngulaire_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 37:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_Kp);
				_deplacement.arreter();
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 38:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 39:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 40:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 41:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 42:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_Kd Arret= ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 43:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_KpRoule);
				_deplacement.arreter();
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 44:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 45:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionD_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionD_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionD_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionD_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 46:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 47:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 48:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 49:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 50:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 51:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 52:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 53:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 54:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidPositionG_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidPositionG_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidPositionG_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidPositionG_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 55:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 56:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 57:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 58:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 59:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 60:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 61:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 62:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_KiRouleRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 63:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseD_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseD_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseD_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseD_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 64:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_Kp);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_Kp = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_Kp ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_Kp, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 65:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_Ki);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_Ki = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_Ki ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_Ki, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 66:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_Kd);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_Kd = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_Kd ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_Kd, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 67:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_KpArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_KpArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_KpArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_KpArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 68:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_KiArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_KiArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_KiArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_KiArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 69:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_KdArret);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_KdArret = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_KdArret ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_KdArret, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 70:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_KpRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_KpRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_KpRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_KpRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 71:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_KiRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_KiRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_KiRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_KiRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			case 72:
				_deplacement.demanderParametre(CarteDeplacement::TypeParametre::PidVitesseG_KdRoule);
				while(!_deplacement.parametreActualise())
					sleep(10_ms);
				logDebug0("PidVitesseG_KdRoule = ", _deplacement.lireParametre());
				printf("Nouvelle valeur du PidVitesseG_KdRoule ?\n");
				std::cin >> valeur;
				_deplacement.reglerParametre(CarteDeplacement::TypeParametre::PidVitesseG_KdRoule, valeur);
				desactiverPuisActiverAsserv();
				break;
			default:
				break;
		}
	}
}

void CalibrationDepla::desactiverPuisActiverAsserv() {
	logDebug1("Verifier que la nouvelle valeur est bonne");
	logDebug1("Appuyer sur [Entrer] pour désactiver l'asservissement et recaler le robot, ailleurs sinon");
	int c = getchar();
	if(c == '\n') {
		_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
		_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
		logDebug3("Asservissement OFF");

		logDebug1("Recaler le robot, puis appuyer sur [Entrer] pour activer l'asservissement");
		getchar();
		_dep.get()->setRepere(Vector2m(1_m, 1_m),
		                      0_deg); // le robot oublie qu'on l'a fait bouger pdt que l'asserv était désactivée
		_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);
		_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, true);
	}
	printf("Changements enregistrés\n");
}

void CalibrationDepla::diametres() {
	logDebug0("DEBUT CALIBRATION RAPPORT DES DIAMETRES");
	// logDebug3("Changement vitesse max");
	_deplacement.reglerParametre(ParametresCarte::Longitudinal_VitesseMax, (float)(600) / 1000.0);

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	logDebug3("Asservissement OFF");

	logDebug0("RECALER ROBOT.....(puis ENTREE)");
	getchar(); // PAUSE ____________________________________________

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);

	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distD_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue D : ", _distD_Avant);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distG_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue G : ", _distG_Avant);
	logDebug0("Distance Roue G : ", _distG_Avant);

	logDebug0("FAIRE PARCOURIR ROBOT ET LE RECALER AVEC LE MEME ANGLE....(puis ENTREE)");
	getchar(); // PAUSE ____________________________________________

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distD_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue D apres : ", _distD_Apres);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distG_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue G apres : ", _distG_Apres);

	_rapport_D_sur_G = fabs(_distD_Apres - _distD_Avant) / fabs(_distG_Apres - _distG_Avant);
	logDebug0("Rapport diam D / G = ", _rapport_D_sur_G);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueD);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_diamRoueD = _deplacement.lireParametre();
	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueG);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_diamRoueG = _deplacement.lireParametre();
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueG, _diamRoueG * _rapport_D_sur_G);
	logDebug0("Diametre Roue D = ", _diamRoueD);
	logDebug0("Diametre Roue G compense = ", _diamRoueG * _rapport_D_sur_G);

	logDebug0("FIN CALIBRATION RAPPORT DES DIAMETRES");
}

void CalibrationDepla::facteurEchelle() {
	logDebug0("DEBUT CALIBRATION DU FACTEUR D'ECHELLE DES ENCODEURS");

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	logDebug3("Asservissement OFF");

	logDebug0("RECALER ROBOT...., puis entrée");
	getchar(); // PAUSE ____________________________________________

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distD_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue D : ", _distD_Avant);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distG_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue G : ", _distG_Avant);

	logDebug0("START.....(puis ENTREE)");
	getchar(); // PAUSE ____________________________________________
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, true);
	logDebug3("Asservissement ON");

	_deplacement.avancerInfini(VITESSE_LINEAIRE_RECALAGE, SensAvance::Arriere);
	this->attendreBlocage();

	sleep(1_s);

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	logDebug3("Asservissement OFF");

	logDebug0("VERIF ROBOT CALE SUR AVANT.....(puis ENTREE)");
	getchar(); // PAUSE ____________________________________________

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distD_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue D apres : ", _distD_Apres);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_distG_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue G apres : ", _distG_Apres);

	_distanceParcourue = ((_distD_Apres - _distD_Avant) + (_distG_Apres - _distG_Avant)) / 2.0;
	logDebug0("Distance parcourue = ", _distanceParcourue);

	// FacteurEchelle_Reel_sur_Mesure = distance théorique parcourue / distanceParcourue
	// = LONGUEUR TABLE (3000) - LONGUEUR ROBOT (210) / distanceParcourue
	// 2.792 Pour Chronos
	// 2.852 pour Pan
	_facteurEchelle_Reel_sur_Mesure = 2.60 / _distanceParcourue; // Secondaire : 2. //Principal : 2.
	logDebug0("Facteur d'echelle reel / mesure = ", _facteurEchelle_Reel_sur_Mesure);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueD);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_diamRoueD = _deplacement.lireParametre() * _facteurEchelle_Reel_sur_Mesure;
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueD, _diamRoueD);
	logDebug0("Diametre Roue D calibre = ", _diamRoueD);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueG);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_diamRoueG = _deplacement.lireParametre() * _facteurEchelle_Reel_sur_Mesure;
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueG, _diamRoueG);
	logDebug0("Diametre Roue G calibre = ", _diamRoueG);

	logDebug0("FIN CALIBRATION DU FACTEUR D'ECHELLE DES ENCODEURS");
}

void CalibrationDepla::entreAxes() {

	logDebug0("DEBUT CALIBRATION ENTREAXE (Distance entre les 2 roues codeuses)");

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	logDebug3("Asservissement OFF");

	logDebug0("RECALER ROBOT..... puis entrée");
	getchar(); // PAUSE ____________________________________________

	_deplacement.demanderParametre(ParametresCarte::Etat_AngleBrut);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_angleBrutRobot_Avant = _deplacement.lireParametre();
	logDebug0("Angle brut du robot = ", _angleBrutRobot_Avant);

	logDebug0("START.....(puis ENTREE)");
	getchar(); // PAUSE ____________________________________________
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, true);
	logDebug3("Asservissement ON");

	_deplacement.avancer(400_mm);
	this->attendreFinDeplacement();

	_dep->pushVitesseAngulaire(2_rad_s);

	_deplacement.tourner(10_PI);
	this->attendreFinDeplacement(TEMPS_DEPLACEMENT_MAX * 20);

	_dep->popVitesseAngulaire();

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::POSITION, false);
	logDebug3("Asservissement OFF");

	logDebug0("RECALER ROBOT SUR ARRIERE.....(puis ENTREE)");
	getchar(); // PAUSE ____________________________________________

	_deplacement.demanderParametre(ParametresCarte::Etat_AngleBrut);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_angleBrutRobot = _deplacement.lireParametre();
	logDebug0("Angle brut du robot = ", _angleBrutRobot);

	_rapportEntreAxe_Reel_sur_Mesure = (_angleBrutRobot - _angleBrutRobot_Avant) / (10.0 * M_PI);
	logDebug0("Rapport Entreaxe reel / mesure = ", _rapportEntreAxe_Reel_sur_Mesure);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_EntreAxe);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_entreAxe = _deplacement.lireParametre();
	logDebug0("Ancien EntreAxe = ", _entreAxe);

	_entreAxe = _entreAxe * _rapportEntreAxe_Reel_sur_Mesure;
	logDebug0("Nouvel EntreAxe = ", _entreAxe);

	getRobot().getCarte<DEPLACEMENT>().reglerParametre(ParametresCarte::Mecanique_EntreAxe, _entreAxe);

	logDebug0("FIN CALIBRATION ENTREAXE");
}

void CalibrationDepla::saveDatas() {
	FILE* F = fopen("calibration_deplacementla.txt", "a");
	fprintf(F, "%.10f\t%.10f\t%.10f\n", _diamRoueD, _diamRoueG, _entreAxe);
	fclose(F);

	logDebug6("Fin du programme");
}

void CalibrationDepla::testDeplacement() {
	_deplacement.allerADecompose(2.675_m, 1.225_m, SensAvance::Avant);
	this->attendreFinDeplacement();
	_deplacement.tournerAbsolu(0_rad, SensRotation::Trigo);
	this->attendreFinDeplacement();
	getchar();
	_deplacement.allerADecompose(50_cm, 30_cm, SensAvance::Avant);
	this->attendreFinDeplacement();
	_deplacement.tournerAbsolu(0.5_PI, SensRotation::Trigo);
	this->attendreFinDeplacement();
	getchar();
	/*dep->allerADecompose(32.5_cm, 1.225_m, SENS_AVANT);
	 attendreFinDeplacement();
	 dep->allerADecompose(32.5_cm, 50_cm, SENS_AVANT);
	 attendreFinDeplacement();getchar();
	 dep->allerADecompose(1_m, 50_cm, SENS_AVANT);
	 attendreFinDeplacement();
	 dep->allerADecompose(1_m, 1.225_m, SENS_AVANT);
	 attendreFinDeplacement();*/
	_deplacement.allerADecompose(2.675_m, 1.225_m, SensAvance::Avant);
	this->attendreFinDeplacement();
	_deplacement.tournerAbsolu(0_rad, SensRotation::Trigo);
	this->attendreFinDeplacement();
	_deplacement.arreter();
}

void CalibrationDepla::diametresAuto() {
	logDebug0("DEBUT CALIBRATION RAPPORT DES DIAMETRES");

	_deplacement.allerADecompose(1.5_m, 1.1_m, SensAvance::Avant);
	this->attendreFinDeplacement();

	// dep->pointerVers(0, 1100, SENS_TRIGO);
	// attendreFinDeplacement();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);

	_deplacement.avancerInfini(200_mm_s, SensAvance::Arriere);
	this->attendreBlocage();
	sleep(1_s);

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distD_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue D : ", _distD_Avant);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distG_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue G : ", _distG_Avant);

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);

	_deplacement.avancer(200_mm);
	sleep(100_ms);
	this->attendreFinDeplacement();

	_deplacement.allerADecompose(2.7_m, 35_cm, SensAvance::Avant);
	this->attendreFinDeplacement();
	_deplacement.allerADecompose(30_cm, 35_cm, SensAvance::Avant);
	this->attendreFinDeplacement();
	_deplacement.allerADecompose(30_cm, 1.1_m, SensAvance::Avant);
	this->attendreFinDeplacement();
	_deplacement.allerADecompose(2.7_m, 1.1_m, SensAvance::Avant);
	this->attendreFinDeplacement();

	_deplacement.tourner(3_rad);
	this->attendreFinDeplacement();
	_deplacement.pointerVers(Coordonnees({0_m, 1.1_m}, 0_deg, _deplacement.getReference()), SensRotation::Trigo);
	this->attendreFinDeplacement();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);

	_deplacement.avancerInfini(200_mm_s, SensAvance::Arriere);
	this->attendreBlocage();
	sleep(1_s);

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distD_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue D apres : ", _distD_Apres);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distG_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue G apres : ", _distG_Apres);

	_rapport_D_sur_G = fabs(_distD_Apres - _distD_Avant) / fabs(_distG_Apres - _distG_Avant);
	logDebug0("Rapport diam D / G = ", _rapport_D_sur_G);

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);

	_deplacement.avancer(200_mm);
	sleep(100_ms);
	this->attendreFinDeplacement();

	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueD);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_diamRoueD = _deplacement.lireParametre();
	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_diamRoueG = _deplacement.lireParametre();
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueG, _diamRoueG * _rapport_D_sur_G);
	logDebug0("Diametre Roue D = ", _diamRoueD);
	logDebug0("Diametre Roue G compense = ", _diamRoueG * _rapport_D_sur_G);

	logDebug0("FIN CALIBRATION RAPPORT DES DIAMETRES");
}

void CalibrationDepla::facteurEchelleAuto() {
	logDebug0("DEBUT CALIBRATION DU FACTEUR D'ECHELLE DES ENCODEURS");

	_deplacement.allerADecompose(2.7_m, 1.1_m, SensAvance::Avant);
	this->attendreFinDeplacement();

	_deplacement.pointerVers(Coordonnees({0_m, 1100_mm}, 0_deg, _deplacement.getReference()), SensRotation::Trigo);
	this->attendreFinDeplacement();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);

	_deplacement.avancerInfini(200_mm_s, SensAvance::Arriere);
	this->attendreBlocage();
	sleep(1_s);

	_deplacement.definirCoordonnees(
	    Coordonnees(Vector2m((3_m - RobotPrincipal::RAYON_ROBOT_RECALAGE), _robot->actualiserEtLireCoordonnees().getY()),
	                -1_PI,
	                _deplacement.getReference()));

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distD_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue D : ", _distD_Avant);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distG_Avant = _deplacement.lireParametre();
	logDebug0("Distance Roue G : ", _distG_Avant);

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);

	_deplacement.allerA(2.6_m, _robot->actualiserEtLireCoordonnees().getY(), SensAvance::Avant);
	this->attendreFinDeplacement();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);

	_deplacement.avancerInfini(200_mm_s, SensAvance::Avant);
	this->attendreBlocage();
	sleep(1_s);

	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueD);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distD_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue D apres : ", _distD_Apres);
	_deplacement.demanderParametre(ParametresCarte::Etat_DistanceRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_distG_Apres = _deplacement.lireParametre();
	logDebug0("Distance Roue G apres : ", _distG_Apres);

	_deplacement.arreter();
	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);

	_distanceParcourue = ((_distD_Apres - _distD_Avant) + (_distG_Apres - _distG_Avant)) / 2.0;
	logDebug0("Distance parcourue = ", _distanceParcourue);

	// distance théorique parcourue (longueur de la table - taille du robot) / distance parcourue
	_facteurEchelle_Reel_sur_Mesure = (3 - 0.255) / _distanceParcourue;
	logDebug0("Facteur d'echelle reel / mesure = ", _facteurEchelle_Reel_sur_Mesure);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueD);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_diamRoueD = _deplacement.lireParametre() * _facteurEchelle_Reel_sur_Mesure;
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueD, _diamRoueD);
	logDebug0("Diametre Roue D calibre = ", _diamRoueD);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_DiametreRoueG);
	while(!_deplacement.parametreActualise())
		sleep(10_ms);
	_diamRoueG = _deplacement.lireParametre() * _facteurEchelle_Reel_sur_Mesure;
	_deplacement.reglerParametre(ParametresCarte::Mecanique_DiametreRoueG, _diamRoueG);
	logDebug0("Diametre Roue G calibre = ", _diamRoueG);

	_deplacement.allerADecompose(2.7_m, 1.1_m, SensAvance::Arriere);
	this->attendreFinDeplacement();

	logDebug0("FIN CALIBRATION DU FACTEUR D'ECHELLE DES ENCODEURS");
}

void CalibrationDepla::entreAxesAuto() {

	logDebug0("DEBUT CALIBRATION ENTREAXE");

	_deplacement.allerADecompose(2.7_m, 1.1_m, SensAvance::Avant);
	this->attendreFinDeplacement();

	_deplacement.pointerVers(Coordonnees({1.5_m, 1.1_m}, 0_deg, _deplacement.getReference()), SensRotation::Trigo);
	this->attendreFinDeplacement();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);

	_deplacement.avancerInfini(200_mm_s, SensAvance::Arriere);
	this->attendreBlocage();
	sleep(1_s);

	_deplacement.definirCoordonnees(
	    Coordonnees(Vector2m((3_m - RobotPrincipal::RAYON_ROBOT_RECALAGE), _robot->actualiserEtLireCoordonnees().getY()),
	                -1_PI,
	                _deplacement.getReference()));

	_deplacement.demanderParametre(ParametresCarte::Etat_AngleBrut);
	while(!getRobot().getCarte<DEPLACEMENT>().parametreActualise())
		sleep(100_ms);
	_angleBrutRobot_Avant = _deplacement.lireParametre();
	logDebug0("Angle brut du robot = ", _angleBrutRobot_Avant);

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);

	_deplacement.avancer(200_mm);
	sleep(1_s);
	this->attendreFinDeplacement();

	_dep->pushVitesseAngulaire(4_rad_s);

	_deplacement.tourner(10_PI);
	this->attendreFinDeplacement();

	_dep->popPrecionAngulaire();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, false);

	_deplacement.avancerInfini(200_mm_s, SensAvance::Arriere);
	this->attendreBlocage();
	sleep(1_s);

	_deplacement.demanderParametre(ParametresCarte::Etat_AngleBrut);
	while(!getRobot().getCarte<DEPLACEMENT>().parametreActualise())
		sleep(100_ms);
	_angleBrutRobot = _deplacement.lireParametre();
	logDebug0("Angle brut du robot = ", _angleBrutRobot);

	_deplacement.arreter();

	_deplacement.activerAsservissement(CarteInfo<DEPLACEMENT>::typeCarte::ANGLE, true);

	_rapportEntreAxe_Reel_sur_Mesure = (_angleBrutRobot - _angleBrutRobot_Avant) / (10.0 * M_PI);
	logDebug0("Rapport Entreaxe reel / mesure = ", _rapportEntreAxe_Reel_sur_Mesure);

	_deplacement.demanderParametre(ParametresCarte::Mecanique_EntreAxe);
	while(!_deplacement.parametreActualise())
		sleep(100_ms);
	_entreAxe = _deplacement.lireParametre();
	logDebug0("Ancien EntreAxe = ", _entreAxe);

	_entreAxe = _entreAxe * _rapportEntreAxe_Reel_sur_Mesure;
	logDebug0("Nouvel EntreAxe = ", _entreAxe);

	getRobot().getCarte<DEPLACEMENT>().reglerParametre(ParametresCarte::Mecanique_EntreAxe, _entreAxe);

	_deplacement.avancer(200_mm);
	sleep(1_s);
	this->attendreFinDeplacement();

	logDebug0("FIN CALIBRATION ENTREAXE");
}

void CalibrationDepla::arreter() {}