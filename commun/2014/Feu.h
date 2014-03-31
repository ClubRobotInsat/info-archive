/*
 * Feu.h
 *
 *  Created on: 13 mars 2014
 *      Author: jerk
 */

#ifndef FOYER_H_
#define FOYER_H_

#include "ConstantesCommunes2014.h"

class Feu {
public:
	/// Enum
	typedef enum {
		PLAT,		// A plat
		VERTICAL,	// Base dans le sens de la hauteur de la table
		HORIZONTAL 	// Base dans le sens de la longueur de la table
	} SensFeu;

	Feu(int x, int y, SensFeu sens, int level);
	virtual ~Feu();

	/// Utils
	Constantes2014::CouleurRobot const changeColor(Constantes2014::CouleurRobot c) const;

	/// Getters
	// Const = accessible depuis un objet "Feu" constant.
	int getPosX() const {return _x;}
	int getPosY() const {return _y;}
	int getLevel() const {return _level;}
	SensFeu getSens() const {return _sens;}
	Constantes2014::CouleurRobot getCouleurTop() const {return _couleurTop;}
	Constantes2014::CouleurRobot getCouleurRight() const {return _couleurRight;}
	Constantes2014::CouleurRobot getCouleurLeft() const {return changeColor(_couleurRight);}
	Constantes2014::CouleurRobot getCouleurUp() const {return _couleurUp;}
	Constantes2014::CouleurRobot getCouleurDown() const {return changeColor(_couleurUp);}
	bool isCollected() const {return _collected;}

	/// Setters
	// Pas const = pas accessibles depuis un objet "Feu" constant car modifie ses attributs
	void setPosX(int pos) {_x = pos;}
	void setPosY(int pos) {_y = pos;}
	void setLevel(int level) {_level = level;}
	void setSens(SensFeu sens) {_sens = sens;}
	void setCouleurTop(Constantes2014::CouleurRobot couleur) {_couleurTop = couleur;}
	void setCouleurRight(Constantes2014::CouleurRobot couleur) {_couleurRight = couleur;}
	void setCouleurUp(Constantes2014::CouleurRobot couleur) {_couleurUp = couleur;}
	// Const car fait quelque chose sur un attribut mutable = modifiable par un objet "const"
	void setCollected(bool collected) const {_collected = collected;}

private:
	int _x, _y;
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
