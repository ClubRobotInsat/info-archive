//
// Created by louis on 30/11/17.
//

#ifndef ROOT_WEBOBJECTHANDLER_H
#define ROOT_WEBOBJECTHANDLER_H

#include "../IGraphicalInstance.h"
#include "SimulationToWebserver.h"

class WebGraphicalContext;

class WebObjectHandler : public IGraphicalInstance {
public:
	WebObjectHandler(WebGraphicalContext& context, int id);

	int getId() const {
		return _id;
	}

	void setAdditionalContent(JSON content);

	Color3f getColor() override;
	void setColor(const Color3f& color) override;
	void setScale(const Vector3f& scale);

	void update(Object3D& parent) override;

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

private:
	WebGraphicalContext& _context;
	int _id;

	Vector3m _position;
	Vector3f _scale;
	Vector3ang _rotation;

	Color3f _color;
	JSON _additionalContent;

	// TODO peut-être une nouvelle classe (héritée de WebObjectHandler) pour les modèles ?

	bool _created;
	bool _changed;

	Matrix4f generateTransformationMatrix();

	friend class WebGraphicalContext;
};


#endif // ROOT_WEBOBJECTHANDLER_H
