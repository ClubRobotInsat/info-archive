#include "../CAN.h"
#include "../RS232.h"
#include "../Serie.h"
#include "../TCPIP.h"

using namespace Commun;

int main(int argc, char* argv[]) {
	std::unique_ptr<CAN> monCAN;

	std::cout << "** TESTEUR DE FONCTIONNALITES CAN **\nPermet l'envoi et la reception de trames CAN pour les tests\n";
	// si argument TCPIP
	if(argc == 4 && argv[1] == "TCPIP"s) {
		monCAN = std::make_unique<CAN>(std::make_unique<TCPIP>(argv[2], Utils::strToNumber<uint16_t>(argv[3])));
		// si argument RS232
	} else if(argc == 3 && argv[1] == "RS232"s) {
		monCAN = std::make_unique<CAN>(std::make_unique<RS232>(argv[2]));
		// si argument incorrect
	} else if(argc != 1) {
		std::cout << "parametre incorrect => testeur_CAN type adresse port\nEx: testeur_CAN TCPIP 127.0.0.1 1234 ou "
		             "testeur_CAN RS232 COM6)\n";
		exit(-1);
		// si aucun argument
	} else {
		int choix;
		std::string adresse;

		std::cout << "CONNEXION\n0 Quitter\n1 RS232\n2 TCP/IP\n> ";
		std::cin >> choix;
		switch(choix) {
			case 1:
				std::cout << "Entrez l'adresse du port RS232 (ex: '/dev/ttyUSB0' ou 'COM5')\n";
				std::cin >> adresse;
				monCAN = std::make_unique<CAN>(std::make_unique<RS232>(adresse));
				break;
			case 2:
				std::cout << "Entrez l'adresse et le port du serveur (ex: 'localhost 4242')\n";
				uint16_t port;
				std::cin >> adresse >> port;
				monCAN = std::make_unique<CAN>(std::make_unique<TCPIP>(adresse, port));
				break;
			default:
				exit(0);
		}
	}

	while(1) {
		monCAN->envoyerTrame(Trame(0x0, 0, {1, 2, 3, 4, 5, 6, 7, 8}));
		sleep(100_ms);
	}

	return 0;
}
