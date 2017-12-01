//
// Created by louis on 29/11/17.
//

#ifndef ROOT_IBEHAVIOR_H
#define ROOT_IBEHAVIOR_H

class Object3D;

class IBehavior {
public:
	virtual ~IBehavior() = default;

	/** Met à jour le comportement en fonction des caractéristiques
	 * de l'objet et/ou met à jour l'objet en fonction du comportement */
	virtual void update(Object3D& parent) = 0;
};


#endif // ROOT_IBEHAVIOR_H
