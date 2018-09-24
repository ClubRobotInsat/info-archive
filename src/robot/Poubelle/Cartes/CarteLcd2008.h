/*******************************************************************
 * CarteLcd2008.h *
 * Implémente la classe gérant l'ecriture sur ecran LCD 2007/2008 *
 *******************************************************************/

#ifndef _CARTE_LCD_2008_H_
#define _CARTE_LCD_2008_H_

#include "Carte.h"

#define MESSAGE_SPREE 0x01_b
#define MESSAGE_COMBO 0x02_b
#define MESSAGE_FIN_MATCH 0x03_b
#define MESSAGE_TRY_AGAIN 0x04_b
#define MESSAGE_GUTTER 0x05_b
#define MESSAGE_HEADSHOT 0x06_b

class CarteLcd2008 : public Carte {
public:
	CarteLcd2008(Commun::CAN& can, uint8_t id) : Carte(can, id), _choixIA(-1), _nombreIAs(0), _lastID(0) {}

	virtual void traiterMessage(Trame const& message) override;

	// Retourne l'ia qui a été choisie
	uint8_t getIA();

	// Envoye une IA a l'ecran
	void envoyerIA(const char* nom);

	// Retourne l'id de la dernier ia envoyée
	uint8_t getLastID();

	// Envoye un message a afficher sur l'ecran LCD mode terminal
	void afficherTerminal(const char* format, va_list ap);

	// Envoye un message a afficher sur l'ecran LCD mode match
	void afficherMatch(const char* format, va_list ap);

	// Demande l'affi chage d'un truc con
	void afficherPerso(uint8_t message);

	// Demande l'espionnage de la carte d'id donne
	void setIDEspion(uint8_t id);

private:
	std::atomic<uint8_t> _choixIA;
	std::atomic<uint8_t> _nombreIAs;
	std::atomic<uint8_t> _lastID;
	/*
	 enum NumFinMessageLcd // Les identifiants de fin de message pour l'ecran LCD {
	 MESSAGE_CONTINUE = 0xFF,
	 MESSAGE_FIN = 0x01,
	 };
	 */
	enum NumIdMessageLcd : uint8_t { // Les id des messages
		ID_MESSAGE_TERMINAL = 0x1,
		ID_MESSAGE_MATCH = 0x2,
		ID_NOUVELLE_IA = 0x3,
		ID_SELECTION_IA = 0x4,
		ID_CLEAR_IA = 0x5,
		ID_MESSAGE_FUN = 0x6,
		ID_SET_ESPION = 0x7
	};
};

#endif
