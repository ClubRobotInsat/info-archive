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
			std::cout << "Pas de couleur selectionnée. Utilisation de la couleur bleue par défaut..." << std::endl;
			std::cout << "Usage : " << argv[0] << " --color [orange|green]" << std::endl;
			color = RobotColor::Orange;
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
		PINCE = 0,
		LACET = 1,
		TANGAGE = 2,
		FUSEE = 3,
		GAUCHE = 4,
		DROITE = 5,
		ASCENSEUR = 6,
		STOCKER = 7,
		DESTOCKER = 8,
		SEQUENCE = 9,
		UNDEF = 10,
	};
	TestMeca parseTestMeca(int argc, char* argv[]) {
		static struct option long_options[] = {{"ascenseur", no_argument, 0, 'a'},
		                                       {"pince", no_argument, 0, 'p'},
		                                       {"lacet", no_argument, 0, 'l'},
		                                       {"tangage", no_argument, 0, 't'},
		                                       {"fusee", no_argument, 0, 'f'},
		                                       {"gauche", no_argument, 0, 'g'},
		                                       {"droite", no_argument, 0, 'd'},
		                                       {"id", required_argument, 0, 'i'},
		                                       {"stocker", no_argument, 0, 'o'},
		                                       {"destocker", no_argument, 0, 'e'},
		                                       {"sequence", no_argument, 0, 's'},
		                                       {0, 0, 0, 0}};

		int arg;
		int long_index = 0;
		TestMeca result = UNDEF;

		while((arg = getopt_long(argc, argv, "apltfgdoes:", long_options, &long_index)) != -1) {

			switch(arg) {
				case 'a':
					result = TestMeca::ASCENSEUR;
					break;
				case 'p':
					result = TestMeca::PINCE;
					break;
				case 'l':
					result = TestMeca::LACET;
					break;
				case 't':
					result = TestMeca::TANGAGE;
					break;
				case 'f':
					result = TestMeca::FUSEE;
					break;
				case 'g':
					result = TestMeca::GAUCHE;
					break;
				case 'd':
					result = TestMeca::DROITE;
					break;
				case 'i': {
					int id_servo = ((int)optarg[0]) - (int)'0';
					if(id_servo >= 0 && id_servo <= 5)
						result = (TestMeca)id_servo;
					else
						std::cout << "Usage : " << argv[0] << " --id <numéro du servo>" << std::endl;
				} break;
				case 'o':
					result = TestMeca::STOCKER;
					break;
				case 'e':
					result = TestMeca::DESTOCKER;
					break;
				case 's':
					result = TestMeca::SEQUENCE;
				default:;
			}
		}

		if(result == TestMeca::UNDEF) {
			std::cout << "Pas de servo spécifié. Test de l'ascenseur par défaut." << std::endl;
			std::cout << "Usage : " << argv[0] << " [--ascenseur|--lacet|--tangage|--fusee|--gauche|--droite|"
			          << "--id <id_servo>|--stocker|--destocker|--sequence]" << std::endl;
			result = ASCENSEUR;
		}

		return result;
	}
}