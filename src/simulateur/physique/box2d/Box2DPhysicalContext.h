//
// Created by louis on 30/11/17.
//

#ifndef ROOT_BOX2DPHYSICALCONTEXT_H
#define ROOT_BOX2DPHYSICALCONTEXT_H

#include <memory>

#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <Units/TimePoint.h>

#include "../IPhysicalContext.h"
#include "PhysicalObject.h"

class Box2DPhysicalContext : public IPhysicalContext {
public:
	Box2DPhysicalContext(const b2Vec2& gravity);

	b2World& getWorld();

	virtual void update() override;

	IPhysicalInstance* createDefaultObject(const Vector3m& position, BodyType type) override;
	IPhysicalInstance* createCuboid(const Vector3m& position, Mass mass, BodyType type, const Vector3m& dimensions) override;
	IPhysicalInstance* createCylinder(const Vector3m& position, Mass mass, BodyType type, Length radius, Length height) override;

	void remove(IPhysicalInstance* object) override;

private:
	b2World _b2world;
	TimePoint _lastUpdateTime;

	int _maxId;
	std::vector<std::unique_ptr<PhysicalObject>> _objects;

	template <typename... Args>
	PhysicalObject* addObject(Args... args) {
		_objects.push_back(std::make_unique<PhysicalObject>(args...));
		return _objects.back().get();
	}

	int nextId();
};


#endif // ROOT_BOX2DPHYSICALCONTEXT_H
