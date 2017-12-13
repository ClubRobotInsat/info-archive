//
// Created by paul on 23/02/17.
//

#ifndef PROJECT_OBJECT_H
#define PROJECT_OBJECT_H

#include <ISceneNode.h>
#include <memory>


class Object {

public:
	Object(int id, irr::scene::ISceneNode* ptr);

	std::shared_ptr<irr::scene::ISceneNode> getInternalPtr() const;

	void ObjectSetColor(irr::scene::ISceneNode* Target, irr::u32 a, irr::u32 r, irr::u32 g, irr::u32 b);

private:
	int _id;

	std::shared_ptr<irr::scene::ISceneNode> _objectPtr;
};


#endif // PROJECT_OBJECT_H
