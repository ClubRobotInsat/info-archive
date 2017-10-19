// CarteTelemetres2009.h

#ifndef CARTE_TELEMETRES_2009_H
#define CARTE_TELEMETRES_2009_H

#include "../simulateur/Robot/Carte.h"
#include "ITelemetres.h"

class CarteTelemetres2009 : public Carte {
public:
	// Constantes
	static const unsigned char NB_MAX_TELEMETRES;
	static const bool DEFAULT_ENVOI_AUTO;
	static const Duration PERIODE_ENVOI_AUTO; // en secondes

	// Constructeur
	CarteTelemetres2009(unsigned char id, Robot& robot, ITelemetres& iTelemetres, int nbTelemetres = NB_MAX_TELEMETRES);

	// Destructeur
	virtual ~CarteTelemetres2009();

	// Traite les trames envoyées par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoie une trame toutes les 50 ms, lorsque l'envoi automatique est activé
	virtual void update(Duration elapsed) override;

	// erreur le numero du colorimetre est trop grand
	class ErreurNumTelemetre : public std::runtime_error {
	public:
		ErreurNumTelemetre(unsigned char num)
		        : std::runtime_error("le numero de telemetre " + to_string((short)num) + " est superieur a " +
		                             to_string((short)NB_MAX_TELEMETRES)) {}
	};

private:
	ITelemetres& _iTelemetres;

	// Envoi automatique
	bool _envoiAuto;

	Duration _lastTime;
	StopWatch _clock;

	// Envoyer une trame de pong
	void envoyerPong();

	// Envoyer la distance demandée par l'IA
	void envoyerDistance(unsigned char num);

	// Envoyer les distances de tous les télémètres gérés par la carte
	void envoyerToutesDistances();

	// Nombre de télémètres gérés par la carte
	int _nbTelemetres;
};

#endif // CARTE_TELEMETRES_2009_H
