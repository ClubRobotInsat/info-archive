// ICodesBarres.h

#ifndef ICODESBARRES_H_
#define ICODESBARRES_H_

class ICodesBarres {
public:
	enum MessageLu { REINE = 0, ROI = 1, RIEN = 2 };

	virtual ~ICodesBarres() {}

	// Accesseur sur l'etat des codes barres
	virtual MessageLu getEtatCodeBarre(unsigned char numeroContact) const = 0;
};

#endif /*ICODESBARRES_H_*/
