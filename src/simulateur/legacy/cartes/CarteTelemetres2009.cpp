// CarteTelemetres2009.cpp

#include "CarteTelemetres2009.h"
#include "../simulateur/GLHeaders.h"

const unsigned char CarteTelemetres2009::NB_MAX_TELEMETRES = 3;
const bool CarteTelemetres2009::DEFAULT_ENVOI_AUTO = false;
const Duration CarteTelemetres2009::PERIODE_ENVOI_AUTO = 50_ms;

// Constructeur
CarteTelemetres2009::CarteTelemetres2009(unsigned char id, Robot& robot, ITelemetres& iTelemetres, int nbTelemetres)
        : Carte(id, robot), _iTelemetres(iTelemetres), _envoiAuto(DEFAULT_ENVOI_AUTO), _nbTelemetres(nbTelemetres) {
	_lastTime = 0.0_s;
}

// Destructeur
CarteTelemetres2009::~CarteTelemetres2009() {}

// Envoyer une trame de pong
void CarteTelemetres2009::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	logInfo("pong");
}

// Envoyer la distance demandée par l'IA
void CarteTelemetres2009::envoyerDistance(uint8_t num) {
	// On vérifie que le numéro du télémètre n'est pas trop grand
	if(num >= NB_MAX_TELEMETRES)
		throw ErreurNumTelemetre(num);

	// Récupération de la distance et conversion
	auto distance = _iTelemetres.getTelemetresDistance(getId(), num);

	// Envoi de la trame
	Trame t(this->getId(), 0x01_b);
	t.addDonnees(num, distance.toMm<uint16_t>());
	this->envoyer(t);
}

// Envoyer les distances de tous les télémètres gérés par la carte
void CarteTelemetres2009::envoyerToutesDistances() {
	// Construction de la trame
	Trame t(this->getId(), 0x02_b);

	for(int i = 0; i < 3; i++) {
		if(i < _nbTelemetres) {
			// Récupération de la distance et conversion
			auto distance = _iTelemetres.getTelemetresDistance(getId(), i);

			// Ajout de la distance à la trame
			t.addDonnees(distance.toMm<uint16_t>());
		} else
			t.addDonnees(uint16_t(0));
	}

	// Envoi de la trame
	this->envoyer(t);
}

// Traite les trames envoyées par l'IA
void CarteTelemetres2009::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// Ping/pong
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				envoyerPong();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Demande de la distance d'un télémètre
		case 0x01_b:
			if(t.getNbDonnees() == 1)
				envoyerDistance(t.getDonnee(0));
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Activation/désactivation envoi auto
		case 0x03_b:
			if(t.getNbDonnees() == 1)
				_envoiAuto = (bool)(t.getDonnee(0));
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Envoie une trame toutes les 50 ms, lorsque l'envoi automatique est activé
void CarteTelemetres2009::update(Duration elapsed) {
	Carte::update(elapsed);

	auto now = _clock.getElapsedTime();

	if(now - _lastTime >= PERIODE_ENVOI_AUTO) {
		_lastTime = now;

		if(_envoiAuto)
			envoyerToutesDistances();
	}
}
