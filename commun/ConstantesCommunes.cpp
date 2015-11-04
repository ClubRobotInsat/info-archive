// ConstantesCommunes.cpp

#include "ConstantesCommunes.h"

uint16_t const Constantes::TCPIP_PORT_SIMU_DEFAUT = 4321;

int const Constantes::deltaPositionServo = 20;
int const Constantes::deltaPositionMoteur = 70;

Duration const Constantes::TIMEOUT_DEPLACEMENT_DEFAUT = 10_s;
Duration const Constantes::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = 5_s;
Speed const Constantes::VITESSE_LINEAIRE_DEFAUT = 40_cm_s;
AngularSpeed const Constantes::VITESSE_ANGULAIRE_DEFAUT = 5_rad_s; // Unité arbitraire décidée en élec.
Distance const Constantes::PRECISION_LINEAIRE_DEFAUT = 4_cm;
Angle const Constantes::PRECISION_ANGULAIRE_DEFAUT = 4_deg;

Speed const Constantes::VITESSE_LINEAIRE_EVITEMENT = 20_cm_s;
AngularSpeed const Constantes::VITESSE_ANGULAIRE_EVITEMENT = 1_rad_s;

Speed const Constantes::VITESSE_LINEAIRE_RECALAGE = 20_cm_s;

Duration const Constantes::DUREE_MATCH = 90_s;

int const Constantes::NOMBRE_BLOCAGE_LOGICIEL_MAX = 5;
int const Constantes::NOMBRE_BLOCAGE_PHYSIQUE_MAX = 5;

Vector2m const Constantes::TAILLE_TABLE = {3_m, 2_m};
Vector2u16 const Constantes::TAILLE_GRILLE = {300, 200};

int ConstantesCommunes::getNombreBlocageLogicielMax() const {
	return Constantes::NOMBRE_BLOCAGE_LOGICIEL_MAX;
}

int ConstantesCommunes::getNombreBlocagePhysiqueMax() const {
	return Constantes::NOMBRE_BLOCAGE_PHYSIQUE_MAX;
}

Duration ConstantesCommunes::getDureeMatch() const {
	return Constantes::DUREE_MATCH;
}

Vector2m ConstantesCommunes::getTailleTable() const {
	return Constantes::TAILLE_TABLE;
}

Vector2u16 ConstantesCommunes::getTailleGrille() const {
	return Constantes::TAILLE_GRILLE;
}
