#ifndef CarteDetectAdv2012_H
#define CarteDetectAdv2012_H

#include "Carte.h"

class CarteDetectAdv2012 : public Carte {
public:
	CarteDetectAdv2012(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation de la position du robot adverse
	void actualiserPosition();

	// retourne vrai si la position du robot adverse est actualisee
	bool estPositionActualise();

	// retourne la coordonne en X de la position du robot adverse
	Distance getPositionX();

	// retourne la coordonne en Y de la position du robot adverse
	Distance getPositionY();

	// si vrai active l'envoie automatique de la position, angle, vitesse toutes les 0.2s
	void setModeAutomatique(bool mode);


private:
	std::atomic_bool _positionActualise;
	Distance _positionX;
	Distance _positionY;
};

#endif // CarteDetectAdv2012_H
