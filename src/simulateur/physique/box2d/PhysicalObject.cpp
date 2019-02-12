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
	bodyDef.position = b2Vec2(static_cast<float32>(toBox2D(position.x)), static_cast<float32>(toBox2D(position.y)));
	_body = _context->getWorld().CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = def.getShape().get(); // La shape est clonée, aucun problème de pointeurs
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	_body->CreateFixture(&fixtureDef);


	// Masse
	b2MassData massData;
	massData.mass = static_cast<float32>(def.getMass().toKg());
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
//                 Informations                  //
// ********************************************* //
int PhysicalObject::getId() const {
	return _id;
}

void PhysicalObject::enableSimulation(bool enabled) {
	_body->SetActive(enabled);
}

bool PhysicalObject::isEnabled() const {
	return _body->IsActive();
}

bool PhysicalObject::isDynamic() const {
	return _body->GetType() == b2BodyType::b2_dynamicBody;
}

// ********************************************* //
//                   Position                    //
// ********************************************* //
void PhysicalObject::setPosition(Vector2m position) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Position To:" << position.x.toM() << " & " << position.y.toM() << std::endl;
#endif
	_body->SetTransform(b2Vec2(static_cast<float32>(toBox2D(position.x)), static_cast<float32>(toBox2D(position.y))),
	                    _body->GetAngle());
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
	_body->SetLinearVelocity(
	    b2Vec2(static_cast<float32>(toBox2D(speed) * cos(angle)), static_cast<float32>(toBox2D(speed) * sin(angle))));
}

Speed PhysicalObject::getLinearVelocity() const {
	b2Vec2 vec1 = _body->GetLinearVelocity();
	Speed speedAbs = fromBox2DVL(sqrt(pow(vec1.x, 2) + pow(vec1.y, 2)));
	b2Vec2 vec2 = b2Vec2(static_cast<float32>(toBox2D(speedAbs) * cos(_body->GetAngle())),
	                     static_cast<float32>(toBox2D(speedAbs) * sin(_body->GetAngle())));
	// Obtient la vitesse relative à l'angle (positive ou négative).
	if(vec1.x * vec2.x + vec2.y * vec1.y > 0)
		return speedAbs;
	else
		return -speedAbs;
}

// ********************************************* //
//                     Angle                     //
// ********************************************* //
void PhysicalObject::setAngularVelocity(AngularSpeed velocity) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Angular Speed To:" << velocity.toDeg_s() << std::endl;
#endif
	_body->SetAngularVelocity(static_cast<float32>(toBox2D(velocity)));
}

void PhysicalObject::setAngle(Angle angle) {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Setted Angle To:" << angle.toDeg() << std::endl;
#endif
	_body->SetTransform(_body->GetPosition(), static_cast<float32>(angle.toRad()));
}

Angle PhysicalObject::getAngle() const {
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Given Angle To:" << _body->GetAngle() << std::endl;
#endif
	return Angle::makeFromRad(_body->GetAngle());
}


Mass PhysicalObject::getMass() const {
	return Mass::makeFromKg(_body->GetMass());
}


// ********************************************* //
//               pour l'Affichage                //
// ********************************************* //
b2Shape::Type PhysicalObject::getShapeType() const {
	b2Shape* mainShape = getMainShape();
	if(mainShape == nullptr)
		return b2Shape::Type::e_polygon;

	return getMainShape()->GetType();
}

CircleDefinition PhysicalObject::getBodyCircleDef() const {
	b2Shape* mainShape = getMainShape();

	CircleDefinition result;
	result._pos = b2Vec2(b2Vec2_zero);
	result._radius = 0.0f;

	if(mainShape != nullptr && mainShape->GetType() == b2Shape::Type::e_circle) {
		auto circleShape = dynamic_cast<b2CircleShape*>(mainShape);
		result._pos = circleShape->m_p;
		result._radius = circleShape->m_radius;
	}

	return result;
}

