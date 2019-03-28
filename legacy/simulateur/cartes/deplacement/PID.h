// PID.h - un correcteur Proportionnel Intégral Dérivateur !

#ifndef PID_H
#define PID_H

#include <Commun.h>

class PID {
public:
	PID();
	PID(double Kp, double Ki, double Kd);
	virtual ~PID();

	// Setters

	void setCoefficients(double Kp, double Ki, double Kd) {
		_Kp = Kp;
		_Ki = Ki;
		_Kd = Kd;
	}

	void setKp(double Kp) {
		_Kp = Kp;
	}
	void setKi(double Ki) {
		_Ki = Ki;
	}
	void setKd(double Kd) {
		_Kd = Kd;
	}

	void setConsigne(double consigne) {
		_consigne = consigne;
	}

	// Getters
	double getKp() {
		return _Kp;
	}
	double getKi() {
		return _Ki;
	}
	double getKd() {
		return _Kd;
	}

	// update() : à appeler à chaque frame : met à jour la valeur de sortie
	// et la renvoie.
	double update(Duration elapsed, double sortieMesuree);

	// Renvoie la valeur courante (qui change entre 2 appels à update()) de la sortie.
	double getSortie() {
		return _sortiePID;
	}

private:
	double _Kp, _Ki, _Kd; // Coefficients proportionnel, intégral et dérivateur
	double _sortiePID;
	double _erreurPrecedente;
	double _integraleErreur;
	double _consigne;
};

#endif // PID_H
