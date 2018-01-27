//
// Created by louis on 16/11/17.
//

#ifndef ROOT_IPHYSICALBEHAVIOR_H
#define ROOT_IPHYSICALBEHAVIOR_H

#include "../core/IBehavior.h"

/** Cette interface permet de manipuler la partie physique
 * d'un objet du simulateur. */
class IPhysicalInstance : public IBehavior {
public:
	virtual ~IPhysicalInstance() = default;

	virtual int getId() const = 0;

	/** Active ou desactive la simulation physique de cet
	 * objet. Si un objet n'est pas simulé, il ne bouge pas
	 * et n'interagit pas avec les autres objets. */
	virtual void enableSimulation(bool enable) = 0;

	virtual bool isEnabled() const = 0;
	virtual bool isDynamic() const = 0;

	virtual Mass getMass() const = 0;

	virtual Vector2m getPosition() const = 0;
	virtual void setPosition(Vector2m position) = 0;

	virtual Angle getAngle() const = 0;
	virtual void setAngle(Angle angle) = 0;

	virtual Speed getLinearVelocity() const = 0;
	virtual void setLinearVelocity(Speed speed) = 0;

	virtual AngularSpeed getAngularVelocity() const = 0;
	virtual void setAngularVelocity(AngularSpeed speed) = 0;
};

#endif // ROOT_IPHYSICALBEHAVIOR_H
