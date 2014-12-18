/*
 * Feu.h
 *
 *  Created on: 13 mars 2014
 *      Author: jerk
 */

#ifndef FOYER_H_
#define FOYER_H_

#include "../../commun/2014/ConstantesCommunes2014.h"
#include "../../commun/MathToolbox/MathToolbox.h"

class Feu {
public:
	/// Enum
	typedef enum {
		PLAT,		// A plat
		VERTICAL,	// Base dans le sens de la hauteur de la table
		HORIZONTAL 	// Base dans le sens de la longueur de la table
	} SensFeu;

	Feu(distanceM x, distanceM y, SensFeu sens, int level);
	virtual ~Feu();

	/// Getters
	// Const = accessible depuis un objet "Feu" constant.
	Vector2m getPosition() const {return _position;}
	int getLevel() const {return _level;}
	SensFeu getSens() const {return _sens;}
	Constantes2014::CouleurRobot getCouleurTop() const {return _couleurTop;}
	Constantes2014::CouleurRobot getCouleurRight() const {return _couleurRight;}
	Constantes2014::CouleurRobot getCouleurLeft() const {return !_couleurRight;}
	Constantes2014::CouleurRobot getCouleurUp() const {return _couleurUp;}
	Constantes2014::CouleurRobot getCouleurDown() const {return !_couleurUp;}
	bool isCollected() const {return _collected;}

	/// Setters
	// Pas const = pas accessibles depuis un objet "Feu" constant car modifie ses attributs
	void setPosition(Vector2m const &pos) {_position = pos;}
	void setLevel(int level) {_level = level;}
	void setSens(SensFeu sens) {_sens = sens;}
	void setCouleurTop(Constantes2014::CouleurRobot couleur) {_couleurTop = couleur;}
	void setCouleurRight(Constantes2014::CouleurRobot couleur) {_couleurRight = couleur;}
	void setCouleurUp(Constantes2014::CouleurRobot couleur) {_couleurUp = couleur;}
	// Const car fait quelque chose sur un attribut mutable = modifiable par un objet "const"
	void setCollected(bool collected) const {_collected = collected;}

private:
	Vector2mm _position;
	// niveau en hauteur : 0 = sol, 1, 2, 3 = empilé
	int _level;
	SensFeu _sens;
	// défini si pas debout
	Constantes2014::CouleurRobot _couleurTop;
	// défini si debout dans le sens vertical
	Constantes2014::CouleurRobot _couleurRight;
	// défini si debout dans le sens horizontal
	Constantes2014::CouleurRobot _couleurUp;
	// 0 si non ramassé
	// Mutable = possible de le modifier depuis un objet "const"
	mutable bool _collected;
};

#endif /* FOYER_H_ */
