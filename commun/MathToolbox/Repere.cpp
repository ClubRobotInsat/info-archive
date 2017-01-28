//
// Created by benji on 26/01/17.
//

#include "Repere.h"

using namespace repere;

Coordonnees::Coordonnees(const Repere &repere_parent, Vector3m position, Angle angle) : _repere_parent(repere_parent) {
    _pos = position;
    _angle = angle;
}

Coordonnees::Coordonnees(const Repere &repere_parent, Vector2m position, Angle angle)
        : Coordonnees(repere_parent, Vector3m(position.x, position.y, 0_m), angle) {}

Coordonnees::Coordonnees(Coordonnees const &coords)
        : Coordonnees(coords._repere_parent, coords._pos, coords._angle) {}

/*Coordonnees& Coordonnees::operator=(const Coordonnees other) {
    if(this != &other) {
//        std::swap(static_cast<Repere &>(this->_repere_parent), static_cast<Repere &>(other._repere_parent));
        _pos = other._pos;
        _angle = other._angle;
        //std::swap(_pos, copy._pos);
        //std::swap(_angle, copy._angle);
    }
    return *this;
}*/

Coordonnees Repere::get_coordonnees(Coordonnees coords) const {
    const int mx0 = coords._repere_parent._multX;
    const int my0 = coords._repere_parent._multY;
    const int mx1 = this->_multX;
    const int my1 = this->_multY;

    Vector3m new_position = {(coords._pos.x * mx0 + coords._repere_parent._origine.x - this->_origine.x) * mx1,
                             (coords._pos.y * my0 + coords._repere_parent._origine.y - this->_origine.y) * my1,
                             coords._pos.z};

    Angle new_angle;
    if (mx0 == mx1) {
        if (my0 == my1) {
            new_angle = coords._angle;
        }
        else {
            new_angle = - coords._angle;
        }
    }
    else {
        if (my0 == my1) {
            new_angle = (180_deg - coords._angle).toMinusPiPi();
        }
        else {
            new_angle = (coords._angle + 180_deg).toMinusPiPi();
        }
    }

    return {coords._repere_parent, new_position, new_angle};
}