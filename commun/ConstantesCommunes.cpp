// ConstantesCommunes.cpp

#include "ConstantesCommunes.h"

uint16_t const Constantes::TCPIP_PORT_SIMU_DEFAULT = 4321;

int const Constantes::deltaServoPosition = 20;
int const Constantes::deltaEnginePosition = 70;

Duration const Constantes::TIMEOUT_MOVE_DEFAULT = 10_s;
Duration const Constantes::TIMEOUT_OPPONENT_BLOCKING_DEFAULT = 5_s;
Speed const Constantes::LINEAR_SPEED_DEFAULT = 40_cm_s;
AngularSpeed const Constantes::ANGULAR_SPEED_DEFAULT = 5_rad_s; // Unité arbitraire décidée en élec.
Distance const Constantes::LINEAR_PRECISION_DEFAULT = 4_cm;
Angle const Constantes::ANGULAR_PRECISION_DEFAULT = 4_deg;

Speed const Constantes::VITESSE_LINEAIRE_EVITEMENT = 20_cm_s;
AngularSpeed const Constantes::VITESSE_ANGULAIRE_EVITEMENT = 1_rad_s;

Speed const Constantes::VITESSE_LINEAIRE_RECALAGE = 20_cm_s;

Duration const Constantes::MATCH_DURATION = 90_s;

int const Constantes::NOMBRE_BLOCAGE_LOGICIEL_MAX = 5;
int const Constantes::NOMBRE_BLOCAGE_PHYSIQUE_MAX = 5;

Vector2m const Constantes::TABLE_SIZE = {3_m, 2_m};
Vector2u16 const Constantes::TAILLE_GRILLE = {300, 200};

int ConstantesCommunes::getNombreBlocageLogicielMax() const {
	return Constantes::NOMBRE_BLOCAGE_LOGICIEL_MAX;
}

int ConstantesCommunes::getNombreBlocagePhysiqueMax() const {
	return Constantes::NOMBRE_BLOCAGE_PHYSIQUE_MAX;
}

Duration ConstantesCommunes::getMatchDuration() const {
	return Constantes::MATCH_DURATION;
}

Vector2m ConstantesCommunes::getTableSize() const {
	return Constantes::TABLE_SIZE;
}

Vector2u16 ConstantesCommunes::getTailleGrille() const {
	return Constantes::TAILLE_GRILLE;
}
