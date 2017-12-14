//
// Created by paul on 04/02/16.
//

#ifndef ROOT_WORLD_H
#define ROOT_WORLD_H

#include <list>
#include <memory>

#include <Box2D/Dynamics/b2World.h>

#include <ConstantesCommunes.h>

#include "../graphique/IGraphicalContext.h"
#include "../physique/IPhysicalContext.h"
#include "Object3D.h"

class Robot2017;
class Object3D;

class World {

public:
	/**
	 * Construction du monde de la simulation
	 * Contient le monde physique, tous les objets 3D qui y appartiennent et les messages à envoyer au serveur
	 * @param gravity gravité à donner au moteur physique box2D
	 */
	World(IPhysicalContext* physics, IGraphicalContext* graphics);

	/**
	 * Destruction du monde : suppression de tous les objets
	 */
	~World();

	IPhysicalContext& getPhysics() {
		return *_physicalCtx;
	}

	IGraphicalContext& getGraphics() {
		return *_graphicalCtx;
	}

	// TODO méthode à supprimer (remplacer par des constantes)
	// -> cette méthode est aussi utilisée pour la carte déplacement 2009, il faudra utiliser un repère particulier
	/**
	 * Obtient la taille de la table de jeu
	 * Méthode à surcharger dans les mondes propres à chaque année
	 */
	virtual Vector2m getSize() = 0;

	/**
	 * Mise à jour du monde envoie les messages d'update pour tous les objets
	 * @param time 10_ms is good
	 */
	void update(Duration time);

	/**
	 *
	 * @tparam Args
	 * @param args
	 * @return
	 */
	template <typename... Args>
	Object3D& createObject(Args&&... args) {
		_objectsList.push_back(std::make_unique<Object3D>(nextId(), args...));
		return *_objectsList.back();
	}

	// Refactorer les noms de fonctions pour garder une cohérence dans tous le projet
	Object3D& createModel(const Vector3m& position, Mass mass, BodyType type, const std::string& model, const Color3f& color);
	Object3D& createCube(const Vector3m& dimensions, const Vector3m& position, Mass mass, BodyType type, const Color3f& color);
	Object3D& createCylinder(Length radius, Length height, const Vector3m& position, Mass mass, BodyType type, const Color3f& color);
	Object3D& createSphere(Length radius, const Vector3m& position, Mass mass, BodyType type, const Color3f& color);

	/**
	 * Supprime un objet de la liste des objets.
	 * */
	void removeObject(const Object3D* object);

	/**
	 * Suppression de tous les objets
	 * Utile pour réinitialiser ou arrêter la simulation
	 */
	void removeAllObject();

private:
	/// Le contexte physique, qui gère la simulation physique et alloue les données physiques des objets
	IPhysicalContext* _physicalCtx;

	/// Le contexte graphique, qui gère l'affichage des objets et alloue leurs données graphiques.
	IGraphicalContext* _graphicalCtx;

	int _maxId;

	/// la liste de tous les objets 3D
	std::vector<std::unique_ptr<Object3D>> _objectsList;

	int nextId();
};


#endif // ROOT_WORLD_H
