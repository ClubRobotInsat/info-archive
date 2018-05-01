//
// Created by louis on 09/03/17.
//

#include <getopt.h>

#ifndef ROOT_PARSING_H
#define ROOT_PARSING_H

#endif // ROOT_PARSING_H
namespace ia_parsing {

	struct ParsedArguments {
		RobotColor color;
		bool debug_mode;
		bool magic_strategy;

		friend std::ostream& operator<<(std::ostream& os, const ParsedArguments& obj) {
			os << "Parsing des arguments :" << std::endl;
			os << "\tCouleur du robot : " << toString(obj.color) << std::endl;
			os << "\tDebug mode : " << std::boolalpha << obj.debug_mode << std::endl;
			return os << "\tStrategie utilisée : " << (obj.magic_strategy ? "magicStrategy" : "petri") << std::endl;
		}
	};

	void print_help(std::string prog) {
		std::cout << "Usage: " << prog << " --color|-c <green|orange> [--debug|-d [on|off]] [--strat|-s <petri|magic>]"
		          << std::endl;
	}

	ParsedArguments parsing_function(int argc, char* argv[]) {
		static struct option long_options[] = {{"color", required_argument, 0, 'c'},
		                                       {"debug", optional_argument, 0, 'd'},
		                                       {"strat", optional_argument, 0, 's'},
		                                       {0, 0, 0, 0}};

		int arg;
		int long_index = 0;

		ParsedArguments result;
		result.color = RobotColor::Undef;
		result.debug_mode = false;
		result.magic_strategy = false;

		while((arg = getopt_long(argc, argv, "c:d:s", long_options, &long_index)) != -1) {
			switch(arg) {
				case 'c':
					if(std::string(optarg) == "green") {
						result.color = RobotColor::Green;
					} else if(std::string(optarg) == "orange") {
						result.color = RobotColor::Orange;
					}
					break;

				case 'd':
					if(std::string(optarg) == "off") {
						result.debug_mode = false;
					} else {
						result.debug_mode = true;
					}
					break;

				case 's':
					if(std::string(optarg) == "petri") {
						result.magic_strategy = false;
					} else if(std::string(optarg) == "magic") {
						result.magic_strategy = true;
					} else {
						print_help(argv[0]);
					}
					break;

				default:
					print_help(argv[0]);
					break;
			}
		}

		if(result.color == Constantes::RobotColor::Undef) {
			std::cout << "Pas de couleur selectionnée. Utilisation de la couleur verte par défaut..." << std::endl;
			print_help(argv[0]);
			result.color = RobotColor::Green;
		}

		return result;
	}

	enum TestMeca {
		// servos
		PORTE_CUBES = 0,
		SOUTE_GAUCHE = 1,
		SOUTE_DROITE = 2,
		ABEILLE_GAUCHE = 3,
		ABEILLE_DROITE = 4,

		// moteurs
		ASCENSEURS = 5,
		AVALEURS = 6,

		// séquences

		UNDEF = 7,
	};
	TestMeca parseTestMeca(int argc, char* argv[]) {
		static struct option long_options[] = {{"cubes", no_argument, 0, 'c'},
		                                       {"souteg", no_argument, 0, 's'},
		                                       {"souted", no_argument, 0, 'x'},
		                                       {"abeilleg", no_argument, 0, 'a'},
		                                       {"abeilled", no_argument, 0, 'q'},
		                                       {"ascenseur", no_argument, 0, 'j'},
		                                       {"avaleur", no_argument, 0, 'v'},
		                                       {0, 0, 0, 0}};

		int arg;
		int long_index = 0;
		TestMeca result = UNDEF;

		while((arg = getopt_long(argc, argv, "csxaqjv:", long_options, &long_index)) != -1) {

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
					result = TestMeca::ASCENSEURS;
					break;
				case 'v':
					result = TestMeca::AVALEURS;
					break;
				default:;
			}
		}

		if(result == TestMeca::UNDEF) {
			std::cout << "Pas de servo spécifié. Test de la porte pour les cubes par défaut." << std::endl;
			std::cout << "Usage : " << argv[0]
			          << " [--cubes|--souteg|--souted|--abeilleg|--abeilled|--ascenseur|--avaleur]" << std::endl;
			result = PORTE_CUBES;
		}

		return result;
	}
}