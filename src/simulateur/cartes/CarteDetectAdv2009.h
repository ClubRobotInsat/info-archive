// CarteDetectAdv2009.h - carte commandant le moteur qui fait tourner le
// rayon laser sur le robot pour détecter l'adversaire

#ifndef CARTE_DETECT_ADV_2009_H
#define CARTE_DETECT_ADV_2009_H

#include "../simulateur/robot/Carte.h"
#include "ICoordonneesAdversaire.h"
#include "IDeplacement.h"

class CarteDetectAdv2009 : public Carte {
public:
	// Constantes
	static const bool DEFAULT_ENVOI_AUTO;
	static const Duration PERIODE_ENVOI_AUTO;
	static const Distance DISTANCE_MAX; // Distance à partir de laquelle on ne voit rien

	// Constructeur
	CarteDetectAdv2009(unsigned char id, RobotLogic& robot, IDeplacement& iDeplacement, ICoordonneesAdversaire& iCoordsAdv);

	// Destructeur
	virtual ~CarteDetectAdv2009();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoi de la distance et de l'angle de l'adversaire toutes les 200 ms, sous
	// certaines conditions
	virtual void update(Duration elapsed) override;

private:
	IDeplacement& _iDeplacement;
	ICoordonneesAdversaire& _iCoordsAdv;

	// Indique si l'envoi automatique est activé
	bool _envoiAuto;

	// Indique si l'IA a appelé un DefinirAngle() (i.e. pour pas que l'on renvoie n'importe quoi,
	// il faut que l'angle ait été défini)
	bool _angleDefini;

	// Indique si l'on a demandé au moteur de tourner indéfiniment
	bool _tourneInfini;

	Duration _lastTime;

	StopWatch _clock;

	// Envoyer une trame de pong
	void envoyerPong();

	// Envoi de la distance et de l'angle de l'adversaire
	void envoyerDistAngleAdv();
};

#endif // CARTE_DETECT_ADV_2009_H
