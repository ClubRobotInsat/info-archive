/************************************************************
 * CarteXbee2010.cpp *
 * Spécifie la classe gérant les Xbee 2009/2010 *
 ************************************************************/

#include "CarteXbee2010.h"

#include <cstring>
#include <ctype.h>

CarteXbee2010::CarteXbee2010(Commun::CAN& can, uint8_t id) : Carte(can, id), _octetRecu(false) {}

void CarteXbee2010::traiterMessage(Trame const& message) {
	static unsigned char compteur1 = 0;
	static unsigned long long compteurMax1 = 0;
	static unsigned int compteurErreur1 = 0;

	static unsigned char compteur2 = 0;
	static unsigned long long compteurMax2 = 0;
	static unsigned int compteurErreur2 = 0;
	switch(message.getCmd()) {
		case 0x01_b: // Reception d'un octet compteur de test comm CAN vers USB
			if(message.getNbDonnees() == 1) {
				if(message.getDonnee(0) != compteur1) {
					printf("erreur compteur=%2x recu=%2x\n", compteur1, message.getDonnee(0));
					compteur1 = message.getDonnee(0);
					compteurErreur1++;
				}
				compteur1++;
				compteurMax1++;
				if((compteurMax1 / 1000) * 1000 == compteurMax1)
					printf("%lld : compteurErreur=%2x\n", compteurMax1, compteurErreur1);
				bloquerVariables();
				_aOctetRecu = true;
				_octetRecu = message.getDonnee(0);
				debloquerVariables();
			}
			break;
		case 0x02_b: // Reception d'une erreur sur le compteur de test de com USB vers CAN
			if(message.getNbDonnees() == 1) {
				if(message.getDonnee(0) != compteur2) {
					printf("erreur compteur=%2x recu=%2x\n", compteur2, message.getDonnee(0));
					compteur2 = message.getDonnee(0);
					compteurErreur2++;
				}
				compteur2++;
				compteurMax2++;
				if((compteurMax2 / 1000) * 1000 == compteurMax2)
					printf("%lld : compteurErreur=%2x\n", compteurMax2, compteurErreur2);
			}
			break;
		case 0x03_b: // Reception du comteur de trame de test de com USB vers CAN
			if(message.getNbDonnees() == 2) {
				printf("\t%d : compteurErreur=%2x\n", message.getDonnee(0) * 1000, message.getDonnee(1));
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Envoie d'un octet au xbee
void CarteXbee2010::envoyerOctet(unsigned char octet) {
	envoyerMessage(0x01_b, octet);
}

// retourne vrai si il y a un octet recu
bool CarteXbee2010::aOctetRecu() {
	return _aOctetRecu.exchange(false);
}

// Retourne l'octet recu
uint8_t CarteXbee2010::getOctetRecu() {
	return _octetRecu;
}
