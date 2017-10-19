// CarteAsservissement2009.h

#ifndef CARTE_ASSERVISSEMENT_2009_H
#define CARTE_ASSERVISSEMENT_2009_H

#include "../simulateur/robot/Carte.h"
#include "IMoteurAsservi.h"

class CarteAsservissement2009 : public Carte {
public:
	enum SensRotation { SENS_HORAIRE = 0, SENS_TRIGO = 1 };

	// Intervalle de temps de l'envoi auto
	static constexpr Duration DELTA_T_ENVOI_AUTO = 50_ms; // en ms

	// Pour le correcteur proportionnel en vitesse :
	static const Angle ASSERVIS_EPSILON;
	static const float ASSERVIS_COEFF_VITESSE;
	static const AngularSpeed ASSERVIS_VITESSE_MAX;

public:
	// Constructeur
	CarteAsservissement2009(unsigned char id, RobotLogic& robot, IMoteurAsservi& iMoteurAsservi);

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envois de messages :
	void envoyerPong();
	void envoyerBloque();
	void envoyerAngle();
	void envoyerDeplacementTermine();
	void envoyerPrecisionAtteinte();
	void envoyerParametre(float f);

	// Actions :
	// redefinit l'angle courrant ou la distance courrante suivant le mode de la carte
	void definirAngle(Angle angle);
	void arreter();
	// positioner le moteur a l'angle 'angle' ou la distance 'angle' modulo 2PI suivant le mode de la carte
	void tourner(Angle angle);
	// positioner le moteur a l'angle 'angle' ou la distance 'angle' suivant le mode de la carte
	void tournerAbsolu(Angle angle, SensRotation sens);
	// asservit la carte en vitesse de rotation a la place de l'angle ou la distance
	void tournerInfini(AngularSpeed vitesse, SensRotation sens);
	void arretUrgence();

	// Mise à jour
	virtual void update(Duration elapsed) override;

private:
	// Moteur à commander
	IMoteurAsservi& _iMoteurAsservi;

	// Valeurs à atteindre
	AngularSpeed _vitesseVoulue;
	Angle _angleVoulu;

	// Types de déplacement, pour le message "COMMANDE" : récupéré aussi de l'IE
	// et pour mémoriser l'ordre en cours
	enum TypeDeplacement {
		DEPLACEMENT_ARRETER = 0,
		DEPLACEMENT_TOURNER = 1,
		DEPLACEMENT_TOURNER_ABSOLU = 2,
		DEPLACEMENT_TOURNER_INFINI = 3,
		DEPLACEMENT_ARRET_URGENCE = 4,
	};

	// Ordre en cours
	TypeDeplacement _ordreEnCours;

	// Différence entre l'angle mesuré sur le moteur et l'angle perçu par l'IA.
	// Angle moteur = Angle IA + Décalage
	Angle _decalageAngle;

	// Asservissement activé ou pas ? Si pas activé, on ne fait rien.
	bool _asservActive;

	// Envoi auto activé ou pas ?
	bool _envoiAutoActive;

	// Indiquent si les messages "déplacement terminé" et "précision atteinte" ont été envoyés ou pas
	bool _deplacementTermineEnvoye;
	bool _precisionAtteinteEnvoye;

	StopWatch _horloge;
};

#endif // CARTE_ASSERVISSEMENT_2009_H
