//
// Created by terae on 5/3/18.
//

#ifndef ROOT_IMOTEURS_H
#define ROOT_IMOTEURS_H

#include "Commun.h"

class IMoteurs {
public:
	enum SensRotation : uint8_t { Horaire = 0, Trigo = 1 };

	virtual ~IMoteurs(){};

	/// Moteurs asservis
	// Envoi d'une position angulaire
	virtual void setEnginePositionAngle(uint8_t idCarte, uint8_t num_motor, Angle angle) = 0;

	// Envoi d'une position en tours
	virtual void setEnginePositionTurn(uint8_t idCarte, uint8_t num_motor, std::size_t nb_turns) = 0;

	// Renvoie `true` si le moteur a atteint sa position
	virtual bool isAtPosition(uint8_t idCarte, uint8_t num_motor) const = 0;

	/// Moteurs non asservis
	// Fait tourner le moteur dans le sens précisé
	virtual void setEngineOn(uint8_t idCarte, uint8_t num_motor, SensRotation) = 0;

	// Arrêt du moteur
	virtual void setEngineOff(uint8_t idCarte, uint8_t num_motor) = 0;

	/// Moteurs brushless
	virtual void setBrushlessOn(uint8_t idCarte, uint8_t num_motor) = 0;

	virtual void setBrushlessOff(uint8_t idCarte, uint8_t num_motor) = 0;
};

#endif // ROOT_IMOTEURS_H
