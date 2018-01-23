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
	 * Charge une liste d'objets contenus dans un fichier JSON.
	 * Les objets lus sont ajoutés au monde.
	 * @param json Le contenu du fichier, sous forme de JSON déjà
	 * parsé.
	 * */
	void loadJSON(const Json::Value& json);

	/** Charge le fichier indiqué. Le contenu du fichier doit
	 * être un objet JSON décrivant l'état du monde. */
	void loadJSONFromFile(std::string filename);

	/**
	 * Ecrit le contenu du monde dans un json et le renvoit.
	 * Ce json peut être chargé par un autre monde via la méthode
	 * loadJSON()
	 * @return Un json contenant toutes les informations nécessaires
	 * pour recréer le monde.
	 */
	Json::Value getJSON() const;

	/** Ecrit l'état du monde actuel sous forme d'objet JSON dans
	 * le fichier indiqué. */
	void writeJSONToFile(std::string filename) const;

	// Refactorer les noms de fonctions pour garder une cohérence dans tout le projet
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

	/** Indique si on autorise les collisions sur le simulateur.*/
	void enableCollisions(bool enable);

private:
	/// Le contexte physique, qui gère la simulation physique et alloue les données physiques des objets
	IPhysicalContext* _physicalCtx;

	/// Le contexte graphique, qui gère l'affichage des objets et alloue leurs données graphiques.
	IGraphicalContext* _graphicalCtx;

	int _maxId;

	/// la liste de tous les objets 3D
	std::vector<std::unique_ptr<Object3D>> _objectsList;

	int nextId();

protected:
	/**
	 * Crée un objet à partir des paramètres donnés et l'ajoute au monde.
	 * @tparam Args Les types des paramètres du constructeur de l'objet.
	 * @param args Les paramètres à donner au constructeur de l'Object3D.
	 * L'id ne doit pas être donné, il est généré automatiquement.
	 * @return Une référence vers l'objet créé.
	 */
	template <typename... Args>
	Object3D& createObject(Args&&... args) {
		_objectsList.push_back(std::make_unique<Object3D>(nextId(), args...));
		return *_objectsList.back();
	}
};


#endif // ROOT_WORLD_H
