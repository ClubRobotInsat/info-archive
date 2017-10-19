/*******************************************************************
 * CarteLcd2009.cpp *
 * Implémente la classe gérant l'ecriture sur ecran LCD 2008/2009 *
 *******************************************************************/

#include "CarteLcd2009.h"
#include <cstring>

void CarteLcd2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case ID_NOUVELLE_IA:
			_lastID = message.getDonnee(0);
			break;
		case ID_SELECTION_IA:
			if(message.getNbDonnees() == 2) {
				_choixIA = message.getDonnee(0);

				if(_nombreIAs != message.getDonnee(1)) {
					fprintf(stderr, "Erreur carteLcd : La carte LCD et l'epia n'ont pas le même nombre d'ia !");
				}
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Retourne l'ia qui a été choisie, négatif si aucune IA
uint8_t CarteLcd2009::getIA() {
	return _choixIA;
}


// Retourne l'ia qui a été choisie, négatif si aucune IA
void CarteLcd2009::envoyerIA(const char* nom) {
	Trame t = this->make_trame(ID_NOUVELLE_IA);
	t.addBytes(8, reinterpret_cast<uint8_t const*>(nom));
	envoyerMessage(t);

	_nombreIAs++;
}

// Retourne l'id de la dernier ia envoyée
uint8_t CarteLcd2009::getLastID() {
	return _lastID.exchange(-1);
}

// Envoye un message a afficher sur l'ecran LCD mode terminal
void CarteLcd2009::afficherTerminal(const char* format, va_list ap) {
	char message[256];
	int index = 0;

	vsprintf(message, format, ap);

	// Envoi des blocs incomplets
	while(strlen(&message[index]) > 7) {
		Trame t = this->make_trame(ID_MESSAGE_TERMINAL);
		t.addBytes(7, reinterpret_cast<uint8_t const*>(message[index]));
		envoyerMessage(t);
		index += 7;
	}

	// Envoi du bloc final
	Trame t = this->make_trame(ID_MESSAGE_TERMINAL);
	t.addBytes(7, reinterpret_cast<uint8_t const*>(message[index]));
	envoyerMessage(t);
}

// Envoye un message a afficher sur l'ecran LCD mode match
void CarteLcd2009::afficherMatch(const char* format, va_list ap) {
	char message[256];
	int index = 0;

	vsprintf(message, format, ap);

	// Envoi des blocs incomplets
	while(strlen(&message[index]) > 7) {
		Trame t = this->make_trame(ID_MESSAGE_MATCH);
		t.addBytes(7, reinterpret_cast<uint8_t const*>(message[index]));
		envoyerMessage(t);
		index += 7;
	}

	// Envoi du bloc final
	Trame t = this->make_trame(ID_MESSAGE_MATCH);
	t.addBytes(7, reinterpret_cast<uint8_t const*>(message[index]));
	envoyerMessage(t);
}

// Demande l'effacement des messages IA
void CarteLcd2009::clearIA() {
	envoyerMessage(this->make_trame(ID_CLEAR_IA));

	_lastID = -1;
}


// Demande l'affichage d'un truc con
void CarteLcd2009::afficherPerso(uint8_t message) {
	envoyerMessage(ID_MESSAGE_FUN, message);
}

// Demande l'espionnage de la carte d'id donne
void CarteLcd2009::setIDEspion(uint8_t id) {
	envoyerMessage(ID_SET_ESPION, id);
}
