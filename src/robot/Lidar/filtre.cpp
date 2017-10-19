/******************************************************************************/
/* robot/Lidar/filtre.cpp                                          2015-03-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "filtre.h"
/******************************************************************************/

namespace {

	// Coeff pour filtres Kalman (chaque point du scanner est traité comme un
	// problème indépendant)
	const double Q = 0.1; // incertitude globale à chaque pas
	// R fonction linéaire de l'écart avec dernier relevé
	const double H = 1.0; // Je sais pas ce que c'est, H
	const double P_init = 0.1;

	// seuils pour traiter les relevés comme aberrants
	const Distance MIN = 50_mm;
	const Distance MAX = 7_m;
}

const TrameLidar& Filtre::getTrame(const TrameLidar& input) {
	if(!_state.points.size()) { // premier relevé
		// assume que ces valeurs restent constantes
		_state.begin = input.begin;
		_state.end = input.end;
		_state.angularResolution = input.angularResolution;

		_state.points = input.points;
		_variances.resize(input.points.size(), P_init);
	}

	_state.timestamp = input.timestamp;

	for(size_t i = 0; i < _state.points.size(); i++) {
		Distance in = input.points[i];
		Distance& o = _state.points[i];

		// traitement heuristique pour les valeurs aberrantes.
		Distance avg_voisins = (i == 0) ? (input.points[1]) : (i == _state.points.size()) ?
		                                  (input.points[i - 1]) :
		                                  ((input.points[i - 1] + input.points[i + 1]) / 2);
		double diff = in.toM() - avg_voisins.toM();
		// double R = 0.01 + (diff*diff)*0.01;
		double R = 0.004;
		if(in < MIN) {
			R = 0.10;
			in = MAX;
		}

		// Kalman
		// prédiction: pas de commande, on suppose que les points ne bougent pas
		double& p = _variances[i];
		p += Q;

		// MàJ
		double K = p * H / (H * p * H + R);
		o += K * (in - H * o);
		p *= (1 - K * H);
	}

	return _state;
}