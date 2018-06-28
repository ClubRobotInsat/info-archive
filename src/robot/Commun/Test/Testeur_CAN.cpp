#include "../Communication/CAN.h"
#include "../Communication/RS232.h"
#include "../Communication/Serie.h"
#include "../Communication/TCPIP.h"
#include <log/Log.h>

using namespace Commun;

std::unique_ptr<CAN> monCAN;

// TODO ajouter des états de cible à tester
enum STATE { TEST_CAN, TEST_CMD_SERVO, QUIT };
std::atomic<STATE> actual_state;
Duration tempo(100_ms);
bool inPause;

// thread qui permet de quitter, changer la tempo ou le mode de test en live
void parseConsole() {
	while(actual_state != QUIT) {
		// Obtention de la commande de l'utilisateur.
		std::string input;
		std::getline(std::cin, input);
		inPause = true;

		if(input == std::string("q")) { // Quitter
			actual_state = QUIT;
		} else if(input == std::string("r")) { // Reset
			logDebug1("Reset du CAN et changement de cible à tester");
			bool ok = false;
			do {
				std::cout << "Quelle commande voulez-vous tester? [can|servo] " << std::flush;
				std::string input;
				std::cin >> input;
				if(input == "can") {
					actual_state = TEST_CAN;
					ok = true;
				} else if(input == "servo") {
					actual_state = TEST_CMD_SERVO;
					ok = true;
				}
				std::cout << "actual state = " << actual_state << std::endl;
			} while(!ok);

			monCAN->reinitialiserFenetre();
			monCAN->reinitialiserNumSequence();
		} else if(input == std::string("t")) { // Set temporisation
			logDebug1("Changement de la temporisation par défaut (actuellement : ", tempo, ")");
			std::cout << "Quelle temporisation voulez-vous mettre en ms ? " << std::flush;
			int value;
			std::cin >> value;
			tempo = Duration::makeFromMs(value);
			monCAN->setTemporisation(tempo);
		} else {
			logDebug1("Help : q (quit), r (reset), t (set tempo)\n");
		}
		inPause = false;
	}
}

int main(int argc, char* argv[]) {
	logDebug("** TESTEUR DE FONCTIONNALITES CAN **\nPermet l'envoi et la reception de trames CAN pour les tests\n");
	// si argument TCPIP
	if(argc == 4 && argv[1] == "TCPIP"s) {
		monCAN = std::make_unique<CAN>(std::make_unique<TCPIP>(argv[2], Utils::strToNumber<uint16_t>(argv[3])));
		// si argument RS232
	} else if(argc == 3 && argv[1] == "RS232"s) {
		monCAN = std::make_unique<CAN>(std::make_unique<RS232>(argv[2]));
		// si argument incorrect
	} else if(argc != 1) {
		std::cout << "Usage : " << argv[0] << " <type> <adresse> <port>\n"
		                                      "Ex: '"
		          << argv[0] << " TCPIP 127.0.0.1 1234' ou '" << argv[0] << " RS232 COM6'\n";
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

	// Test par défaut
	actual_state = TEST_CMD_SERVO;

	// Listener des entrées utilisateur pour modifier en live des paramètres
	std::thread consoleThread(std::bind(&parseConsole));
	consoleThread.detach();
	inPause = false;

	while(actual_state != QUIT) {
		if(inPause) {
			continue;
		}

		switch(actual_state) {
			case TEST_CAN:
				monCAN->envoyerTrame(Trame(0x0, 0, {1, 2, 3, 4, 5, 6, 7, 8}));
				sleep(tempo);
				break;

			case TEST_CMD_SERVO:
				monCAN->envoyerTrame(Trame(0x02, 0x01, {0x03, 0xE6}));
				sleep(tempo);
				break;

			// TODO : ajouter des jeux de test / améliorer ceux existants
			case QUIT:
				break;
		}
	}

	pthread_cancel(consoleThread.native_handle());

	logDebug("Fin du programme de test.");

	return 0;
}
