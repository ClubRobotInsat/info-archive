//
// Created by paul on 04/02/16.
//

#ifndef ROOT_WORLD_H
#define ROOT_WORLD_H

class Robot2017;

class Object3D;

#include "../graphique/Cube.h"
#include "../graphique/Cylinder.h"
#include "../graphique/Sphere.h"
#include "../graphique/robot/FormeRobot.h"
#include "Object3D.h"
#include <Box2D/Dynamics/b2World.h>
#include <ConstantesCommunes.h>
#include <list>
#include <memory>

class World {

public:
	/**
	 * Construction du monde de la simulation
	 * Contient le monde physique, tous les objets 3D qui y appartiennent et les messages à envoyer au serveur
	 * @param gravity gravité à donner au moteur physique box2D
	 */
	World(b2Vec2 gravity);

	/**
	 * Destruction du monde : suppression de tous les objets
	 */
	~World();

	/**
	 * Obtient la taille de la table de jeu
	 * Méthode à surcharger dans les mondes propres à chaque année
	 */
	virtual Vector2m getSize() = 0;

	/**
	 * Obtient le monde physique de box2D
	 */
	b2World* getPhysicalWorld() {
		return &_physicWorld;
	}

	/**
	 * Envoie les messages de la création de tous les objets 3D depuis le monde au serveur
	 */
	void sendWorldState() const;

	/**
	 * Réinitialise la liste de messages à envoyer au serveur
	 * A utiliser dans l'update
	 */
	void clearMessageList();

	/**
	 * Obtient la liste des tous les messages à envoyer au serveur
	 */
	JSON const& getMessageList() {
		return _messageList;
	}

	/**
	 * Fabrique un cube
	 * @param sizeX
	 * @param sizeY
	 * @param sizeZ
	 * @param position en (x,y) : correspond au centre du carré à la base  ;  en (z) ; correspond à la hauteur minimale
	 * @param mass par défaut : LIGHT = 100_g
	 * @param type = Cube
	 * @param color couleur d'affichage
	 */
	Cube& createCube(Length sizeX, Length sizeY, Length sizeZ, Vector3m position, Mass mass, Type type, std::string color);
	Cube& createCube(Vector3m size, Vector3m position, Mass mass, Type type, std::string color);

	/**
	 * Fabrique un cylindre
	 * @param radius
	 * @param size_Z
	 * @param position correspond au point du centre du cercle à la base du cylindre
	 * @param mass par défaut : LIGHT = 100_g
	 * @param type = Cylinder
	 * @param color couleur d'affichage
	 */
	Cylinder& createCylinder(Length radius, Length size_Z, Vector3m position, Mass mass, Type type, std::string color);

	/**
	 * Fabrique une sphère
	 * @param radius
	 * @param position correspond au centre de la sphère
	 * @param mass par défaut : LIGHT = 100_g
	 * @param type = Sphere
	 * @param color couleur d'affichage
	 * @return
	 */
	Sphere& createSphere(Length radius, Vector3m position, Mass mass, Type type, std::string color);

	/**
	 * Mise à jour du monde envoie les messages d'update pour tous les objets
	 * @param time 10_ms is good
	 */
	void update(Duration time);

	/**
	 * Supprime un objet de la liste des objets.
	 * */
	void removeObject(const Object3D* object);

	/**
	 * Suppression de tous les objets
	 * Utile pour réinitialiser ou arrêter la simulation
	 */
	void removeAllObject();

	/**
	 * Obtient les ID de tous les objets suivant leur type
	 * @param type {Cube, Cylinder, Sphere, Table, ... }
	 */
	std::vector<int> getListId(Object3D::ObjectType type);

	/**
	 * Active ou désactive la simulation physique de tous les objets par le moteur physique box2D
	 * Utile si on veut vérifier la position du robot sur la table sans qu'il ne soit stoppé ou qu'il y ait du mouvement
	 * @param enabled TRUE si activation, FALSE si désactivation
	 */
	void enableSimulation(bool enabled);

	/**
	 * Crée un objet suivant son type et l'ajoute à la liste des objets du monde de la simulation
	 * @param args arguments à fournir en fonction de l'objet que l'on souhaite créer
	 */
	template <typename T, typename... Args>
	T& createObject(Args&&... args) {
		_objectList.push_back(std::make_unique<T>(args...));
		return (T&)*_objectList.back();
	}

private:
	/// le monde physique box2D
	b2World _physicWorld;

	/// la liste de tous les objets 3D
	std::vector<std::unique_ptr<Object3D>> _objectList;

	/// la liste des modifications des objets à envoyer au serveur
	JSON _messageList;
};


#endif // ROOT_WORLD_H
