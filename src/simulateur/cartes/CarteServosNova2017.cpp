// CarteServosNova2017.cpp

#include "CarteServosNova2017.h"

CarteServosNova2017::CarteServosNova2017(unsigned char id, RobotLogic& robot, IServosIntelligents& iServos, uint8_t nbServos)
        : Carte(id, robot), _iServos(iServos), _nbServos(nbServos) {
	if(_nbServos > NB_MAX_SERVOS) {
		throw std::runtime_error("Trop de servos pour la carte !");
	}
}

void CarteServosNova2017::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// Ping / pong :
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				envoyerPong();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Ordre de position (commande en angle) :
		case 0x01_b:
			if(t.getNbDonnees() == 3) {
				auto angle = Angle::makeFromDeg(t.getDonnee<std::uint16_t>(1)) / 10;
				_iServos.setAngle(this->getId(), t.getDonnee(0), angle);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Lecture de l'angle d'un servo :
		case 0x02_b:
			if(t.getNbDonnees() == 1)
				envoyerPosition(t.getDonnee(0));
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// changement vitesse servo :
		case 0x05_b:
			if(t.getNbDonnees() == 2) {
				unsigned char servo = t.getDonnee(0);
				unsigned char vitesse = t.getDonnee(1);
				_iServos.setSpeed(this->getId(), servo, vitesse);
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

void CarteServosNova2017::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	logInfo("ping -> pong");
}

// Envoi de la position d'un servo
void CarteServosNova2017::envoyerPosition(uint8_t numServo) {
	uint16_t angle = (_iServos.getAngle(this->getId(), numServo) * 10).toDeg<uint16_t>();

	Trame aEnvoyer(this->getId(), 0x02_b);
	aEnvoyer.addDonnees(numServo, angle);
	this->envoyer(aEnvoyer);
}

// Envoi de l'information indiquant si le mouvement d'un servo est terminé ou pas
void CarteServosNova2017::envoyerEtatTermine(uint8_t numServo, bool termine) {
	Trame aEnvoyer(this->getId(), 0x03_b);
	aEnvoyer.addDonnees(numServo, termine);
	this->envoyer(aEnvoyer);
}

// Envoi de l'état du servo indiqué (bloqué ou pas)
void CarteServosNova2017::envoyerServoBloque(uint8_t numServo, bool bloque) {
	Trame aEnvoyer(this->getId(), 0x04_b);
	aEnvoyer.addDonnees(numServo, bloque);
	this->envoyer(aEnvoyer);

	// logInfo("servo ", (int)numServo," de la carte ", (int)this->getId(), " est bloque");
}

// Mise à jour de la carte : pour envoyer le message "mouvement terminé"
void CarteServosNova2017::update(Duration elapsed) {
	Carte::update(elapsed);

	for(uint8_t numServo = 0; numServo < _nbServos; ++numServo) {
		if(_iServos.isMovementCompleted(this->getId(), numServo))
			envoyerEtatTermine(numServo, true);

		if(_iServos.isBlocked(this->getId(), numServo))
			envoyerServoBloque(numServo, true);
	}
}
