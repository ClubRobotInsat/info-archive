//
// Created by louis on 09/03/17.
//

#include <getopt.h>

#ifndef ROOT_PARSING_H
#define ROOT_PARSING_H

#endif // ROOT_PARSING_H
namespace ia_parsing {

	RobotColor parseColor(int argc, char* argv[]) {
		static struct option long_options[] = {{"color", required_argument, 0, 'c'},
		                                       {"debug", optional_argument, 0, 'd'},
		                                       {0, 0, 0, 0}};

		int arg;
		int long_index = 0;
		RobotColor color = RobotColor::Undef;

		while((arg = getopt_long(argc, argv, "c:d", long_options, &long_index)) != -1) {

			switch(arg) {
				case 'c':
					if(std::string(optarg) == "orange") {
						color = RobotColor::Orange;
					} else if(std::string(optarg) == "green") {
						color = RobotColor::Green;
					}
				default:;
			}
		}

		if(color == Constantes::RobotColor::Undef) {
			std::cout << "Pas de couleur selectionnée. Utilisation de la couleur verte par défaut..." << std::endl;
			std::cout << "Usage : " << argv[0] << " --color [orange|green]" << std::endl;
			color = RobotColor::Green;
		}

		return color;
	}

	bool getDebugMode(int argc, char* argv[]) {
		for(int i = 0; i < argc; i++) {
			if(std::string(argv[i]) == "--debug") {
				return true;
			}
		}
		return false;
	}

	enum TestMeca {
		// servos
		PORTE_CUBES = 0,
		SOUTE_GAUCHE = 1,
		SOUTE_DROITE = 2,
		ABEILLE_GAUCHE = 3,
		ABEILLE_DROITE = 4,

		// moteurs
		ASCENSEUR_GAUCHE = 5,
		ASCENSEUR_DROIT = 6,
		AVALEUR_GAUCHE = 7,
		AVALEUR_DROIT = 8,

		// séquences

		UNDEF = 9,
	};
	TestMeca parseTestMeca(int argc, char* argv[]) {
		static struct option long_options[] = {{"cubes", no_argument, 0, 'c'},
		                                       {"souteg", no_argument, 0, 's'},
		                                       {"souted", no_argument, 0, 'x'},
		                                       {"abeilleg", no_argument, 0, 'a'},
		                                       {"abeilled", no_argument, 0, 'q'},
		                                       {"ascenseurg", no_argument, 0, 'j'},
		                                       {"ascenseurd", no_argument, 0, 'k'},
		                                       {"avaleurg", no_argument, 0, 'l'},
		                                       {"avaleurd", no_argument, 0, 'm'},
		                                       {0, 0, 0, 0}};

		int arg;
		int long_index = 0;
		TestMeca result = UNDEF;

		while((arg = getopt_long(argc, argv, "csxaqjklm:", long_options, &long_index)) != -1) {

			switch(arg) {
				case 'c':
					result = TestMeca::PORTE_CUBES;
					break;
				case 's':
					result = TestMeca::SOUTE_GAUCHE;
					break;
				case 'x':
					result = TestMeca::SOUTE_DROITE;
					break;
				case 'a':
					result = TestMeca::ABEILLE_GAUCHE;
					break;
				case 'q':
					result = TestMeca::ABEILLE_DROITE;
					break;
				case 'j':
					result = TestMeca::ASCENSEUR_GAUCHE;
					break;
				case 'k':
					result = TestMeca::ASCENSEUR_DROIT;
					break;
				case 'l':
					result = TestMeca::AVALEUR_GAUCHE;
					break;
				case 'm':
					result = TestMeca::AVALEUR_DROIT;
					break;
				default:;
			}
		}

		if(result == TestMeca::UNDEF) {
			std::cout << "Pas de servo spécifié. Test de la porte pour les cubes par défaut." << std::endl;
			std::cout << "Usage : " << argv[0] << " [--cubes|--souteg|--souted|--abeilleg|--abeilled|--ascenseurg|"
			          << "--ascenseurd|--avaleurg|--avaleurd]" << std::endl;
			result = PORTE_CUBES;
		}

		return result;
	}
}