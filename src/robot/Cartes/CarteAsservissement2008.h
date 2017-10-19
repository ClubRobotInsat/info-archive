/*****************************************************************
 * CarteAsservissement2008.h *
 * Spécifie la classe gérant les moteurs asservis (pour le *
 * laser, le barillet...) *
 *****************************************************************/

#ifndef _CARTE_ASSERVISSEMENT_2008_H_
#define _CARTE_ASSERVISSEMENT_2008_H_

#include "Carte.h"

class CarteAsservissement2008 : public Carte {
public:
	CarteAsservissement2008(Commun::CAN& can, uint8_t id);

	enum TypePID : uint8_t { PID_VITESSE = 'a', PID_POSITION = 'A' };

	enum SensRotation : uint8_t { SENS_TRIGO = 1, SENS_HORAIRE = 0 };

	virtual void traiterMessage(Trame const& message) override;

	// Indique si le barillet est bloque ou pas
	bool verifierBlocage();

	// Choix d'une vitesse angulaire (en mrad/s)
	void setVitesseAngulaire(std::int16_t vitesse_angulaire, SensRotation sens);

	// Indique si la vitesse angulaire demandée a été atteinte
	bool estVitesseAtteinte();

	// Choix d'une position angulaire (en mrad)
	void setPosition(std::int16_t angle);

	// Demander de s'arreter dans la position angulaire actuelle
	void stop();

	// Indique si la position demandée a été atteinte
	bool estPositionAtteinte();

	// redefinir la valeur de l'angle pour la position courante (en mrad)
	void initialiserPosition(std::int16_t angle);

	// permet de regler les coefficients des correcteurs PID de la carte
	void reglerPID(TypePID type, std::int16_t kp, std::int16_t ki, std::int16_t kd);

	// Activer - Desactiver Asservissement
	void asservissementOnOff(bool active);

private:
	std::atomic_bool _vitesseAtteinte;
	std::atomic_bool _positionAtteinte;
	std::atomic_bool _bloque;
};

#endif
