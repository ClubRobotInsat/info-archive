//
// Created by paul on 23/02/17.
//

#ifndef PROJECT_OBJECT_H
#define PROJECT_OBJECT_H

#include "../IGraphicalContext.h"
#include "../IGraphicalInstance.h"
#include "SimulationToIrrlicht.h"
#include "irrlicht/irrlicht.h"
#include <ISceneNode.h>
#include <memory>


class Object : public IGraphicalInstance {

public:
	Object(int id, irr::scene::ISceneNode* ptr);

	irr::scene::ISceneNode getInternalPtr() const;

	void ObjectSetColor(irr::scene::ISceneNode* Target, irr::u32 a, irr::u32 r, irr::u32 g, irr::u32 b);

	virtual int getId() const;
	virtual void setColor(const Color3f& color) = 0;
	virtual void setScale(const Vector3f& scale);
	virtual Color3f getColor();

private:
	int _id;

	irr::scene::ISceneNode _objectPtr;
};


#endif // PROJECT_OBJECT_H
