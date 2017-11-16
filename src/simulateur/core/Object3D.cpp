//
// Created by paul on 04/02/16.
//

#include "Object3D.h"
#include "Simulateur.h"
#include "log/Log.h"


int Object3D::_lastID = 0;

// Retourne true si 2 distances sont à peu près égales (0.1mm)
inline bool distanceEquals(Distance d1, Distance d2) {
	return abs(d2 - d1) <= 0.1_mm;
}

// Retourne true si 2 vecteurs sont à peu près égaux (0.1mm)
inline bool vector2mEquals(Vector2m v1, Vector2m v2) {
	return distanceEquals(v1.x, v2.x) && distanceEquals(v1.y, v2.y);
}

inline bool angleEquals(Angle a1, Angle a2) {
	return abs(a1 - a2) <= 0.1_deg;
}
// On initialise cette objet


Object3D::Object3D() : _ID(_lastID++) {}

Object3D::~Object3D() {
	Object3D::getRemoveMessage();
	_physicProperties.reset(nullptr);
}


JSON Object3D::getCreationMessage() {

	// Partie qui envoie au serveur un nouvel objet
	JSON message;
	message["type"] = "newmodel";
	message["id"] = _ID;
	message["position"] = toJVector3(toSimulation(_physicProperties->getPosition().x),
	                                 toSimulation(_physicProperties->getPosition().y),
	                                 toSimulation(_zPosition));
	message["angle"] = _physicProperties->getAngle().toDeg();
	message["model"] = _modelName;
	message["color"] = _color;
	return message;
}

JSON Object3D::getUpdateMessage() {

	// Partie qui fait la MaJ des objets
	JSON message;
	// Mise à jour de la position
	Vector2m physicalPos = _physicProperties->getPosition();

	// On fait la maj seulement si nécessaire
	if(!(vector2mEquals(_oldPosition, physicalPos) && angleEquals(_oldAngle, _physicProperties->getAngle()))) {
		_oldPosition = physicalPos;
		_oldAngle = _physicProperties->getAngle();

		message["matrix"] = toJSON(generateTransformationMatrix());
	}

	// Si le message est vide => on n'ajoute rien.
	if(!message.empty()) {
		message["type"] = "move";
		message["id"] = _ID;
	}

	return message;
}

JSON Object3D::getRemoveMessage() {
	JSON message;
	message["type"] = "remove";
	message["id"] = _ID;
	return message;
}

// DEBUG
void Object3D::displayBodyPoints() {
	std::list<b2Vec2> bodypoints = _physicProperties->getBodyPoints();

	std::cout << "hitbox de l'objet " << getId() << " : ";
	for(b2Vec2 point : bodypoints) {
		std::cout << "(" << point.x << "," << point.y << ") ";
	}
	std::cout << std::endl;
}

Matrix4f Object3D::generateTransformationMatrix() {
	Matrix4f matIdentity;
	return matIdentity * Matrix4f::translationMatrix(toSimulation(
	                         {_physicProperties->getPosition().x, _physicProperties->getPosition().y, _zPosition})) *
	       Matrix4f::rotationMatrix({1, 0, 0}, _offsetAngle.x.toDeg()) *
	       Matrix4f::rotationMatrix({0, 1, 0}, _offsetAngle.y.toDeg()) *
	       Matrix4f::rotationMatrix({0, 0, 1}, _physicProperties->getAngle().toDeg() + _offsetAngle.z.toDeg()) *
	       Matrix4f::scaleMatrix(toSimulation(_scale));
}