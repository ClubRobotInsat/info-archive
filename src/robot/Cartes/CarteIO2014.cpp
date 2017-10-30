/*
 * CarteIO2014.cpp
 *
 *  Created on: 20 mars 2014
 *      Author: seydoux
 */
#include "CarteIO2014.h"
//#include <stdlib.h>
#include "../../commun/log/Log.h"
#include <cstring>

CarteIO2014::CarteIO2014(Commun::CAN& can, uint8_t id) : Carte(can, id) {
	std::memset(_capteurs, 1, sizeof(_capteurs));
	for(int i = 0; i < NBCAPTEURS; i++)
		_capteurs_actualises[i] = false;
}

void CarteIO2014::traiterMessage(Trame const& message) {
	unsigned char noCapteur = 0xff_b;

	switch(message.getCmd()) {
		case 0x01_b:
			logDebug("L'actualisation de tous les capteurs à la fois n'est pas implémentée");
			// todo : cette fonction est appelée par l'élec à chaque changement de capteur, il faut
			// corriger le code élec
			// throw std::exception();
			break;
		case 0x02_b:
			// Reception de l'etat d'un capteur
			// Le numéro du capteur est le premier octet de données
			//			printf("taille = %d\n", taille);
			//			printf("data[0] = %d\n", donnees[0]);
			noCapteur = message.getDonnee(0);
			// nocapteur entre 1 et 12
			if(noCapteur > 0 && noCapteur <= NBCAPTEURS) {
				if(message.getNbDonnees() == 2) {
					// printf("data[1] = %d\n", message.getDonnee(1));
					// On a reçu un booléen
					_capteurs[noCapteur] = !(message.getDonnee(1) == 0x00_b);
					if(message.getDonnee(1) == 0xff) {
						// Si la carte ne reconnaît pas un numéro de capteur, elle renvoie 0xff
						logError("Numéro de capteur non reconnu par la carte IO");
						// throw std::exception();
					}
					//						logDebug("|||||| CAPTEUR N°", (int)noCapteur, " : ", (int) donnees[1],", ",
					//_capteurs[noCapteur-1]);
					_capteurs_actualises[noCapteur] = true;

					// printf("le capteur est actualisé\n");
				} else if(message.getNbDonnees() == 3) {
					//				printf("data[1] = %d\n", donnees[1]);
					//				printf("data[2] = %d\n", donnees[2]);
					// On a reçu une donnée analogique
					// On reconstitue la valeur entière associée
					//					logDebug("Màj capteurs");


					// FIXME: pas beau pas beau !!
					logError("Attention ! Possible confusion big-endian/little endian !");
					_capteurs[noCapteur] = (message.getDonnee(1) << 8) + message.getDonnee(2);
					_capteurs_actualises[noCapteur] = true;

					printf("le capteur est actualisé\n");
				}
			} else {
				logError("Mauvais capteur : ", (int)noCapteur);
			}
			//			else
			//			{
			//				logDebug("Tony, c'est la merde !!!");
			//				printf("taille = %d\n", taille);
			//				printf("data[0] = %d\n", donnees[0]);
			//				// Ceci est une référence aux Lascars, qui est un dessin animé très drôle. Regardez le.
			//				// Sans déconner, il est vraiment casse de rire. Et puis il y a Vincent Cassel dedans.
			//				// PS : la carte n'est pas sensée renvoyer de data de taille ni 2 ni 3,
			//				// c'est pour ça que si vous êtes dans cette branche du if, c'est la merde.
			//				//throw std::exception();
			//			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}


// Envoie la demande de lecture des capteurs
void CarteIO2014::actualiserUnCapteur(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS) {
		logError("Numéro de capteur ", (int)noCapteur, " hors limite pour la carte IO");
	} else {
		// Envoi de la requête de données
		// attention, romain a foutu la merde en numérotant de 1 à 12
		// logDebug("demande d'actualisation du capteur ", (int)noCapteur);
		// logDebug("mapping au capteur interne ", (int)noCapteurInterne);
		_capteurs_actualises[noCapteur] = false;
		debloquerVariables();
		envoyerMessage(0x02_b, noCapteur);
	}
}

bool CarteIO2014::lireUnCapteur(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS || noCapteur <= 0) {
		logError("Numéro de capteur hors limite pour la carte IO");
		return false;
	} else {
		// logDebug("|||||| LIRE CAPTEUR N°", (int)noCapteur, " : ", _capteurs[noCapteur-1]);
		return _capteurs[noCapteur];
	}
}

bool CarteIO2014::capteurEstActualise(uint8_t noCapteur) {
	if(noCapteur > NBCAPTEURS || noCapteur <= 0) {
		logError("Numéro de capteur hors limite pour la carte IO");
		return false;
	} else {
		return _capteurs_actualises[noCapteur];
	}
}

bool CarteIO2014::actualiserPuisLireUnCapteur(uint8_t noCapteur) {
	int demandesActualisation = 0;
	actualiserUnCapteur(noCapteur);
	while(!capteurEstActualise(noCapteur)) {
		sleep(10_ms);
		demandesActualisation++;
		// sécurité : toutes les 5 vérifications, on renvoie le message
		if((demandesActualisation % 5) == 0) {
			actualiserUnCapteur(noCapteur);
		}
	}

	bool valeurCapteur = lireUnCapteur(noCapteur);
	// logDebug("%%% VALEUR CAPTEUR : ", valeurCapteur);

	return valeurCapteur;
}
