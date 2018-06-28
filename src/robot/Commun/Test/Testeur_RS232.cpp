#include "../Communication/RS232.h"
#include <Commun.h>
#include <Units.h>

using namespace Commun;

int main() {
	int c = 1;
	std::string tty;
	unsigned int i;
	unsigned char octet;

	std::cout
	    << ("** TESTEUR DE FONCTIONNALITES RSR232 **\nNote: TOUS les nombres sont en hexadecimal !\nEntrez le port "
	        "serie utiliser (ex: '/dev/ttyS0' ou 'COM4') ");
	std::cin >> tty;

	auto monRS232 = std::make_unique<RS232>(tty);

	while(c) {
		std::cout << "\nMENU\n0 Quitter\n1 Envoyer un octet\n2 Recevoir un octet\n3 Envoyer un octet en boucle\n> ";
		c = 0;
		std::cin >> c;
		if(c == 1) {
			printf("octet ?\n");
			std::cin >> std::hex >> i;
			octet = (unsigned char)i;
			monRS232->ecrireOctet(octet);
			printf("Octet %x envoye\n", octet);
		} else if(c == 2) {
			octet = monRS232->lireOctet();
			printf("Octet %x recu\n", octet);
		} else if(c == 3) {
			printf("octet ?\n");
			std::cin >> std::hex >> i;
			octet = (unsigned char)i;
			printf("temps en chaque envoi en ms ?\n");
			int temp;
			std::cin >> temp;
			auto duree = Duration::makeFromMs(temp);
			while(1) {
				printf("Octet %x envoyee\n", octet);
				monRS232->ecrireOctet(octet);
				sleep(duree);
			}
		}
	}

	return 0;
}
