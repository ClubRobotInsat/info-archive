//
// Created by louis on 07/02/16.
//

#ifndef ROOT_SIMULATEUR_H
#define ROOT_SIMULATEUR_H

#define TARGET_SIMULATEUR

#include <atomic>

#include "ConstantesCommunes.h"
#include "World.h"

// permet de positionner plus facilement les objets
struct CubeData {
public:
	/**
	 * Obtient la taille du cube
	 */
	const Vector3m getSize() {
		return size;
	}
	/**
	 * Obtient les coordonnées du centre du cube
	 * Permet de convertir la position du coin supérieur gauche en centre du cube pour faciliter le placement des points
	 */
	const Vector3m getPosition();
	/**
	 * Modifie la taille du cube
	 * @param newSize
	 */
	void setSize(Vector3m newSize) {
		size = newSize;
	}
	/**
	 * Modifie la position du cube
	 * @param newPosition du coin supérieur gauche du cube
	 */
	void setPosition(Vector3m newPosition) {
		position = newPosition;
	}

private:
	/// Taille du cube
	Vector3m size;
	/// Position du coin supérieur gauche du cube
	Vector3m position;
};

enum SIMU_YEAR { MOON = 2017, CITY = 2018 };

class Simulateur {
public:
	/**
	 * Obtient un pointeur vers l'unique instance du simulateur
	 */
	static Simulateur& getInstance() {
		return *_instance;
	}

	/**
	 * Création du simulateur : instance + serveur + monde
	 */
	Simulateur();

	/**
	 * Destrution du simulateur : fin du serveur + suppression de tous les objets
	 */
	~Simulateur();

	/**
	 * Obtient un pointeur sur le monde du simu
	 */
	World* getWorld() {
		return &_theWorld;
	}

	/**
	 * Mise à jour du monde et de l'état du robot
	 * @param time 10_ms is good
	 */
	void update(Duration time);

	/**
	 * Change de fichier JSON cible pour load toute la table et ajouter le robot
	 */
	void setJSONFile(const std::string&);

	/**
	 * Initialise la simulation et place tous les objets de l'année en cours
	 */
	void initWorld();

	/**
	 * On demande une réinitialisation du monde si on reçoit une entrée console
	 * @param flag TRUE = reset demandé
	 */
	void setResetWorldFlag(bool flag) {
		_resetWorld = flag;
	};

	/**
	 * Réinitialisation du simulateur : objets + robot
	 */
	void resetWorld();

	/**
	 * On arrête la simulation : suppression du robot et de tous les objets
	 */
	void endWorld();

	/**
	 * On ajoute notre robot sur la table
	 * @param color couleur du robot, détermine sa position initiale et sa couleur d'affichage
	 */
	void addRobot(Constantes::RobotColor color);

	/**
	 * Obtient la couleur du robot
	 */
	Constantes::RobotColor getRobotColor();

	/**
	 * Désactive la simulation physique de tous les objets et du robot sur la table
	 * L'affichage n'est pas compromis
	 */
	void disableSimulation();

	void sendTextMessage(const std::string& message);

private:
	/// Unique instance du simulateur
	static Simulateur* _instance;

	std::unique_ptr<IGraphicalContext> _graphicalCtx;
	std::unique_ptr<IPhysicalContext> _physicalCtx;

	/// Le monde dans lequel on stocke tous les objets
	World _theWorld;

	/// On demande à réinitialiser la simulation?
	std::atomic_bool _resetWorld;

	/// Indique si la physique est activée.
	bool _enablePhysics;

	// Pour afficher sans simuler les objets de la table
	//(parce que le robot est sensible et que ça fait n'importe quoi)

	std::string _json_file;
};

#endif // ROOT_SIMULATEUR_H
