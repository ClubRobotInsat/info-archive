/************************************************************
 * CarteBaliseInertielle2010.h *
 * Spécifie la classe gérant les Balises avec centralle *
 * inertielle 2009/2010 *
 ************************************************************/

#ifndef CARTEBALISEINERTIELLE2010_H_
#define CARTEBALISEINERTIELLE2010_H_

#include "Carte.h"

class CarteBaliseInertielle2010 : public Carte {
public:
	CarteBaliseInertielle2010(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation de la position du robot adverse
	void actualiserPosition();

	// retourne vrai si la position du robot adverse est actualisee
	bool estPositionActualise();

	// retourne la coordonne en X de la position du robot adverse
	Distance getPositionX();

	// retourne la coordonne en Y de la position du robot adverse
	Distance getPositionY();

	// Demande une actualisation de la position du robot adverse
	void actualiserAngle();

	// retourne vrai si l'angle du robot adverse est actualisee
	bool estAngleActualise();

	// retourne l'angle du robot adverse
	Angle getAngle();

	// Demande une actualisation de la vitesse angulaire du robot adverse
	void actualiserVitesseAngulaire();

	// retourne vrai si la vitesse angulaire du robot adverse est actualisee
	bool estVitesseAngulaireActualise();

	// retourne la vitesse angulaire du robot adverse
	AngularSpeed getVitesseAngulaire();

	// Demande une actualisation de la vitesse lineaire du robot adverse
	void actualiserVitesseLineaire();

	// retourne vrai si la vitesse lineaire du robot adverse est actualisee
	bool estVitesseLineaireActualise();

	// retourne la vitesse angulaire du robot adverse
	Speed getVitesseLineaire();

	// si vrai active l'envoie automatique de la position, angle, vitesse toutes les 0.2s
	void setModeAutomatique(bool mode);

	// lance la calibration des acceleros
	void calibration();


private:
	std::atomic_bool _positionActualise;
	std::atomic_bool _angleActualise;
	std::atomic_bool _vitesseAngulaireActualise;
	std::atomic_bool _vitesseLineaireActualise;

	Distance _positionX;
	Distance _positionY;
	Angle _angle;
	AngularSpeed _vitesseAngulaire;
	Speed _vitesseLineaire;
};

#endif /* CARTEBALISEINERTIELLE2010_H_ */
