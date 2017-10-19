// Asservissement.cpp

#include "Asservissement.h"
#include "../../PhysicalToSimulationUnits.h"
#include <cmath>
#include <iostream>

using std::abs;

Asservissement::Asservissement()
        : VITESSE_MAX(10), ACCELERATION_MAX(40), VITESSE_ANGULAIRE_MAX(6.5), ACCELERATION_ANGULAIRE_MAX(30) {
	_commandeVitesseLongitudinalePrecedente = 0.0;
	_commandeVitesseAngulairePrecedente = 0.0;

	pidPositionVersVitesse.setKp(7.5);
	pidAngleVersVitesseAngulaire.setKp(9.0);
}

Asservissement::~Asservissement() {}

// Fonction utilitaire de seuillage
double Asservissement::clamp(double valeur, double valeurMax, double valeurMin) {
	return valeur > valeurMax ? valeurMax : (valeur < valeurMin ? valeurMin : valeur);
}

// Entr?e: intervalle de temps entre 2 frames (elapsed), distance ? parcourir (consigne), angle duquel on doit tourner
// (consigne),
// distances parcourues par les roues gauche et droite.
// Sorties : forces ? appliquer ? la roue gauche et ? la roue droite du robot.
void Asservissement::calculer(Duration el,
                              Distance distanceAParcourir,
                              Angle angleAEffectuer,
                              Distance distanceTotaleParcourue,
                              Angle angleBrut,
                              Speed* vitesseLongitudinale,
                              AngularSpeed* vitesseAngulaire) {
	double elapsed = el.toS();
	// Calcul de la vitesse longitudinale de consigne
	// double erreurDistance = (distanceAParcourir - distanceTotaleParcourue)/10.0;
	// if(erreurDistance < 0.05)
	// pidPositionVersVitesse.setKp(14.44);
	// else if(erreurDistance < 0.4 && erreurDistance >= 0.05)
	// pidPositionVersVitesse.setKp(3.7777 * exp(-0.4537*log(erreurDistance)));
	// else
	pidPositionVersVitesse.setKp(7.5);
	pidPositionVersVitesse.setConsigne(distanceAParcourir.toDm());
	pidPositionVersVitesse.update(el, distanceTotaleParcourue.toDm());

	// Calcul de la vitesse angulaire de consigne
	// double erreurAngle = angleAEffectuer - angleBrut;
	// if(erreurAngle < 0.42)
	// pidAngleVersVitesseAngulaire.setKp(15.0);
	// else if(erreurAngle < 1.0 && erreurAngle >= 0.42)
	// pidAngleVersVitesseAngulaire.setKp(6.0 * exp(-0.4114*log(erreurAngle)));
	// else
	pidAngleVersVitesseAngulaire.setKp(9.0);
	pidAngleVersVitesseAngulaire.setConsigne(angleAEffectuer.toRad());
	pidAngleVersVitesseAngulaire.update(el, angleBrut.toRad());

	double commandeVitesseLongitudinale = clamp(pidPositionVersVitesse.getSortie(), VITESSE_MAX, -VITESSE_MAX);

	double commandeVitesseAngulaire = clamp(pidAngleVersVitesseAngulaire.getSortie(), VITESSE_ANGULAIRE_MAX, -VITESSE_ANGULAIRE_MAX);

	if((abs(commandeVitesseLongitudinale - _commandeVitesseLongitudinalePrecedente)) / elapsed > ACCELERATION_MAX) {
		if((commandeVitesseLongitudinale - _commandeVitesseLongitudinalePrecedente) / elapsed > 0.0)
			commandeVitesseLongitudinale = _commandeVitesseLongitudinalePrecedente + ACCELERATION_MAX * elapsed;
		else
			commandeVitesseLongitudinale = _commandeVitesseLongitudinalePrecedente - ACCELERATION_MAX * elapsed;
	}

	if((abs(commandeVitesseAngulaire - _commandeVitesseAngulairePrecedente)) / elapsed > ACCELERATION_ANGULAIRE_MAX) {
		if((commandeVitesseAngulaire - _commandeVitesseAngulairePrecedente) / elapsed > 0.0)
			commandeVitesseAngulaire = _commandeVitesseAngulairePrecedente + ACCELERATION_ANGULAIRE_MAX * elapsed;
		else
			commandeVitesseAngulaire = _commandeVitesseAngulairePrecedente - ACCELERATION_ANGULAIRE_MAX * elapsed;
	}

	*vitesseLongitudinale = fromSimulationVL(commandeVitesseLongitudinale);
	*vitesseAngulaire = fromSimulationVA(commandeVitesseAngulaire);

	_commandeVitesseAngulairePrecedente = commandeVitesseAngulaire;
	_commandeVitesseLongitudinalePrecedente = commandeVitesseLongitudinale;
}

void Asservissement::resetPIDs() {
	pidPositionVersVitesse = PID();
	pidAngleVersVitesseAngulaire = PID();
	pidVitesseVersForce = PID();
	pidVitesseAngulaireVersForce = PID();

	pidPositionVersVitesse.setKp(7.5);
	pidAngleVersVitesseAngulaire.setKp(9.0);

	_commandeVitesseAngulairePrecedente = 0.0;
	_commandeVitesseLongitudinalePrecedente = 0.0;
}
