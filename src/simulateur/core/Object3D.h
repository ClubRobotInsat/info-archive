//
// Created by paul on 04/02/16.
//

#ifndef ROOT_OBJECT3D_H
#define ROOT_OBJECT3D_H

#include "../physique/PhysicalObject.h"
#include "SimulationToWebserver.h"
#include <MathToolbox/MathToolbox.h>
#include <Units.h>
#include <petri/Runtime/Cpp/detail/jsoncpp/include/json/json.h>

using JSON = Json::Value;

class World;

class Object3D {

protected:
	/**
	 * Classe virtuelle des objets 3D
	 */
	Object3D();

public:
	enum ObjectType {
		TABLE,
		CYLINDER,
		CUBE,
		SPHERE,
		FRIEND_ROBOT_BIG,
	};
	/**
	 * Destructeur des objets 3D simulés
	 */
	virtual ~Object3D();


	// --------------------- Accesseurs --------------------- //


	/**
	 * Obtient la position actuelle de l'objet
	 */
	Vector2m getPosition() {
		return _physicProperties->getPosition();
	};

	/**
	 * Obtient la valeur de l'angle de l'objet selon le plan (Ex, Ey)
	 * selon la convention box2D
	 */
	Angle getAngle() {
		return _physicProperties->getAngle();
	};

	/**
	 * Obtient la vitesse de l'objet en _m_s
	 */
	Speed getLinearVelocity() {
		return _physicProperties->getLinearVelocity();
	};

	/**
	 * Obtient la vitesse angulaire en _rad_s
	 */
	AngularSpeed getAngularVelocity() {
		return _physicProperties->getAngularVelocity();
	};

	/**
	 * Obtient la masse de l'objet
	 */
	Mass getMass() {
		return _physicProperties->getMass();
	};

	/**
	 * Obtient l'ID de l'objet
	 */
	int getId() {
		return _ID;
	};

	/**
	 * Obtient l'ID du dernier objet créé
	 */
	static int getLastId() {
		return _lastID;
	};

	/**
	 * Obtient le type de l'objet : {Cube, Cylinder, Sphere, Table, ... }
	 */
	ObjectType getObjectType() {
		return _objectType;
	};

	/**
	 * Obtient la couleur de l'objet pour l'affichage
	 * string : couleur en code xCode
	 */
	std::string getColor() {
		return _color;
	};


	// --------------------- Modifieurs --------------------- //


	/**
	 * Modifie la vitesse linéaire de l'objet
	 * @param speed nouvelle vitesse dans le sens de l'objet
	 */
	void setLinearVelocity(Speed speed) {
		_physicProperties->setLinearVelocity(speed);
	};

	/**
	 * Modifie la vitesse angulaire de l'objet
	 * @param speed nouvelle vitesse, selon les conventions box2D
	 */
	void setAngularVelocity(AngularSpeed speed) {
		_physicProperties->setAngularVelocity(speed);
	};

	/**
	 * Modifie la position de l'objet
	 * /!\ ne pas utiliser au milieu d'une simulation
	 * @param position nouvelle position du centre de l'objet
	 */
	void setPosition(Vector2m position) {
		_physicProperties->setPosition(position);
	};

	/**
	 * Modifie l'angle de l'objet
	 * @param rotation nouvel angle
	 */
	void setAngle(Angle rotation) {
		_physicProperties->setAngle(rotation);
	};

	/**
	 * Modifie le type de l'objet : {Cube, Cylinder, Sphere, Table, ... }
	 * A utiliser pour changer le type des objets appartenant à la table
	 * @param type nouveau type de l'objet
	 */
	void setObjectType(ObjectType type) {
		_objectType = type;
	};


	// --------------------- Autres --------------------- //


	/**
	 * Active ou désactive la simulation physique de l'objet par le moteur physique box2D
	 * @param enabled TRUE si activation, FALSE si désactivation
	 */
	void enableSimulation(bool enabled) {
		_physicProperties->enableSimulation(enabled);
	}

	/**
	 * Récupère un message JSON et l'envoie au serveur
	 * @return message de création de l'objet
	 */
	virtual JSON getCreationMessage();

	/**
	 * Récupère un message JSON et l'envoie au serveur
	 * A utiliser dans l'update du World
	 * @return message de mise à jour de l'objet
	 */
	virtual JSON getUpdateMessage();

	/**
	 * Récupère un message JSON et l'envoie au serveur
	 * @return message de destruction de l'objet
	 */
	virtual JSON getRemoveMessage();

	/**
	 * Affiche les points qui composent la shape de l'objet contenue dans _physicProperties (PhysicalObject)
	 * Fonction de debug
	 */
	void displayBodyPoints();

	/**
	 * Retourne une matrice qui permet d'appliquer une rotation dans l'espace à des objets
	 * A utiliser sur les objets qui ne réalisent pas une rotation plausible après une collision (Cube, Cylinder)
	 */
	virtual Matrix4f generateTransformationMatrix();

protected:
	/// Variable statique représentant l'ID du dernier Objet créé
	static int _lastID;

	/// Offset d'angle, utilisé lors de la rotation matricielle des cylindres
	Vector3ang _offsetAngle = {0_deg, 0_deg, 0_deg};

	/// ID de l'objet
	int _ID;

	/// Objet physique Box2D
	std::unique_ptr<PhysicalObject> _physicProperties = nullptr;

	/// Nom du modèle 3D envoyé au serveur par un message JSON
	std::string _modelName;

	/// Type de l'objet (cube, robot, cylindre...)
	ObjectType _objectType;

	/// Position selon l'axe Z
	Length _zPosition;

	/// Couleur de l'objet pour l'affichage en code xCode
	std::string _color;

	/// Facteur d'échelle de l'objet
	Vector3m _scale;

	/**
	 * Mémorisent la position précédente de l'objet
	 * Permet d'envoyer des messages de mise à jour seulement si l'objet bouge entre deux updates
	 */
	Vector2m _oldPosition;
	Length _oldZ;
	Angle _oldAngle;
};


#endif // ROOT_OBJECT3D_H
