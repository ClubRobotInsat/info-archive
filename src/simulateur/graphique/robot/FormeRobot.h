//
// Created by paul on 04/02/16.
//

#ifndef ROOT_FORME_ROBOT_H
#define ROOT_FORME_ROBOT_H


#include "../../communication/cartes/IDeplacement.h"
#include "../../core/World.h"
#include "ConstantesCommunes.h"

using namespace mass;

class FormeRobot : public Object3D, IDeplacement {

public:
	FormeRobot(World& world, Vector3m position, ObjectType typeRobot, Constantes::RobotColor color);

	// retourne s'il est ami, ennemi et sa taille
	Object3D::ObjectType getType() {
		return _objectType;
	}

	JSON getCreationMessage() override;

	// Implémentation des méthodes IDeplacement
	Vector2m getPosition() override;

	Angle getAngle() override;

	Speed getLinearVelocity() const override;

	AngularSpeed getAngularVelocity() override;

	void setLinearVelocity(Speed) override;

	void setAngularVelocity(AngularSpeed) override;

	Matrix4f generateTransformationMatrix() override;
};


#endif // ROOT_FORME_ROBOT_H
