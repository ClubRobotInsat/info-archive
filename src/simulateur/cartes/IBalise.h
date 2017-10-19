// IBalise.h

#ifndef IBALISE_H_
#define IBALISE_H_

#include "btBulletDynamicsCommon.h"

class IBalise {
public:
	virtual ~IBalise() {}

	virtual const Vector3m& getPosition() const = 0;
};

#endif /*IBALISE_H_*/
