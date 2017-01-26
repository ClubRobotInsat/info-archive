//
// Created by benji on 26/01/17.
//

#include "Repere.h"

using namespace repere;
Coordonnees Repere::get_coordonnees(Coordonnees coords) {
    const int mx0 = coords.repere_projete._multX;
    const int my0 = coords.repere_projete._multY;
    const int mx1 = this->_multX;
    const int my1 = this->_multY;

    Vector2m new_position = {(coords.pos.x * mx0 + coords.repere_projete._origine.x - this->_origine.x) * mx1,
                             (coords.pos.y * my0 + coords.repere_projete._origine.y - this->_origine.y) * my1};

    Angle new_angle;
    if (mx0 == mx1) {
        if (my0 == my1) {
            new_angle = coords.angle;
        }
        else {
            new_angle = - coords.angle;
        }
    }
    else {
        if (my0 == my1) {
            new_angle = (180_deg - coords.angle).toMinusPiPi();
        }
        else {
            new_angle = (coords.angle + 180_deg).toMinusPiPi();
        }
    }

    return {new_position, new_angle, coords.repere_projete};
}