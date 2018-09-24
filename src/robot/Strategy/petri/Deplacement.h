//
// Created by graphman on 24/04/15.
//

#pragma once
#include "../../../Cartes/CarteDeplacementCommun.h"
#include "PetriCommon.h"
#include <MathToolbox/MathToolbox.h>
#include <Units/Units.h>

static const Time TIMEOUT_DEFAULT = Constantes::MATCH_DURATION;
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

// fonctions de recallage : le robot tourne dans le bon sens et il recule jusqu'à être bloqué
// il mets ensuite à jour sa position ; /!\ aux objets sur la table !
ResultatAction recallageHaut(SensAvance sens = SensAvance::Avant, Distance y = 2_m);
ResultatAction recallageBas(SensAvance sens = SensAvance::Avant, Distance y = 0_m);
ResultatAction recallageDroit(SensAvance sens = SensAvance::Avant, Distance x = 3_m);
ResultatAction recallageGauche(SensAvance sens = SensAvance::Avant, Distance x = 0_m);

// @PetriLab@

// fonctions utilisées souvent
ResultatAction tournerAbsolu(Angle angle);
ResultatAction allerA(Distance x, Distance y);


// @Utilitaires@

// retourne la position actuelle
Vector2m positionRobot();
Angle angleRobot();
// Renvoie la distance en vol d'oiseau entre le robot et un point
Distance distanceRobotPosition(Distance x, Distance y);

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
