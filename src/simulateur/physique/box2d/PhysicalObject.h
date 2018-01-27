//
// Created by louis on 07/12/15.
//

#ifndef TESTBOX2D_PHYSICALOBJECT_H
#define TESTBOX2D_PHYSICALOBJECT_H

#include <list>
#include <vector>

#include <Box2D/Collision/Shapes/b2Shape.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>

#include <MathToolbox/MathToolbox.h>
#include <Units.h>

#include "../IPhysicalInstance.h"
#include "PhysicalToSimulationUnits.h"

class PhysicalObjectDefinition;
class Box2DPhysicalContext;

struct CircleDefinition {
	b2Vec2 _pos;
	float _radius;
};

class PhysicalObject : public IPhysicalInstance {
public:
	/**
	 * Crée un objet physique utilisable dans le moteur physique de box2D
	 * @param def définition préalable de l'objet (attribution d'une shape et d'un body type de box2D)
	 * @param world monde du simulateur qui possède l'objet
	 * @param position initiale de l'objet
	 */
	PhysicalObject(PhysicalObjectDefinition& def, Box2DPhysicalContext* context, int id, Vector2m position = b2Vec2_zero);
	PhysicalObject(PhysicalObjectDefinition& def, Box2DPhysicalContext* context, int id, Length x, Length y);

	/**
	 * Destruction de l'objet physique
	 */
	virtual ~PhysicalObject() override;

	int getId() const override;

	/**
	 * Active ou désactive la simulation physique de l'objet par le moteur physique box2D
	 * @param enabled TRUE si activation, FALSE si désactivation
	 */
	void enableSimulation(bool enabled) override;

	/**
	 * @return TRUE si la simulation physique de l'objet est activée, FALSE sinon
	 */
	bool isEnabled() const override;

	/**
	 * @return TRUE si l'objet est dynamic (s'il peut avoir des mouvements)
	 */
	bool isDynamic() const override;

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
	void setPosition(Vector2m position) override;

	/**
	 * Obtient la position courante de l'objet
	 */
	Vector2m getPosition() const override;

	/**
	 * Modifie la vitesse linéaire actuelle de l'objet
	 * @param speed nouvelle vitesse
	 */
	void setLinearVelocity(Speed speed) override;

	/**
	 * Récupère la vitesse actuelle de l'objet
	 */
	Speed getLinearVelocity() const override;

	/**
	 * Modifie la vitesse angulaire de l'objet
	 * @param velocity nouvelle vitesse
	 */
	void setAngularVelocity(AngularSpeed velocity) override;

	/**
	 * Modifie l'angle de l'objet
	 * @param angle nouvel angle
	 */
	void setAngle(Angle angle) override;

	/**
	 * Obtient la vitesse angulaire de l'objet
	 */
	AngularSpeed getAngularVelocity() const override {
		return fromBox2DVA(_body->GetAngularVelocity());
	};

	/**
	 * Obtient l'angle de lobjet
	 */
	Angle getAngle() const override;

	/**
	 * Obtient la masse de l'objet
	 */
	Mass getMass() const override;

	/**
	 * Obtient le type box2D de la shape de l'objet
	 * Utile pour l'affichage
	 */
	b2Shape::Type getShapeType() const;

	/**
	 * Obtient la liste des points qui définissent la forme de l'objet
	 * Si l'objet est un polygone, renvoie la liste des sommets
	 * Si l'objet est un cercle, renvoie lla liste des sommets de l'octogone régulier ressemblant au cercle
	 * Utile pour l'affichage
	 */
	std::list<b2Vec2> getBodyPoints() const;
	// Retourne un objet CircleDefinition qui contient la position et
	// le rayon de la shape si la shape est de type "cercle".
	/**
	 * Obtient la paire {position, rayon} qui définit la shape d'un objet de type Circle
	 */
	CircleDefinition getBodyCircleDef() const;

	/**
	 * Actualise la position de l'objet 3D passé en paramètre pour
	 * qu'elle corresponde à la position de cet objet physique.
	 * @param parent
	 */
	void update(Object3D& parent) override;

private:
	/// id de l'objet
	int _id;
	/// Contexte physique de l'objet.
	Box2DPhysicalContext* _context;
	/// body rigide de l'objet
	b2Body* _body;

	/// liste des jointures avec d'autres objets physiques
	std::vector<b2Joint*> _joints;

	/// Obtient la première shape passée à l'objet, que l'on consière comme étant sa principale
	b2Shape* getMainShape() const;
	/// Obtient toutes les shapes de l'objet.
	std::vector<b2Shape*> getAllShapes() const;
	/// Obtient la véritable position d'un point de l'objet selon son angle
	Vector2m absolutePositionPoint(Vector2m position) const;
	/// Obtient la véritable position de tous les points d'un objet selon son angle
	std::list<b2Vec2> absolutePositionList(const std::list<b2Vec2>& points) const;
};


#endif // TESTBOX2D_PHYSICALOBJECT_H
