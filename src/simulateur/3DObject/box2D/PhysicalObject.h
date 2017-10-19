//
// Created by louis on 07/12/15.
//

#ifndef TESTBOX2D_PHYSICALOBJECT_H
#define TESTBOX2D_PHYSICALOBJECT_H

#include "PhysicalObjectDefinition.h"
#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>
#include <MathToolbox/MathToolbox.h>
#include <Units/Frequency.h>
#include <Units/Speed.h>
#include <Units/Units.h>

class PhysicalObjectDefinition;

struct CircleDefinition {
	b2Vec2 _pos;
	float _radius;
};

class PhysicalObject {
public:
	/**
	 * Crée un objet physique utilisable dans le moteur physique de box2D
	 * @param def définition préalable de l'objet (attribution d'une shape et d'un body type de box2D)
	 * @param world monde du simulateur qui possède l'objet
	 * @param position initiale de l'objet
	 */
	PhysicalObject(PhysicalObjectDefinition& def, b2World* world, Vector2m position = b2Vec2_zero);
	PhysicalObject(PhysicalObjectDefinition& def, b2World* world, Length x, Length y);

	/**
	 * Destruction de l'objet physique
	 */
	virtual ~PhysicalObject() {
		if(_body) { // au cas où _body == nullptr
			_world->DestroyBody(_body);
		}

		for(b2Joint* joint : _joints) {
			_world->DestroyJoint(joint);
			joint = nullptr;
		}
	}

	/**
	 * Active ou désactive la simulation physique de l'objet par le moteur physique box2D
	 * @param enabled TRUE si activation, FALSE si désactivation
	 */
	void enableSimulation(bool enabled) {
		_body->SetActive(enabled);
	}

	/**
	 * Réunit deux objets physiques en les jointant
	 * @param other second objet à réunir
	 * @param commonPoint point commun depuix lesquels les deux objets seront liés
	 */
	void addRevoluteJoint(PhysicalObject& other, Vector2m commonPoint);

	/**
	 * Modifie la position de l'objet
	 * @param position nouvelle position
	 */
	void setPosition(Vector2m position);

	/**
	 * Obtient la position courante de l'objet
	 */
	const Vector2m getPosition();

	/**
	 * Modifie la vitesse linéaire actuelle de l'objet
	 * @param speed nouvelle vitesse
	 */
	void setLinearVelocity(Speed speed);

	/**
	 * Récupère la vitesse actuelle de l'objet
	 */
	Speed getLinearVelocity() {
		b2Vec2 vec1 = _body->GetLinearVelocity();
		Speed speedAbs = fromSimulationVL(sqrt(pow(vec1.x, 2) + pow(vec1.y, 2)));
		b2Vec2 vec2 = b2Vec2(toSimulation(speedAbs) * cos(_body->GetAngle()), toSimulation(speedAbs) * sin(_body->GetAngle()));
		// Obtient la vitesse relative à l'angle (positive ou négative).
		if(vec1.x * vec2.x + vec2.y * vec1.y > 0)
			return speedAbs;
		else
			return -speedAbs;
	};

	/**
	 * Modifie la vitesse angulaire de l'objet
	 * @param velocity nouvelle vitesse
	 */
	void setAngularVelocity(AngularSpeed velocity);

	/**
	 * Modifie l'angle de l'objet
	 * @param angle nouvel angle
	 */
	void setAngle(Angle angle);

	/**
	 * Obtient la vitesse angulaire de l'objet
	 */
	AngularSpeed getAngularVelocity() {
		return fromSimulationVA(_body->GetAngularVelocity());
	};

	/**
	 * Obtient l'angle de lobjet
	 */
	Angle getAngle();

	/**
	 * Obtient la masse de l'objet
	 */
	Mass getMass();

	/**
	 * Obtient le type box2D de la shape de l'objet
	 * Utile pour l'affichage
	 */
	b2Shape::Type getShapeType();

	/**
	 * Obtient la liste des points qui définissent la forme de l'objet
	 * Si l'objet est un polygone, renvoie la liste des sommets
	 * Si l'objet est un cercle, renvoie lla liste des sommets de l'octogone régulier ressemblant au cercle
	 * Utile pour l'affichage
	 */
	std::list<b2Vec2> getBodyPoints();
	// Retourne un objet CircleDefinition qui contient la position et
	// le rayon de la shape si la shape est de type "cercle".
	/**
	 * Obtient la paire {position, rayon} qui définit la shape d'un objet de type Circle
	 */
	CircleDefinition getBodyCircleDef();

private:
	/// body rigide de l'objet
	b2Body* _body;
	/// monde physique qui contient l'objet
	b2World* _world;

	/// liste des jointures avec d'autres objets physiques
	std::vector<b2Joint*> _joints;

	/// Obtient la première shape passée à l'objet, que l'on consière comme étant sa principale
	b2Shape* getMainShape();
	/// Obtient toutes les shapes de l'objet.
	std::vector<b2Shape*> getAllShapes();
	/// Obtient la véritable position d'un point de l'objet selon son angle
	Vector2m absolutePositionPoint(Vector2m position);
	/// Obtient la véritable position de tous les points d'un objet selon son angle
	std::list<b2Vec2> absolutePositionList(const std::list<b2Vec2>& points);
};


#endif // TESTBOX2D_PHYSICALOBJECT_H
