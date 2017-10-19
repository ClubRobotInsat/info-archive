//
// Created by benji on 30/01/17.
//

#ifndef ROOT_CONSTANTESSTRATEGIE_H
#define ROOT_CONSTANTESSTRATEGIE_H

#include "../../commun/ConstantesCommunes.h"
#include "shapes.h"

using namespace Constantes;
using namespace positionObjetsTable;

struct RectInfos {
	repere::Coordonnees coords;
	Vector2m size;
};

struct CircleInfos {
	repere::Coordonnees coords;
	Distance radius;
};

const repere::Repere REFERENCE_ENVIRONMENT =
    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);

const std::vector<RectInfos> LIST_RECT_OBJECTS = {
    {{{-WALLS_SIZE, -WALLS_SIZE}}, {WALLS_DIMENSIONS.x + 2 * WALLS_SIZE, WALLS_SIZE}}, // murs
    {{{-WALLS_SIZE, -WALLS_SIZE}}, {WALLS_SIZE, WALLS_DIMENSIONS.y + 2 * WALLS_SIZE}},
    {{{-WALLS_SIZE, WALLS_DIMENSIONS.y}}, {WALLS_DIMENSIONS.x + 2 * WALLS_SIZE, WALLS_SIZE}},
    {{{WALLS_DIMENSIONS.x, -WALLS_SIZE}}, {WALLS_SIZE, WALLS_DIMENSIONS.y + 2 * WALLS_SIZE}},

    {{{0_m, 1.618_m}}, {71_cm, 382_mm}}, // zone de départ
    {{{2.29_m, 1.618_m}}, {71_cm, 382_mm}},

    {{{0_cm, 828_mm}}, {108_mm, 494_mm}}, // bâtiments latéraux
    {{{2.892_m, 828_mm}}, {108_mm, 494_mm}},

    /*{{{1.438_m, 20_cm}}, {94_mm, 60_cm}}, // bâtiments centraux
    {{{1.277_m, 117_mm}, -45_deg}, {94_mm, 60_cm}},
    {{{1.775_m, 510_mm}, 45_deg}, {94_mm, 60_cm}},*/
};


const std::vector<CircleInfos> LIST_CIRCLE_OBJECTS = {{{{65_cm, 1.46_m}}, 1_cm},  // petits ronds // R = 115_mm
                                                      {{{2.35_m, 1.46_m}}, 1_cm}, // R = 115_mm
                                                      //{{{1.07_m, 13_cm}},  115_mm},
                                                      //{{{1.93_m, 13_cm}},  115_mm},
                                                      {{{1.5_m, 0_m}}, 80_cm}, // bâtiments centraux
                                                      // grands ronds dans les coins
                                                      {{{0_m, 0_m}}, 20_cm},
                                                      {{{3_m, 0_m}}, 20_cm}};

#endif // ROOT_CONSTANTESSTRATEGIE_H
