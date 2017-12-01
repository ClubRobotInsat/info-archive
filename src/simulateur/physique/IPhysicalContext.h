//
// Created by louis on 29/11/17.
//

#ifndef ROOT_IPHYSICALCONTEXT_H
#define ROOT_IPHYSICALCONTEXT_H

#include <MathToolbox/MathToolbox.h>
#include <Units.h>

class IPhysicalInstance;

enum BodyType { STATIC_BODY, DYNAMIC_BODY };

class IPhysicalContext {
public:
	virtual ~IPhysicalContext() = default;

	virtual void update() = 0;

	virtual IPhysicalInstance* createDefaultObject(const Vector3m& position, BodyType type) = 0;
	virtual IPhysicalInstance* createCuboid(const Vector3m& position, Mass mass, BodyType type, const Vector3m& dimensions) = 0;
	virtual IPhysicalInstance* createCylinder(const Vector3m& position, Mass mass, BodyType type, Length radius, Length height) = 0;

	virtual void remove(IPhysicalInstance* object) = 0;
};

#endif // ROOT_IPHYSICALCONTEXT_H
