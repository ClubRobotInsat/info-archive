/************************************************************
 * CarteDeplacement2007.h *
 * Spécifie la classe gérant le déplacement 2006/2007 *
 ************************************************************/

#ifndef _CARTE_DEPLACEMENT_2007_H_
#define _CARTE_DEPLACEMENT_2007_H_

#include "Carte.h"
#include "CarteDeplacementCommun.h"


class CarteDeplacement2007 : public Carte {
public:
	enum TypeAck : uint8_t { ACK_RIEN, ACK_MESSAGE_EXECUTE, ACK_MESSAGE_IGNORE };

	enum TypeDeplacement : uint8_t {
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
		DEPLACEMENT_CONTROLE_TRAJECTOIRE
	};

	enum TypeMesure : uint8_t { MESURE_DISTANCE = 0, MESURE_ANGLE = 1, MESURE_DROITE = 2, MESURE_GAUCHE = 3 };

	enum TypeParam : uint8_t { PID = 0, CINETIQUE = 1, PRECISION = 2, MECANIQUE = 3 };

	enum TypeGrandeur : uint8_t {
		POSITION = 'L',
		ANGLE = 'A',
		DROITE = 'D',
		GAUCHE = 'G',
		VPOSITION = 'l',
		VANGLE = 'a'
	};

	enum TypeMeca : uint8_t { ENTRE_AXE = 'R', DIAMETRE_D = 'D', DIAMETRE_G = 'G' };

	enum TypeCinetique : uint8_t { VITESSE = 'v', ACCELERATION = 'a' };

	CarteDeplacement2007(CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Actualise les coordonnées du robot
	void actualiserCoordonnees();

	// Vérifie que les coordonnées du robot soient actualisées
	bool coordonneesActualisees();

	// Retourne les coordonnées du robot
	CoordonneesOld lireCoordonnees();

	// Definit les coordonnees actuels du robot
	void definirCoordonnees(std::int16_t x, std::int16_t y, std::int16_t a);

	// Indique si le déplacement est terminé ou pas
	bool verifierDeplacementTermine();

	// Indique si le robot est bloqué ou pas.
	bool verifierBlocage();

	// S'arrêter aux coordonnées actuelles
	void arreter();

	// Avancer de la distance donnée (mm)
	void avancer(std::int16_t distance);

	// Tourner de l'angle donné (mrad)
	void tourner(std::int16_t angle);

	// Tourner jusqu'à avoir l'angle absolu donné (mrad)
	void tournerAbsolu(std::int16_t theta, SensRotation sens);

	// Se tourner vers le point donné (mm, mm)
	void pointerVers(std::int16_t x, std::int16_t y, SensRotation sens);

	// Se rend au point donné (mm, mm) par une composition de rotation et d'avance, imprécis => déprécié
	void vaVers(std::int16_t x, std::int16_t y, SensAvance sens);

	// Se rendre au point donné (mm, mm) par une composition de rotation et d'avance
	void allerADecompose(std::int16_t x, std::int16_t y, SensAvance sens);

	// Se rendre au point donné (mm, mm) par une trajectoire lissée
	void allerA(std::int16_t x, std::int16_t y, SensAvance sens);

	// Avancer à vitesse constante jusqu'à nouvel ordre
	void avancerInfini(SensAvance sens);

	// Tourner à vitesse constante jusqu'à nouvel ordre
	void tournerInfini(SensRotation sens);

	// Se rendre au point donné (mm, mm) par une trajectoire lissée sans s'y arrêter
	void passerPar(std::int16_t x, std::int16_t y, SensAvance sens);

	// Pivote de l'angle donné (mrad) autour de la roue droite
	void pivoterDroite(std::int16_t angle, SensRotation sens);

	// Pivote de l'angle donné (mrad) autour de la roue gauche
	void pivoterGauche(std::int16_t angle, SensRotation sens);

	// Fait avancer chacune des 2 roues d'une distance donnée (mm, mm)
	void commanderRoues(std::int16_t droite, std::int16_t gauche);

	// Changer la vitesse individuelle des roue quand on les commandes individuellement
	void commanderVitesseRoues(SensAvance sens);

	// Arret d'urgence du robot
	void arretUrgence(void);

	// Permet de contrôler précisement la trajectoire du robot
	void controleTrajectoire(std::int16_t distance, std::int16_t angle, SensAvance sens);

	// Permet de contrôler en direct la vitesse longitudinale et angulaire du robot
	void controleVitesseLongAng(std::int16_t vitesse_longitudinale, std::int16_t vitesse_angulaire);

	// Actualise les données de debug demandées
	void actualiserMesure(TypeMesure parametres);

	// Vérifie que les données de debug soient actualisées
	bool mesureActualisee();

	// Lit une donnée de debug
	std::int16_t lireMesure(uint8_t donnee);

	// Allume ou éteint la LED témoin
	void reglerTemoin(bool led1, bool led2);

	// Regle les coefficients des correcteurs PID
	void reglerPID(TypeGrandeur type, std::int16_t kp, std::int16_t ki, std::int16_t kd);

	// Permet d'activer ou de désactiver certains types d'asservissement (distance, angle, roue droite, roue gauche)
	void activerAsservissement(TypeGrandeur type, bool active);

	// Regle la vitesse_max ou l'acceleration_max du robot
	void reglerCinetique(TypeGrandeur type, TypeCinetique cinetique, std::int16_t valeur);

	// Regle la precision des mouvements
	void reglerPrecision(TypeGrandeur type, std::int16_t precision);

	// Regle quelques grandeurs mécaniques (entre_axe, diametre des roues)
	void reglerMecanique(TypeMeca type, double valeur);

	// Vérifie qu'un ack ait été reçu
	bool ackRecu();

	void activerEnvoiAuto();

	void desactiverEnvoiAuto();

private:
	CoordonneesOld _coords;
	std::atomic_bool _coordsOk;

	std::atomic_bool _bloque;
	TypeDeplacement _deplacement;
	std::atomic_bool _deplacementTermine;
	std::atomic<std::int16_t> _debug[3];
	std::atomic_bool _debugOk;

	TypeAck _ack;
};

#endif
