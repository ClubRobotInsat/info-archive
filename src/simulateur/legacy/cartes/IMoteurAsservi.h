// IMoteurAsservis.h

#ifndef IMOTEURASSERVI_H_
#define IMOTEURASSERVI_H_

#include "Commun.h"

class IMoteurAsservi {
public:
	virtual ~IMoteurAsservi(){};

	// modifier la vitesse angulaire
	virtual void setEngineAngularSpeed(unsigned char idCarte, AngularSpeed speed) = 0;

	// acceder a la valeur de l'angle entre -PI et PI
	virtual Angle getEngineAngle(unsigned char idCarte) = 0;

	// Renvoie true si le moteur est bloque
	virtual bool isBlocked(unsigned char idCarte) const = 0;

	// acceder a coefficient de convertion entre la vitesse angulaire et la vitesse lineaire
	// retourner 1.0 pour un moteur qui est utilise en rotation
	// ou s'il est utilise en lineaire avec les bonne valeur pour les
	// retourner un truc qui va bien de maniere empirique quand le moteur est utiliser en lineaire
	virtual float getCoeffConversionAngularLinearSpeed() const = 0;
};

#endif // IMOTEURASSERVIS_H_
