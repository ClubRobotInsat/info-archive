#ifndef ICOLORIMETRES_H_
#define ICOLORIMETRES_H_

class IColorimetres {
public:
	static const unsigned char NBR_COULEURS = 3;

	struct Couleur {
		unsigned short couleur[NBR_COULEURS];
	};

	virtual ~IColorimetres() {}

	// Accesseur sur la couleur d'un colorimetre
	virtual Couleur* getColorimetre(unsigned char numero) const = 0;
};

#endif /*ICOLORIMETRES_H_*/
