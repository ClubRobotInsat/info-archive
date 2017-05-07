#ifndef ROOT_REPERE_H
#define ROOT_REPERE_H

#include "MathToolbox/MathToolbox.h"
#include "Units.h"

/**
 * @file Repere.h
 * @brief Implémentation d'un repère global et normé
 * @authors Louis et Benjamin
 * @date 26 janvier 2017
 *
 * Définition d'un repère unique pour avoir la même norme partout dans le code
 */

namespace repere {
	class Repere;

	/**
	 * @struct Coordonnees
	 * Définition de coordonnées normalisées dans un repère
	 *
	 * @param repere_parent Repère dans lequel on exprime la coordonnée
	 *
	 * @param position Position de l'objet par rapport au repère parent
	 *
	 * @param angle Angle de l'objet par rapport au sens défini des vecteurs unitaires Ex et Ey
	 * si multX == multY, Angle(+45_deg) correspond à une rotation de 45_deg dans le sens Trigo
	 * si multX != multY, Angle(+45_deg) correspond à une rotation de 45_deg dans le sens Horaire
	 */
	struct Coordonnees {
		Coordonnees(const Repere& repere_parent, Vector3m position = Vector3m(0_m, 0_m, 0_m), Angle angle = 0_rad);

		/// Surcharge pour donner des coordonnées 2D
		Coordonnees(const Repere& repere_parent, Vector2m position, Angle angle = 0_rad);

		Coordonnees(Coordonnees const& coords);

		/// Getters
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

	/**
	 * @enum Multiplicateur
	 * Permet de connaître la direction des vecteurs unitaires Ex, Ey
	 *
	 * Ex positif : de l'armoire vers la porte
	 * Ey positif : de l'armoire vers le poste méca
	 */
	enum Multiplicateur { SENS_POSITIF = 1, SENS_NEGATIF = -1 };

	/**
	 * @class Repere
	 * Définition d'un repère par rapport auquel toutes les coordonnées seront définies
	 *
	 * @param origine Origine du repère (0, 0)
	 *
	 * Selon la table du club :    (0,2)|--------------------------------|(3,2)
	 *                                  |                                |
	 *                                  |                                |
	 *                                  |            (1.5, 1)            |
	 *                                  |                                |
	 *                                  |                                |
	 *                             (0,0)|________________________________|(3,0)
	 *                         // proche de l'armoire                    // proche de la porte
	 *
	 * @param multX direction du vecteur Ex : > 0 en allant de 0_m à 3_m
	 *                                        < 0 en allant de 3_m à 0_m
	 *
	 * @param multY direction du vecteur Ey : > 0 en allant de 0_m à 2_m
	 *                                        < 0 en allant de 2_m à 0_m
	 *
	 *      (0,2)|--------------------------------|(3,2)
	 *           |                                |
	 *           |                                |       |
	 *           |                                |       | multY = Multiplicateur::SENS_NEGATIF
	 *           |                                |       v
	 *           |                                |
	 *      (0,0)|________________________________|(3,0)
	 *
	 *                  ==> multX = Multiplicateur::SENS_POSITIF
	 *
	 * @fn get_coordonnees Conversion des coordonnées d'un repère vers un autre
	 *
	 * @fn convertDeltaAngle Permet de gérer la symétrie Y de la table (si le robot est d'une couleur ou d'une autre)
	 *
	 * @example
	 * # Initialisation #
	 * Définition du repère global (instanciation dans ../ConstantesCommunes.h)
	 * const repere::Repere REPERE_ABSOLU = repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF,
	 * repere::Multiplicateur::SENS_POSITIF);
	 *
	 *           |--------------------------------|
	 *           |                                |
	 *           |                                |       ^
	 *           |                                |       | multY
	 *           |                                |       |
	 *           |                                |
	 *          O|________________________________|
	 *                      ==>  multX
	 *
	 * Définition d'un repère quelconque de travail
	 * const repere::Repere REPERE_CENTRE_AXES_INVERSES = repere::Repere({1.5_m, 1_m},
	 * repere::Multiplicateur::SENS_NEGATIF, repere::SENS_NEGATIF);
	 *
	 *           |--------------------------------|
	 *           |                                |
	 *           |                                |       |
	 *           |               O                |       | multY
	 *           |                                |       v
	 *           |                                |
	 *           |________________________________|
	 *                      <==  multX
	 *
	 * # Création des positions
	 * repere::Coordonnees mes_coords(ANCIEN_REPERE, Vector3m(1_m, 0.5_m, 27_cm), 12_deg);
	 *
	 * # Conversion vers un autre repère
	 * repere::Coordonnes new_coords(NOUVEAU_REPERE.get_coordonnees(mes_coords));
	 */
	class Repere {
	public:
		Repere(Vector2m origine = {0_m, 0_m}, Multiplicateur multX = SENS_POSITIF, Multiplicateur multY = SENS_POSITIF)
		        : _origine(origine), _multX(multX), _multY(multY) {}

		Repere(const Repere& other) : _origine(other._origine), _multX(other._multX), _multY(other._multY) {}

		Coordonnees get_coordonnees(Coordonnees coordonnees) const;

		Angle convertDeltaAngle(const Repere& origin, const Angle& angle) const;

		bool operator==(const Repere& other) const;

	private:
		Vector2m _origine;
		Multiplicateur _multX, _multY;
	};
}


#endif // ROOT_REPERE_H
