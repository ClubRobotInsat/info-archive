//
// Created by benji on 26/01/17.
//

#ifndef ROOT_REPERE_H
#define ROOT_REPERE_H

#include "MathToolbox/MathToolbox.h"
#include "Units.h"


/**
 * Définition d'un repère unique pour avoir la même norme partout dans le code
 */

namespace repere {
	class Repere;

	struct Coordonnees {
		Coordonnees(const Repere& repere_parent, Vector3m position = Vector3m(0_m, 0_m, 0_m), Angle angle = 0_rad);
		Coordonnees(const Repere& repere_parent, Vector2m position, Angle angle = 0_rad);
		Coordonnees(Coordonnees const& coords);

		Distance getX() const {
			return _pos.x;
		}
		Distance getY() const {
			return _pos.y;
		}
		Vector2m getPos2D() const {
			return toVec2(_pos);
		}
		Angle getAngle() const {
			return _angle;
		}

		Coordonnees& operator=(const Coordonnees& coords);

		Vector3m _pos;
		Angle _angle;
		const Repere& _repere_parent;
	};

	inline std::ostream& operator<<(std::ostream& os, const Coordonnees& coords) {
		os << "[x=" << coords.getX() << ", y=" << coords.getY() << ", theta=" << coords._angle.toDeg() << " deg]";
		return os;
	}

	enum Multiplicateur { SENS_POSITIF = 1, SENS_NEGATIF = -1 };

	class Repere {
	public:
		Repere(Vector2m origine = {0_m, 0_m}, Multiplicateur multX = SENS_POSITIF, Multiplicateur multY = SENS_POSITIF) {
			_origine = origine;
			_multX = multX;
			_multY = multY;
		}

		Coordonnees get_coordonnees(Coordonnees coordonnees) const;
		Angle convertDeltaAngle(const Repere& origin, const Angle& angle) const;

		bool operator==(const Repere& other) const;

	private:
		Multiplicateur _multX, _multY;
		Vector2m _origine;
	};
}


#endif // ROOT_REPERE_H
