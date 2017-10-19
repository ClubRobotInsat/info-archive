#ifndef CARTETELEMETREIR2008_H_
#define CARTETELEMETREIR2008_H_

#include "../simulateur/Robot/Carte.h"
#include "ITelemetres.h"

class CarteTelemetreIR2008 : public Carte {
public:
	static const unsigned char NBR_TELEMETRES;
	static const unsigned char TELEMETRE_IR_DEVANT_MILIEU;

public:
	CarteTelemetreIR2008(unsigned char id, Robot& robot, ITelemetres& iTelemetres);

	virtual ~CarteTelemetreIR2008();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoie une trame toutes les 200 ms
	virtual void update(Duration elapsed) override;

	// erreur le numero du telemetre est trop grand
	class ErreurNumTelemetre : public std::runtime_error {
	public:
		ErreurNumTelemetre(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " + to_string((short)NBR_TELEMETRES)) {}
	};

private:
	ITelemetres& _iTelemetres;

	// envoyer une trame de pong
	void envoyerPong();

	// Envoi une mesure
	void envoyerMesureDistance(unsigned char num);
};

#endif /*CARTECOLORIMETRES2008_H_*/
