// IDeplacement.h

#ifndef IDEPLACEMENT_H_
#define IDEPLACEMENT_H_

#include "MathToolbox/MathToolbox.h"
#include "log/Log.h"
#include <Units.h>
class IDeplacement {
public:
	/*
	 enum Roue {
	 ROUE_GAUCHE=0,
	 ROUE_DROITE=1
	 };
	 */

	virtual ~IDeplacement() {}

	virtual Vector2m getPosition() = 0;

	// virtual Angle getAngle2d() const = 0;

	virtual Speed getLinearVelocity() const = 0;

	virtual Angle getAngle() = 0;

	virtual AngularSpeed getAngularVelocity() = 0;

	virtual void setLinearVelocity(Speed) = 0;

	virtual void setAngularVelocity(AngularSpeed) = 0;

	// Fonction utilitaire qui permet de faire passer un point du référentiel du robot
	// au référentiel absolu (du monde).
	inline Vector2m robotToWorld(const Vector2m&) const {
		// TODO : à implémenter avec Box2D
		logDebug("[Ideplacement.h] robotToWorld not implemented !");
		// return p.rotate(Vector3d(0.0, 1.0, 0.0), getAngle2d()) + getPosition();
		return {};
	}

	// Même chose dans l'autre sens
	inline Vector2m worldToRobot(const Vector2m&) const {
		// TODO : à implémenter avec Box2D
		logDebug("[Ideplacement.h] robotToWorld not implemented !");
		// return (p - getPosition()).rotate(Vector3d(0.0, 1.0, 0.0), -getAngle2d());
		return {};
	}

	/*
	 // NB : la force d'un moteur et le frein d'un moteur restent constants tant qu'on
	 // n'a pas réappelé setForceMoteur() ou setFrein().
	 virtual void setForceMoteur(btScalar force, Roue roue) = 0;

	 virtual btScalar getForceMoteur(Roue roue) = 0;

	 virtual void setFrein(btScalar brake, Roue roue) = 0;

	 virtual btScalar getFrein(Roue roue) = 0;

	 virtual btWheelInfo& getWheelInfo(int index) = 0;

	 virtual btScalar getSpeed() = 0;
	 */
};

#endif /*IDEPLACEMENT_H_*/
