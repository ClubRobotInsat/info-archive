#ifndef ROOT_REPERE_H
#define ROOT_REPERE_H

#include "Commun.h"
#include "MathToolbox/MathToolbox.h"
#include "MathToolbox/Repere.h"
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
	struct Coordonnees;

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

		Vector3m getPosition(Vector3m initPos, const Repere& repere) const;
		Angle getAngle(Angle initAngle, const Repere& repere) const;

		Angle convertDeltaAngle(const Repere& origin, const Angle& angle) const;

		bool operator==(const Repere& other) const;

	private:
		Vector2m _origine;
		Multiplicateur _multX, _multY;
	};

	// Repère absolu pour toutes les tables : (0, 0) en bas à gauche (contre le mur proche du tableau au club)
	const repere::Repere ABSOLUTE_REFERENCE = repere::Repere({0_m, 0_m}, ::repere::SENS_POSITIF, ::repere::SENS_POSITIF);

	struct Orientation {
		Orientation(Angle angle = 0_deg, const Repere& repere_parent = ABSOLUTE_REFERENCE);
		Orientation(const Orientation& other);

		Angle getAngle(const Repere& repere = ABSOLUTE_REFERENCE) {
			return repere.getAngle(_angle, _repere);
		}

	private:
		Angle _angle;
		const Repere& _repere;
	};

	struct Position {
		Position(Vector3m pos = {0_m, 0_m, 0_m}, const Repere& repere_parent = ABSOLUTE_REFERENCE);
		Position(Vector2m pos, const Repere& repere_parent = ABSOLUTE_REFERENCE);
		Position(const Position& other);

		Distance getX(const Repere& repere) {
			return repere.getPosition(_pos, _repere).x;
		}

		Distance getY(const Repere& repere) {
			return repere.getPosition(_pos, _repere).y;
		}

		void setPos2D(const Vector2m& pos, const Repere& repere = ABSOLUTE_REFERENCE) {
			_pos = _repere.getPosition(toVec3(pos), repere);
		}

		void setPos3D(const Vector3m& pos, const Repere& repere = ABSOLUTE_REFERENCE) {
			_pos = _repere.getPosition(pos, repere);
		}

		Vector2m getPos2D(const Repere& repere) {
			return toVec2(repere.getPosition(_pos, _repere));
		}

	private:
		Vector3m _pos;
		const Repere& _repere;
	};

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
		Coordonnees(Vector3m position = Vector3m(0_m, 0_m, 0_m), Angle angle = 0_rad, const Repere& repere_parent = ABSOLUTE_REFERENCE);

		/// Surcharge pour donner des coordonnées 2D
		Coordonnees(Vector2m position, Angle angle = 0_rad, const Repere& repere_parent = ABSOLUTE_REFERENCE);

		Coordonnees(Coordonnees const& coords);

		/// Getters
		Distance getX(const Repere& repere = ABSOLUTE_REFERENCE) const {
			return repere.getPosition(_pos, _repere_parent).x;
		}

		Distance getY(const Repere& repere = ABSOLUTE_REFERENCE) const {
			return repere.getPosition(_pos, _repere_parent).y;
		}

		void setPos2D(const Vector2m& pos, const Repere& repere = ABSOLUTE_REFERENCE) {
			_pos = _repere_parent.getPosition(toVec3(pos), repere);
		}

		void setPos3D(const Vector3m& pos, const Repere& repere = ABSOLUTE_REFERENCE) {
			_pos = _repere_parent.getPosition(pos, repere);
		}

		Vector2m getPos2D(const Repere& repere = ABSOLUTE_REFERENCE) const {
			return toVec2(repere.getPosition(_pos, _repere_parent));
		}

		Vector3m getPos3D(const Repere& repere = ABSOLUTE_REFERENCE) const {
			return repere.getPosition(_pos, _repere_parent);
		}

		void setAngle(Angle angle, const Repere& repere = ABSOLUTE_REFERENCE) {
			_angle = _repere_parent.getAngle(angle, repere);
		}

		Angle getAngle(const Repere& repere = ABSOLUTE_REFERENCE) const {
			return repere.getAngle(_angle, _repere_parent);
		}

		Coordonnees& operator=(const Coordonnees& coords);

	private:
		Vector3m _pos;
		Angle _angle;
		const Repere& _repere_parent;

		friend class Repere;
	};

	inline std::ostream& operator<<(std::ostream& os, const Coordonnees& coords) {
		os << "[x=" << coords.getX() << ", y=" << coords.getY() << ", theta=" << coords.getAngle() << " deg]";
		return os;
	}
}


#endif // ROOT_REPERE_H
