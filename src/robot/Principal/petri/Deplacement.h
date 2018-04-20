//
// Created by graphman on 24/04/15.
//

#pragma once
#include "../../Cartes/CarteDeplacementCommun.h"
#include "PetriCommon.h"
#include <MathToolbox/MathToolbox.h>
#include <Units/Units.h>

static const Time TIMEOUT_DEFAULT = 90_s;
static const Time ADD_TIMEOUT_SECURITY = 1_s;


// @PetriLab@ Déplacement

// déplacements linéaires avec arret si adversaire
ResultatAction avancerDe(Distance distance, SensAvance sens);
ResultatAction reculerDe(Distance distance);
ResultatAction avancerDe(Distance distance);

// avancer jusqu'à un blocage
ResultatAction avancerInfini(Duration timeout = TIMEOUT_DEFAULT);
ResultatAction reculerInfini(Duration timeout = TIMEOUT_DEFAULT);

// arret d'urgence
ResultatAction arreter();

// rotation du robot sur lui meme
// cote bleu, sens trigo : + ; cote jaune, sens trigo : -
ResultatAction tournerRelatif(Angle angle);

// position finale ; tout est automatisé
ResultatAction allerA(Distance x, Distance y, SensAvance sens);
ResultatAction allerA(Distance x, Distance y, SensAvance sens, Duration timeout);
ResultatAction allerA_vec(Vector2m pos, SensAvance sens, Duration timeout = 25_s);
ResultatAction allerA_vec(Vector2m pos);


// @PetriLab@

// fonctions utilisées souvent
ResultatAction tournerAbsolu(Angle angle);
ResultatAction allerA(Distance x, Distance y);


// @Utilitaires@

// retourne la position actuelle
Vector2m positionRobot();
double angleRobot();
// Renvoie la distance en vol d'oiseau entre le robot et un point
double distanceRobotPosition(Distance x, Distance y);

/// @return true si le robot adverse se trouve à une distance inférieure à celle spécifiée
bool advProche(Distance distance = 50_cm);


// @PetriLab@ Vitesses

// changements d'allures
ResultatAction setVitesseAngulaireLente();
ResultatAction setVitesseAngulaireRapide();
ResultatAction resetVitesseAngulaire();
ResultatAction setVitesseAngulaire(AngularSpeed s);
AngularSpeed getVitesseAngulaire();

ResultatAction setVitesseLineaireLente();
ResultatAction setVitesseLineaireRapide();
ResultatAction resetVitesseLineaire();
ResultatAction setVitesseLineaire(Speed s);
Speed getVitesseLineaire();
