// IBarrieres.h

#ifndef IBARRIERES_H_
#define IBARRIERES_H_

class IBarrieres {
public:
	virtual ~IBarrieres(){};

	// Accesseur sur l'etat des barrieres
	virtual bool getEtatBarrieres(unsigned char idCarte, unsigned char numeroBarriere) const = 0;
};

#endif /*IBARRIERES_H_*/
