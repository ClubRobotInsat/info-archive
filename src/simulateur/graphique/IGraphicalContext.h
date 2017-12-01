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

	virtual void update() = 0;

	// TODO Etablir si ces méthodes renvoient des pointeurs ou des références ou des smartpointers...
	virtual IGraphicalInstance* createDefaultObject() = 0;
	virtual IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height) = 0;
	virtual IGraphicalInstance* createSphere(const Vector3m& position, Length radius) = 0;
	virtual IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions) = 0;
	virtual IGraphicalInstance* createModel(const Vector3m& position, const std::string& model) = 0;

	virtual void remove(IGraphicalInstance* object) = 0;
};

#endif // ROOT_IGRAPHICALCONTEXT_H
