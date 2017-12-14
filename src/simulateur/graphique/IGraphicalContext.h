//
// Created by louis on 29/11/17.
//

#ifndef ROOT_IGRAPHICALCONTEXT_H
#define ROOT_IGRAPHICALCONTEXT_H

#include <MathToolbox/MathToolbox.h>
#include <Units.h>

class IGraphicalInstance;

/**
 * Interface utilisée pour caractériser un module d'affichage
 * graphique du simulateur. Le IGraphicalContext assure la
 * représentation des objets simulés, sur un affichage en 3D.
 */
class IGraphicalContext {
public:
	virtual ~IGraphicalContext() = default;

	/** Met à jour l'affichage graphique pour qu'il
	 * corresponde à la simulation. Cette méthode peut
	 * n'avoir aucun effet si le contexte se met déjà
	 * à jour automatiquement. */
	virtual void update() = 0;

	/** Affiche un message d'information à l'écran. */
	virtual void displayMessage(std::string message) = 0;

	/** Crée un objet graphique vide. Ce type d'objet peut être
	 * utilisé si une entité de la table ne possède pas d'apparence
	 * graphique. */
	virtual IGraphicalInstance* createDefaultObject() = 0;

	/** Crée un objet graphique de forme cylindrique, avec le rayon
	 * et la hauteur passées en paramètres. */
	virtual IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height) = 0;

	/** Crée un objet graphique de forme sphérique avec le rayon
	 * donné en paramètres. */
	virtual IGraphicalInstance* createSphere(const Vector3m& position, Length radius) = 0;

	/** Crée un objet graphique ayant la forme d'un pavé avec les
	 * dimensions données. */
	virtual IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions) = 0;

	/** Crée un objet graphique dont la forme est défini par un
	 * modèle 3D.*/
	virtual IGraphicalInstance* createModel(const Vector3m& position, const std::string& model) = 0;

	/** Supprime l'objet graphique passé en paramètres. Le pointeur
	 * ne peut plus être utilisé après l'appel de cette méthode. */
	virtual void remove(IGraphicalInstance* object) = 0;
};

#endif // ROOT_IGRAPHICALCONTEXT_H
