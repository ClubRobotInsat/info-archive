// PID.cpp

#include "PID.h"

PID::PID() : _Kp(0.0), _Ki(0.0), _Kd(0.0), _sortiePID(0.0), _erreurPrecedente(0.0), _integraleErreur(0.0) {}

PID::PID(double Kp, double Ki, double Kd)
        : _Kp(Kp), _Ki(Ki), _Kd(Kd), _sortiePID(0.0), _erreurPrecedente(0.0), _integraleErreur(0.0) {}

PID::~PID() {}

// update() : à appeler à chaque frame : met à jour la valeur de sortie
// et la renvoie.
double PID::update(Duration el, double sortieMesuree) {
	double elapsed = el.toS();

	// Calcul de l'erreur
	double erreur = _consigne - sortieMesuree;

	// Calcul de l'intégrale de l'erreur
	_integraleErreur += erreur * elapsed;

	// Calcul de la sortie
	_sortiePID = _Kp * erreur + _Ki * _integraleErreur + _Kd * (erreur - _erreurPrecedente) / elapsed;

	// Sauvegarde de la valeur de l'erreur
	_erreurPrecedente = erreur;

	return _sortiePID;
}
