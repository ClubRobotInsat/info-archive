//
// Created by terae on 4/20/18.
//

#include "Moteur.h"
#include "../librobot/libRobot.h"

namespace {
	StrategiePrincipal* _strategie;

	MecaManagerPrincipal& meca() {
		return _strategie->getMecaManager();
	}
}

void initMoteurPetri(StrategiePrincipal& strategie) {
	_strategie = &strategie;
}

ResultatAction monterAscenseursDe(int nbr_tours) {
	return meca().monterAscenseursDe(nbr_tours);
}

ResultatAction monterAscenseursDe(Angle value) {
	return meca().monterAscenseursDe(value);
}

ResultatAction activerAvaleurs(SensAvaleurs sens) {
	return meca().activerAvaleurs(sens);
}

ResultatAction desactiverAvaleurs() {
	return meca().desactiverAvaleurs();
}
