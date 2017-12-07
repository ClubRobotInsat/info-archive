//#define DEBUG_BOX2D //A definir si on veut afficher des messages de DEBUG_BOX2D à chaque fois que des données passent
// par box2D
#include "PhysicalObject.h"

#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>

#include "../../core/Object3D.h"
#include "Box2DPhysicalContext.h"
#include "PhysicalObjectDefinition.h"

PhysicalObject::PhysicalObject(PhysicalObjectDefinition& def, Box2DPhysicalContext* context, int id, Vector2m position)
        : _id(id), _context(context) {
	// Definition et création du body
	b2BodyDef bodyDef;
	bodyDef.type = def.getType();
	bodyDef.position = b2Vec2(toSimulation(position.x), toSimulation(position.y));
	_body = _context->getWorld().CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = def.getShape().get(); // La shape est clonée, aucun problème de pointeurs
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	_body->CreateFixture(&fixtureDef);


	// Masse
	b2MassData massData;
	massData.mass = def.getMass().toKg();
	massData.I = 1;
	massData.center = b2Vec2_zero;
	_body->SetMassData(&massData);

	// Position
	setPosition(position);
}

PhysicalObject::PhysicalObject(PhysicalObjectDefinition& def, Box2DPhysicalContext* context, int id, Length x, Length y)
        : PhysicalObject(def, context, id, Vector2m(x, y)) {}

PhysicalObject::~PhysicalObject() {
	if(_body) { // au cas où _body == nullptr
		_context->getWorld().DestroyBody(_body);
	}

	for(b2Joint* joint : _joints) {
		_context->getWorld().DestroyJoint(joint);
		joint = nullptr;
	}
}

// ********************************************* //
//                   Position                    //
// ********************************************* //
void PhysicalObject::setPosition(Vector2m position) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Position To:" << position.x.toM() << " & " << position.y.toM() << std::endl;
#endif
	_body->SetTransform(b2Vec2(toSimulation(position.x), toSimulation(position.y)), _body->GetAngle());
}

// ********************************************* //
//                   Velocity                    //
// ********************************************* //

void PhysicalObject::setLinearVelocity(Speed speed) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Speed To:" << speed.toM_s() << std::endl;
#endif
	float angle = _body->GetAngle();
	_body->SetLinearVelocity(b2Vec2(toSimulation(speed) * cos(angle), toSimulation(speed) * sin(angle)));
}

// ********************************************* //
//                     Angle                     //
// ********************************************* //
void PhysicalObject::setAngularVelocity(AngularSpeed velocity) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Angular Speed To:" << velocity.toDeg_s() << std::endl;
#endif
	_body->SetAngularVelocity(toSimulation(velocity));
}

void PhysicalObject::setAngle(Angle angle) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Angle To:" << angle.toDeg() << std::endl;
#endif
	_body->SetTransform(_body->GetPosition(), angle.toRad());
}

Angle PhysicalObject::getAngle() {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Given Angle To:" << _body->GetAngle() << std::endl;
#endif
	return Angle::makeFromRad(_body->GetAngle());
}


Mass PhysicalObject::getMass() {
	return Mass::makeFromKg(_body->GetMass());
}


// ********************************************* //
//               pour l'Affichage                //
// ********************************************* //
b2Shape::Type PhysicalObject::getShapeType() {
	b2Shape* mainShape = getMainShape();
	if(mainShape == nullptr)
		return b2Shape::Type::e_polygon;

	return getMainShape()->GetType();
}

CircleDefinition PhysicalObject::getBodyCircleDef() {
	b2Shape* mainShape = getMainShape();

	CircleDefinition result;
	result._pos = b2Vec2(b2Vec2_zero);
	result._radius = 0.0f;

	if(mainShape != nullptr && mainShape->GetType() == b2Shape::Type::e_circle) {
		b2CircleShape* circleShape = (b2CircleShape*)mainShape;
		result._pos = circleShape->m_p;
		result._radius = circleShape->m_radius;
	}

	return result;
}

