//
// Created by paul on 23/02/17.
//

#ifndef PROJECT_OBJECT_H
#define PROJECT_OBJECT_H

#include "irrlicht/irrlicht.h"
#include <memory>

#include "../IGraphicalContext.h"
#include "../IGraphicalInstance.h"
#include "SimulationToIrrlicht.h"

class Object : public IGraphicalInstance {
public:
	Object(int id, irr::scene::ISceneNode* ptr);

	irr::scene::ISceneNode* getInternalPtr() const;

	void ObjectSetColor(irr::scene::ISceneNode* Target, irr::u32 a, irr::u32 r, irr::u32 g, irr::u32 b);

	int getId() const override;
	void setColor(const Color3f& color) override;
	void setScale(const Vector3f& scale) override;
	Color3f getColor() override;

	void update(Object3D& parent) override;

private:
	int _id;

	irr::scene::ISceneNode* _objectPtr;
};

#endif // PROJECT_OBJECT_H
