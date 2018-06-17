//
// Created by louis on 10/09/16.
//

#include "../librobot/Strategie.h"
#include "../wiimote/Wiimote.h"
#include <Commun.h>
#include <cmath>

#ifndef ROOT_IAWIIMOTE_H
#define ROOT_IAWIIMOTE_H

//#define NUNCHUK_UTILISER_COURBES


class IAWiimote;

enum TypeDeplacement {
	AVANCER = 0,
	AVANCER_DROITE,
	TOURNER_DROITE,
	RECULER_DROITE,
	RECULER,
	RECULER_GAUCHE,
	TOURNER_GAUCHE,
	AVANCER_GAUCHE,
	IMMOBILE
};

class IAWiimote : public StrategiePrincipal, WiimoteEventHandler {
public:
	IAWiimote(std::vector<std::string> const& args);

protected:
	virtual void executer() override;

	/// Méthode appellée régulièrement pour traiter les entrées wiimote
	virtual void processWiimoteInput(WiimoteState& state) override;

	void avancer(SensAvance sens, float magnitude = 1.0);
	void stop();

	void tourner(SensRotation sens, float magnitude = 1.0);

	void setVitesseLineaire(Speed linearVelocity);
	void setVitesseAngulaire(AngularSpeed angularVelocity);

	TypeDeplacement getTypeDeplacement(float angleNunchuk);

private:
	WiimoteInputProvider inputProvider;

	TypeDeplacement typeDeplacement;

	float angleLu;
	float magnitudeLue;
	float magnitudeOld = 0.0;

	bool linearVelocityPushed = false;
	bool angularVelocityPushed = false;

	bool linearVelocityVeryFast = false;
	bool angularVelocityVeryFast = false;
	bool linearVelocityFast = true;
	bool angularVelocityFast = true;

	const Vector2m _initial_position = toVec2(ConstantesPrincipal::START_ROBOT_POSITION);
	const Angle _initial_angle = ConstantesPrincipal::START_ROBOT_ANGLE;
	bool _avaleurs_actives = false;
};

#endif // ROOT_IAWIIMOTE_H
