// ITelemetre.h

#ifndef ITELEMETRES_H_
#define ITELEMETRES_H_

#include "../PhysicalToBullet.h"

class ITelemetres {
public:
	virtual ~ITelemetres() {}

	// retourne la distance mesuree par un telemetre en dm
	virtual Distance getTelemetresDistance(unsigned char idCarte, unsigned char numero) const = 0;
};

#endif /*ITELEMETRES_H_*/