std::list<b2Vec2> PhysicalObject::getBodyPoints() const {
	std::list<b2Vec2> result;

	// On se base sur la shape principale.
	b2Shape* shape = getMainShape();

	// On retourne une liste contenant tous les points de la shape
	if(shape != nullptr) {
		// La shape est un polygone ?
		if(shape->GetType() == b2Shape::e_polygon) {
			auto polygonShape = dynamic_cast<b2PolygonShape*>(shape);

			for(int i = 0; i < polygonShape->m_count; i++) {
				b2Vec2 vertex = polygonShape->GetVertex(i);
				result.emplace_back(toBox2D(absolutePositionPoint(Vector2m(fromBox2D(vertex.x), fromBox2D(vertex.y))).x),
				                    toBox2D(absolutePositionPoint(Vector2m(fromBox2D(vertex.x), fromBox2D(vertex.y))).y));
			}
		}
		// La shape est un cercle ?
		else if(shape->GetType() == b2Shape::e_circle) {
			auto circleShape = dynamic_cast<b2CircleShape*>(shape);

			float angleStep = b2_pi / 4;
			// permet de connaitre la position d'un point tous les PI/4 (en fonction de la pos initiale, des cos et sin)
			for(int i = 0; i < 8; i++) {
				result.emplace_back(static_cast<float>(toBox2D(
				                        absolutePositionPoint(fromBox2D({(circleShape->m_radius * cos(angleStep * i)),
				                                                         (circleShape->m_radius * sin(angleStep * i))}))
				                            .x)),
				                    static_cast<float>(toBox2D(
				                        absolutePositionPoint(fromBox2D({(circleShape->m_radius * cos(angleStep * i)),
				                                                         (circleShape->m_radius * sin(angleStep * i))}))
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
	Vector2m newpos = fromBox2D(_body->GetPosition());
	parent.position().x = newpos.x;
	parent.position().y = newpos.y;
	parent.rotation().z = getAngle();
}

// *********** PRIVATE MEMBERS ****************** //


b2Shape* PhysicalObject::getMainShape() const {
	b2Fixture* fixtureList = _body->GetFixtureList();

	if(fixtureList != nullptr) {
		return fixtureList->GetShape();
	} else {
		return nullptr;
	}
}

std::vector<b2Shape*> PhysicalObject::getAllShapes() const {
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

Vector2m PhysicalObject::absolutePositionPoint(Vector2m position) const {
	Vector2m result;
	Angle angle = Angle::makeFromRad(_body->GetAngle());

	result.x = fromBox2D(toBox2D(position.x) * cos(angle.toRad()) - toBox2D(position.y) * sin(angle.toRad()) +
	                     toBox2D(getPosition().x));
	result.y = fromBox2D(toBox2D(position.x) * sin(angle.toRad()) + toBox2D(position.y) * cos(angle.toRad()) +
	                     toBox2D(getPosition().y));

	return result;
}

std::list<b2Vec2> PhysicalObject::absolutePositionList(const std::list<b2Vec2>& points) const {
	std::list<b2Vec2> result;

	for(b2Vec2 vertex : points) {
		result.push_back(b2Vec2(static_cast<float32>(toBox2D(absolutePositionPoint(fromBox2D({vertex.x, vertex.y})).x)),
		                        static_cast<float32>(toBox2D(absolutePositionPoint(fromBox2D({vertex.x, vertex.y})).x))));
	}
	return result;
}

void PhysicalObject::addRevoluteJoint(PhysicalObject& other, Vector2m commonPoint) {
	b2RevoluteJointDef def;
	def.Initialize(_body,
	               other._body,
	               b2Vec2(static_cast<float32>(toBox2D(commonPoint.x)), static_cast<float32>(toBox2D(commonPoint.y))));

	b2Joint* joint = _context->getWorld().CreateJoint(&def);

	_joints.push_back(joint);
	other._joints.push_back(joint);
}

Vector2m PhysicalObject::getPosition() const {
	Vector2m position;
	position.x = fromBox2D(_body->GetPosition().x);
	position.y = fromBox2D(_body->GetPosition().y);
#ifdef DEBUG_BOX2D
	std::cout << "BOX2D::"
	          << "Given Position To:" << position.x.toM() << " & " << position.y.toM() << std::endl;
#endif
	return position;
}