std::list<b2Vec2> PhysicalObject::getBodyPoints() {
	std::list<b2Vec2> result;

	// On se base sur la shape principale.
	b2Shape* shape = getMainShape();

	// On retourne une liste contenant tous les points de la shape
	if(shape != nullptr) {
		// La shape est un polygone ?
		if(shape->GetType() == b2Shape::e_polygon) {
			b2PolygonShape* polygonShape = (b2PolygonShape*)shape;

			for(int i = 0; i < polygonShape->m_count; i++) {
				b2Vec2 vertex = polygonShape->GetVertex(i);
				result.push_back(b2Vec2(
				    toSimulation(absolutePositionPoint(Vector2m(fromSimulation(vertex.x), fromSimulation(vertex.y))).x),
				    toSimulation(absolutePositionPoint(Vector2m(fromSimulation(vertex.x), fromSimulation(vertex.y))).y)));
			}
		}
		// La shape est un cercle ?
		else if(shape->GetType() == b2Shape::e_circle) {
			b2CircleShape* circleShape = (b2CircleShape*)shape;

			float angleStep = b2_pi / 4;
			// permet de connaitre la position d'un point tous les PI/4 (en fonction de la pos initiale, des cos et sin)
			for(int i = 0; i < 8; i++) {
				result.push_back(
				    b2Vec2((float)toSimulation(
				               absolutePositionPoint(fromSimulation({(circleShape->m_radius * (float)cos(angleStep * i)),
				                                                     (circleShape->m_radius * (float)sin(angleStep * i))}))
				                   .x),
				           (float)toSimulation(
				               absolutePositionPoint(fromSimulation({(circleShape->m_radius * (float)cos(angleStep * i)),
				                                                     (circleShape->m_radius * (float)sin(angleStep * i))}))
				                   .y)));
			}
		} else
			std::cout
			    << "\nLa shape n'est ni un polygone, ni un cercle\nImpossible de retourner un ensemble de points\n\n"
			    << std::endl;
	}

	return result;
}

void PhysicalObject::update(Object3D& parent) {
	parent.setPosition(fromSimulation(_body->GetPosition()));
	parent.rotation().z = getAngle();
}

// *********** PRIVATE MEMBERS ****************** //


b2Shape* PhysicalObject::getMainShape() {
	b2Fixture* fixtureList = _body->GetFixtureList();

	if(fixtureList != nullptr) {
		return fixtureList->GetShape();
	} else {
		return nullptr;
	}
}

std::vector<b2Shape*> PhysicalObject::getAllShapes() {
	b2Fixture* fixtureList = _body->GetFixtureList();
	std::vector<b2Shape*> result;

	while(true) {
		result.push_back(fixtureList->GetShape());

		fixtureList = fixtureList->GetNext();
		if(fixtureList == 0) {
			break;
		}
	}

	return result;
}

Vector2m PhysicalObject::absolutePositionPoint(Vector2m position) {
	Vector2m result;
	Angle angle = Angle::makeFromRad(_body->GetAngle());

	result.x = fromSimulation(toSimulation(position.x) * cos(angle.toRad()) -
	                          toSimulation(position.y) * sin(angle.toRad()) + toSimulation(getPosition().x));
	result.y = fromSimulation(toSimulation(position.x) * sin(angle.toRad()) +
	                          toSimulation(position.y) * cos(angle.toRad()) + toSimulation(getPosition().y));

	return result;
}

std::list<b2Vec2> PhysicalObject::absolutePositionList(const std::list<b2Vec2>& points) {
	std::list<b2Vec2> result;

	for(b2Vec2 vertex : points) {
		result.push_back(b2Vec2(toSimulation(absolutePositionPoint(fromSimulation({vertex.x, vertex.y})).x),
		                        toSimulation(absolutePositionPoint(fromSimulation({vertex.x, vertex.y})).x)));
	}
	return result;
}

void PhysicalObject::addRevoluteJoint(PhysicalObject& other, Vector2m commonPoint) {
	b2RevoluteJointDef def;
	def.Initialize(_body, other._body, b2Vec2(toSimulation(commonPoint.x), toSimulation(commonPoint.y)));

	b2Joint* joint = _context->getWorld().CreateJoint(&def);

	_joints.push_back(joint);
	other._joints.push_back(joint);
}

Vector2m PhysicalObject::getPosition() {
	Vector2m position;
	position.x = fromSimulation(_body->GetPosition().x);
	position.y = fromSimulation(_body->GetPosition().y);
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Given Position To:" << position.x.toM() << " & " << position.y.toM() << std::endl;
#endif
	return position;
}
