// CarteServosNova2017.h
// TODO !!

#ifndef CARTE_SERVOS_NOVA_2017_H
#define CARTE_SERVOS_NOVA_2017_H

#include "../simulateur/robot/Carte.h"
#include "IServosIntelligents.h"

class CarteServosNova2017 : public Carte {
public:
	CarteServosNova2017(unsigned char id, RobotLogic& robot, IServosIntelligents& iServos, uint8_t nbServos);
	virtual ~CarteServosNova2017() = default;

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoi d'un pong
	void envoyerPong();

	// Envoi de la position d'un servo
	void envoyerPosition(unsigned char numServo);

	// Envoi de l'information indiquant si le mouvement d'un servo est terminé ou pas
	void envoyerEtatTermine(unsigned char numServo, bool termine);

	// Envoi de l'état du servo indiqué (bloqué ou pas)
	void envoyerServoBloque(unsigned char numServo, bool bloque);

	// Mise à jour de la carte : pour envoyer le message "mouvement terminé"
	virtual void update(Duration elapsed) override;

	// Erreur "numero du servo trop grand"
	class ErreurNumServo : public std::runtime_error {
	public:
		ErreurNumServo(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " de servo est superieur a " +
		                             to_string((short)NB_MAX_SERVOS)) {}
	};

protected:
	IServosIntelligents& _iServos;
	uint8_t _nbServos;
	static uint8_t const NB_MAX_SERVOS = 4;
};

#endif // CARTE_SERVOS_NOVA_2017_H
