// ConstantesCommunes2015.cpp

#include "ConstantesCommunes2015.h"

uint16_t const Constantes2015::TCPIP_PORT_SIMU_DEFAUT = 4321;

int const Constantes2015::deltaPositionServo = 20;
int const Constantes2015::deltaPositionMoteur = 70;

Duree const Constantes2015::TIMEOUT_DEPLACEMENT_DEFAUT = 10_s;
Duree const Constantes2015::TIMEOUT_BLOCAGE_ADVERSAIRE_DEFAUT = 5_s;
VitesseLineaire const Constantes2015::VITESSE_LINEAIRE_DEFAUT = 40_cm_s;
VitesseAngulaire const Constantes2015::VITESSE_ANGULAIRE_DEFAUT = 5_rad_s; // Unité arbitraire décidée en élec.
Distance const Constantes2015::PRECISION_LINEAIRE_DEFAUT = 4_cm;
Angle const Constantes2015::PRECISION_ANGULAIRE_DEFAUT = 4_deg;

VitesseLineaire const Constantes2015::VITESSE_LINEAIRE_EVITEMENT = 20_cm_s;
VitesseAngulaire const Constantes2015::VITESSE_ANGULAIRE_EVITEMENT = 1_rad_s;

VitesseLineaire const Constantes2015::VITESSE_LINEAIRE_RECALAGE = 20_cm_s;

Duree const Constantes2015::DUREE_MATCH = 90_s;

int const Constantes2015::NOMBRE_BLOCAGE_LOGICIEL_MAX = 5;
int const Constantes2015::NOMBRE_BLOCAGE_PHYSIQUE_MAX = 5;

Vector2m const Constantes2015::TAILLE_TABLE = {3_m, 2_m};
Vector2u16 const Constantes2015::TAILLE_GRILLE = {300, 200};

int ConstantesCommunes2015::getNombreBlocageLogicielMax() const {
	return Constantes2015::NOMBRE_BLOCAGE_LOGICIEL_MAX;
}

int ConstantesCommunes2015::getNombreBlocagePhysiqueMax() const {
	return Constantes2015::NOMBRE_BLOCAGE_PHYSIQUE_MAX;
}

Duree ConstantesCommunes2015::getDureeMatch() const {
	return Constantes2015::DUREE_MATCH;
}

Vector2m ConstantesCommunes2015::getTailleTable() const {
	return Constantes2015::TAILLE_TABLE;
}

Vector2u16 ConstantesCommunes2015::getTailleGrille() const {
	return Constantes2015::TAILLE_GRILLE;
}
