// CarteDeplacement2009.h

#ifndef CARTEDEPLACEMENT2009_H_
#define CARTEDEPLACEMENT2009_H_

#include "../simulateur/robot/Carte.h"
#include "IDeplacement.h"
#include "deplacement/Asservissement.h"

// ID des messages pour la carte de déplacement 2009

enum {
	PING = 0,
	ACK = 1,
	SET_VARIABLE = 2,
	DEFINIR_COORD = 3,
	COMMANDE = 4,
	ARRETURGENCE = 5,
	ASSERV_ON_OFF = 6,
	ENVOI_AUTO = 7,
	LED = 8,
	BLOQUE = 9,
	COORD = 10,
	TERMINE = 11,
	MESURE = 12,
	GET_VARIABLE = 13,
	RESET = 15
};


class CarteDeplacement2009 : public Carte {
private:
	static constexpr int NOMBRE_BLOQUAGE_MAX = 25;
	static constexpr float FACTEUR_VITESSE_LONGITUDINALE = 1;
	static constexpr float FACTEUR_VITESSE_ANGULAIRE = 1;

public:
	// Constantes
	static constexpr Distance DISTANCE_INFINIE = 100000000.0_mm;
	static constexpr Distance ANGLE_INFINI = 100000000.0_mm;

	// Sous-types
	enum TypeAck { ACK_RIEN, ACK_MESSAGE_EXECUTE, ACK_MESSAGE_IGNORE };

	enum TypeDeplacement {
		DEPLACEMENT_RIEN,
		DEPLACEMENT_ARRETER,
		DEPLACEMENT_AVANCER,
		DEPLACEMENT_TOURNER,
		DEPLACEMENT_TOURNER_ABSOLU,
		DEPLACEMENT_POINTER_VERS,
		DEPLACEMENT_ALLER_VERS,
		DEPLACEMENT_ALLER_A_DECOMPOSE,
		DEPLACEMENT_ALLER_A,
		DEPLACEMENT_AVANCER_INFINI,
		DEPLACEMENT_TOURNER_INFINI,
		DEPLACEMENT_PASSER_PAR,
		DEPLACEMENT_PIVOTER_DROITE,
		DEPLACEMENT_PIVOTER_GAUCHE,
		DEPLACEMENT_COMMANDER_ROUES,
		DEPLACEMENT_ARRET_URGENCE,
		DEPLACEMENT_CONTROLE_TRAJECTOIRE,
		DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG
	};

	enum Sens {
		SENS_ARRIERE = 0,
		SENS_AVANT = 1,

		SENS_HORAIRE = 0,
		SENS_TRIGO = 1
	};

	enum type_mesure { MESURE_DISTANCE = 0, MESURE_ANGLE = 1, MESURE_DROITE = 2, MESURE_GAUCHE = 3 };

	enum type_param { PID = 0, CINETIQUE = 1, PRECISION = 2, MECANIQUE = 3 };

	enum type_grandeur { POSITION = 'L', ANGLE = 'A', DROITE = 'D', GAUCHE = 'G', VPOSITION = 'l', VANGLE = 'a' };

	enum type_cinetique { VITESSE = 'v', ACCELERATION = 'a' };

	CarteDeplacement2009(unsigned char id, RobotLogic& robot, IDeplacement& depla, Distance hauteurTable);

	virtual ~CarteDeplacement2009() = default;

	void reset();

	// Traite les donnés fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Fonction appelée toutes les frames
	virtual void update(Duration elapsed) override;

	virtual Angle getAngle2d() const;
	virtual void setRepere(Distance x, Distance y, Angle theta);
	virtual Vector2m getPosition() const;

private:
	IDeplacement& _depla; // Pointeur vers l'interface IDeplacement, implémentée par le robot

	Asservissement _asservissement; // Instance de la classe qui implémente l'asservissement de la carte

	TypeDeplacement _typeDeplacement; // Type de déplacement actuellement effectué

	bool _deplacementTermine; // Vrai si precisionAtteinte est vrai et si le robot est arrêté

