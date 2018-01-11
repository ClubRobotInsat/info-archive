//
// Created by paul on 04/02/16.
//

#ifndef ROOT_OBJECT3D_H
#define ROOT_OBJECT3D_H

#include <set>

#include <MathToolbox/MathToolbox.h>
#include <Units.h>

#include "../graphique/IGraphicalInstance.h"
#include "../physique/IPhysicalInstance.h"

class World;
/*
class Object3D {
public:
    enum ObjectType {
        TABLE,
        CYLINDER,
        CUBE,
        SPHERE,
        FRIEND_ROBOT_BIG,
    };

    ObjectType getObjectType() {
        return _objectType;
    };
    void setObjectType(ObjectType type) {
        _objectType = type;
    };
    void enableSimulation(bool enabled) {
        _physicProperties->enableSimulation(enabled);
    }
    void displayBodyPoints();
protected:
    ObjectType _objectType;
};
*/

class Object3D {
public:
	Object3D(int id, IGraphicalInstance* graphics, IPhysicalInstance* physics, const Vector3m& position = {0_m, 0_m, 0_m});
	~Object3D();

	void update();

	IGraphicalInstance& getGraphics() {
		return *_graphics;
	}

	IPhysicalInstance& getPhysics() {
		return *_physics;
	}

	void setPosition(const Vector3m& position);
	void setPosition(const Vector2m& position);
	Vector3m& position() {
		return _position;
	}
	Vector3m getPosition() const {
		return _position;
	}

	void setRotation(const Vector3ang& rotation);
	Vector3ang& rotation() {
		return _rotation;
	}
	Vector3ang getRotation() {
		return _rotation;
	}

	void addTag(const std::string& tag);
	bool hasTag(const std::string& tag);

private:
	int _id;

	IGraphicalInstance* _graphics;
	IPhysicalInstance* _physics;

	Vector3m _position;
	Vector3ang _rotation;

	std::set<std::string> _tags;
};

#endif // ROOT_OBJECT3D_H
