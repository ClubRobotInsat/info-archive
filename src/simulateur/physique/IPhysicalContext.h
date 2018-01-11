//
// Created by louis on 29/11/17.
//

#ifndef ROOT_IPHYSICALCONTEXT_H
#define ROOT_IPHYSICALCONTEXT_H

#include <MathToolbox/MathToolbox.h>
#include <Units.h>

class IPhysicalInstance;

enum BodyType { STATIC_BODY, DYNAMIC_BODY };

/**
 * Interface utilisée pour caractériser un module de simulation
 * physique du simulateur. La simulation physique peut être
 * réalisée en 2D ou en 3D. Cette interface permet d'ajouter et
 * de retirer des objets de la simulation.
 */
class IPhysicalContext {
public:
	virtual ~IPhysicalContext() = default;

	virtual void update() = 0;

	/** Crée un objet physique avec une hitbox par défaut. */
	virtual IPhysicalInstance* createDefaultObject(const Vector3m& position, BodyType type) = 0;

	/** Crée un objet physique de forme rectangulaire ou cubique.
	 * Les dimensions sont passées en paramètres. */
	virtual IPhysicalInstance* createCuboid(const Vector3m& position, Mass mass, BodyType type, const Vector3m& dimensions) = 0;

	/** Crée un objet physique avec une hitbox cylindrique. */
	virtual IPhysicalInstance* createCylinder(const Vector3m& position, Mass mass, BodyType type, Length radius, Length height) = 0;

	/** Supprime l'instance passée en paramètres. Le destructeur
	 * est appelé, l'instance devient donc inutilisable. */
	virtual void remove(IPhysicalInstance* object) = 0;
};

#endif // ROOT_IPHYSICALCONTEXT_H
