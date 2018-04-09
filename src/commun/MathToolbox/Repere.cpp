//
// Created by benji on 26/01/17.
//

#include "Repere.h"

using namespace repere;

Orientation::Orientation(Angle angle, const Repere& repere_parent) : _angle(angle), _repere(repere_parent) {}

Orientation::Orientation(const Orientation& other) : _angle(other._angle), _repere(other._repere) {}

Position::Position(Vector3m pos, const Repere& repere_parent) : _pos(pos), _repere(repere_parent) {}

Position::Position(Vector2m pos, const Repere& repere_parent) : _pos(toVec3(pos)), _repere(repere_parent) {}

Position::Position(Distance x, Distance y, const Repere& repere_parent) : _pos(x, y, 0_m), _repere(repere_parent) {}

Position::Position(const Position& other) : _pos(other._pos), _repere(other._repere) {}

Coordonnees::Coordonnees(Vector3m position, Angle angle, const Repere& repere_parent)
        : _pos(position), _angle(angle), _repere_parent(repere_parent) {}

Coordonnees::Coordonnees(Vector2m position, Angle angle, const Repere& repere_parent)
        : Coordonnees(Vector3m(position.x, position.y, 0_m), angle, repere_parent) {}

Coordonnees::Coordonnees(const Coordonnees& coords) : Coordonnees(coords._pos, coords._angle, coords._repere_parent) {}

Coordonnees& Coordonnees::operator=(const Coordonnees& coords) {
	if(_repere_parent == coords._repere_parent) {
		_pos = coords._pos;
		_angle = coords._angle;
	} else {
		_pos = _repere_parent.getPosition(coords._pos, coords._repere_parent);
		_angle = _repere_parent.getAngle(coords._angle, coords._repere_parent);
	}
	return *this;
}


Vector3m Repere::getPosition(Vector3m initPos, const Repere& repere) const {
	const int mx0 = repere._multX;
	const int my0 = repere._multY;
	const int mx1 = this->_multX;
	const int my1 = this->_multY;

	return {(initPos.x * mx0 + repere._origine.x - this->_origine.x) * mx1,
	        (initPos.y * my0 + repere._origine.y - this->_origine.y) * my1,
	        initPos.z};
}

Angle Repere::getAngle(Angle initAngle, const Repere& repere) const {
	const int mx0 = repere._multX;
	const int my0 = repere._multY;
	const int mx1 = this->_multX;
	const int my1 = this->_multY;

	Angle new_angle;
	if(mx0 == mx1) {
		if(my0 == my1) {
			new_angle = initAngle;
		} else {
			new_angle = initAngle;
		}
	} else {
		if(my0 == my1) {
			new_angle = (180_deg - initAngle).toMinusPiPi();
		} else {
			new_angle = (initAngle + 180_deg).toMinusPiPi();
		}
	}

	return new_angle;
}

Angle Repere::convertDeltaAngle(const Repere& origin, const Angle& angle) const {
	return origin._multX * origin._multY * _multX * _multY * angle;
}

bool Repere::operator==(const Repere& other) const {
	return _origine == other._origine && _multX == other._multX && _multY == other._multY;
}
