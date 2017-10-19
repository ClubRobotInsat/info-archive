// CarteDeplacement2007.h

#ifndef CARTEDEPLACEMENT2007_H_
#define CARTEDEPLACEMENT2007_H_

#include "../simulateur/Robot/Carte.h"
#include "IDeplacement.h"
#include "deplacement/Asservissement.h"

class CarteDeplacement2007 : public Carte {
private:
	static const int TEMPS_BLOQUAGE_MAX = 25;
	static constexpr btScalar FACTEUR_VITESSE_LONGITUDINALE = 0.04;
	static constexpr btScalar FACTEUR_VITESSE_ANGULAIRE = 0.002;

public:
	// Constantes
	static constexpr btScalar DISTANCE_INFINIE = 100000000.0;
	static constexpr btScalar ANGLE_INFINI = 100000000.0;

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

	CarteDeplacement2007(unsigned char id, Robot& robot, IDeplacement& depla);

	virtual ~CarteDeplacement2007();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Fonction appelée toutes les frames
	virtual void update(double elapsed);

private:
	// Variables membres
	IDeplacement& _depla; // Pointeur vers l'interface IDeplacement, implémentée par le robot

	Asservissement _asservissement; // Instance de la classe qui implémente l'asservissement de la carte

	TypeDeplacement _typeDeplacement; // Type de déplacement actuellement effectué

	bool _deplacementTermine; // Traduit le fait qu'un déplacement est en cours, ou est terminé

	bool _deplacementTermineEnvoye;

	int _tempsBloquage;

	short _etapeDeplacement; // Indique à quelle étape du mouvement se trouve le robot au cours d'un déplacement

	bool _asservissementDistanceActive;
	bool _asservissementAngleActive;

	btScalar _distanceTotaleParcourue; // Distance longitudinale parcourue par le robot (dm)

	btScalar _angleBrut; // Angle du robot (non modulo 2*PI) (rad)

	Sens _sensDeplacement;

	btScalar _distanceDemandee; // Consigne relative au mouvement, pas forcement utilisé directement si le mouvement est
	                            // en plusieurs étapes

	btScalar _angleDemande; // Consigne relative au mouvement, pas forcement utilisé directement si le mouvement est en
	                        // plusieurs étapes

	btScalar _distanceAParcourir; // Distance que l'on a à parcourir (dm)

	btScalar _angleAAtteindre; // Angle que l'on veut atteindre (rad)

	btScalar _xConsigne; // X que l'on veut atteindre (dm)

	btScalar _yConsigne; // Y que l'on veut atteindre (dm)

	btScalar _precisionLongitudinale; // precision sur X, Y, d é atteindre pour déclarer la consigne comme atteinte (dm)

	btScalar _precisionAngulaire; // precision sur l'angle é atteindre pour déclarer la consigne comme atteinte (rad)

	btScalar _xPositionPrecedente, _yPositionPrecedente;

	btScalar _anglePrecedent, _vitesseLongitudinale, _vitesseAngulaire;

	btScalar _xSimuToRobot, _ySimuToRobot, _thetaSimuToRobot; // en milimetre

	// Tous les messages que la carte peut envoyer à l'IA

	void pong(); // réponse à un ping

	void envoyerMessageIgnore();

	void envoyerMessageExecute();

	void bloque();

	void envoyerCoordonnees(); // envoi des infos sur les coordonnées actuelles du robot

	void envoyerMesurePosition();

	void envoyerMesureAngle();

	void envoyerDeplacementTermine();

	// Toutes les actions effectuées par la carte ; ces fonctions sont appelées par traiterTrame().

	void arreter();

	void avancer(short distance);

	void tourner(short angle);

	void tournerAbsolu(short theta, Sens sens);

	void pointerVers(short x, short y, Sens sens);

	void allerVers(short x, short y, Sens sens);

	void allerADecompose(short x, short y, Sens sens);

	void allerA(short x, short y, Sens sens);

	void avancerInfini(Sens sens);

	void tournerInfini(Sens sens);

	void passerPar(short x, short y, Sens sens);

	void pivoterDroite(short angle, Sens sens);

	void pivoterGauche(short angle, Sens sens);

	void commanderRoues(short droite, short gauche);

	void reglerTemoin(bool led1, bool led2);

	void arretUrgence();

	void controleTrajectoire(short distance, short angle, Sens sens);

	void controleVitesseLongAng(short vitesseLongitudinale, short);

	btScalar mod2Pi(btScalar angle);

	bool estDansIntervalle(btScalar val, btScalar min, btScalar max);

	bool estImmobile();

	// fonction qui sont par rapport au repere du robot
	virtual btScalar getPositionX() const;                          // en deciMetre
	virtual btScalar getPositionY() const;                          // en deciMetre
	virtual btScalar getAngle2d() const;                            // en miliRadian
	virtual void setRepere(btScalar x, btScalar y, btScalar theta); // en decimètres et en milliradians
};

#endif /*CARTEDEPLACEMENT2007_H_*/
