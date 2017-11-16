// Asservissement.h

#ifndef ASSERVISSEMENT_H
#define ASSERVISSEMENT_H

#include "Commun.h"
#include "PID.h"

// Classe sur laquelle s'appuient beaucoup des actions possibles par le robot ; implémente l'asservissement en vitesses
// longitudinale et angulaire.
class Asservissement {
public:
	Asservissement();
	virtual ~Asservissement();

	void setVdMax(double val) {
		this->VITESSE_MAX = val;
	}
	void setAdMax(double val) {
		this->ACCELERATION_MAX = val;
	}
	void setVaMax(double val) {
		this->VITESSE_ANGULAIRE_MAX = val;
	}
	void setAaMax(double val) {
		this->ACCELERATION_ANGULAIRE_MAX = val;
	}

	double getVdMax() {
		return VITESSE_MAX;
	}
	double getAdMax() {
		return ACCELERATION_MAX;
	}
	double getVaMax() {
		return VITESSE_ANGULAIRE_MAX;
	}
	double getAaMax() {
		return ACCELERATION_ANGULAIRE_MAX;
	}

	PID& getPIDPositionVersVitesse() {
		return pidPositionVersVitesse;
	}
	PID& getPIDAngleVersVitesseAngulaire() {
		return pidAngleVersVitesseAngulaire;
	}

	// Entrée: intervalle de temps entre 2 frames (elapsed), distance à parcourir (consigne), angle duquel on doit
	// tourner (consigne),
	// distances parcourues par les roues gauche et droite.
	// Sorties : forces à appliquer à la roue gauche et à la roue droite du robot.
	void calculer(Duration elapsed,
	              Distance distanceAParcourir,
	              Angle angleAEffectuer,
	              Distance distanceTotaleParcourue,
	              Angle angleBrut,
	              Speed* vitesseLongitudinale,
	              AngularSpeed* vitesseAngulaire);

	void resetPIDs();

private:
	double VITESSE_MAX;                // dm/s
	double ACCELERATION_MAX;           // dm/s^2
	double VITESSE_ANGULAIRE_MAX;      // rad/s
	double ACCELERATION_ANGULAIRE_MAX; // rad/s^2

	double _commandeVitesseLongitudinalePrecedente;
	double _commandeVitesseAngulairePrecedente;

	// Fonction utilitaire de seuillage
	static double clamp(double valeur, double valeurMax, double valeurMin);

protected:
	// Les 4 PIDs
	PID pidPositionVersVitesse, pidAngleVersVitesseAngulaire;
	PID pidVitesseVersForce, pidVitesseAngulaireVersForce;
};

#endif // ASSERVISSEMENT_H