	int _deplacementTermineEnvoye; // 0 : rien envoyé, 1 : precision atteinte envoyé, 2 : deplacement termine envoyé

	int _tempsBloquage;

	short _etapeDeplacement; // Indique à quelle étape du mouvement se trouve le robot au cours d'un déplacement

	bool _asservissementDistanceActive;
	bool _asservissementAngleActive;

	union _VitesseConsigne { // Utilisé pour les déplacements à vitesse constante (avancerinfini, tournerinfini, passer
		// par)
		Speed lineaire;
		AngularSpeed angulaire;
		_VitesseConsigne() : lineaire(0_m_s) {}
	} _vitesseConsigne;

	Distance _hauteurTable;

	Distance _distanceTotaleParcourue; // Distance longitudinale parcourue par le robot

	Angle _angleBrut; // Angle du robot (non modulo 2*PI)

	Sens _sensDeplacement;

	Distance _distanceDemandee; // Consigne relative au mouvement, pas forcement utilisé directement si le
	// mouvement est
	// en plusieurs étapes

	Angle _angleDemande; // Consigne relative au mouvement, pas forcement utilisé directement si le mouvement est
	// en
	// plusieurs étapes

	Distance _distanceAParcourir; // Distance que l'on a à parcourir

	Angle _angleAAtteindre; // Angle que l'on veut atteindre

	Distance _xConsigne; // X que l'on veut atteindre

	Distance _yConsigne; // Y que l'on veut atteindre

	Distance _precisionLongitudinale; // précision sur X, Y, à atteindre pour déclarer la consigne comme atteinte

	Angle _precisionAngulaire; // précision sur l'angle à atteindre pour déclarer la consigne comme atteinte

	Distance _xPositionPrecedente, _yPositionPrecedente;

	Angle _anglePrecedent;

	Speed _vitesseLongitudinale;
	AngularSpeed _vitesseAngulaire;

	Distance _dxOrigineTable = 0_mm, _dyOrigineTable = 0_mm;
	Angle _dthetaOrigineTable = 0_rad;

	Duration _tempsDernierEnvoi;
	Duration _tempsActuel;

	// Tous les messages que la carte peut envoyer à l'IA

	void pong(); // réponse à un ping

	void envoyerMessageIgnore();

	void envoyerMessageExecute();

	void bloque();

	void envoyerCoordonnees(); // envoi des infos sur les coordonnées actuelles du robot

	void envoyerMesurePosition();

	void envoyerMesureAngle();

	void envoyerVariable(int numero);

	void envoyerPrecisionAtteinte();

	void envoyerDeplacementTermine();

	// Toutes les actions effectuées par la carte ; ces fonctions sont appelées par traiterTrame().

	void arreter();

	void avancer(Distance distance);

	void tourner(Angle angle);

	void tournerAbsolu(Angle theta, Sens sens);

	void pointerVers(Distance x, Distance y, Sens sens);

	void allerVers(Distance x, Distance y, Sens sens);

	void allerADecompose(Distance x, Distance y, Sens sens);

	void allerA(Distance x, Distance y, Sens sens);

	void avancerInfini(Speed vitesse, Sens sens);

	void tournerInfini(AngularSpeed vitesse, Sens sens);

	void passerPar(Distance x, Distance y, Speed vitesse, Sens sens);

	void pivoterDroite(Angle angle, Sens sens);

	void pivoterGauche(Angle angle, Sens sens);

	void commanderRoues(Distance droite, Distance gauche);

	void reglerTemoin(bool led1, bool led2);

	void arretUrgence();

	void controleTrajectoire(Distance distance, Angle angle, Sens sens);

	void controleVitesseLongAng(Speed vitesseLongitudinale, AngularSpeed vitesseAngulaire);

	bool estImmobile();

	// DEBUG

	// TimePoint _tempo = TimePoint::now();

	// TypeDeplacement _oldTypeDepla = DEPLACEMENT_RIEN;
};

#endif /*CARTEDEPLACEMENT2009_H_*/
