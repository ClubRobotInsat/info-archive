//
//  CarteServoCommun.h
//  Club Robot
//
//  Created by Rémi on 17/01/2015.
//

#ifndef Club_Robot_CarteServoCommun_h
#define Club_Robot_CarteServoCommun_h

#include "Carte.h"

#define NB_MAX_SERVOS_NOVA 8

class CarteServo : public Carte {
public:
	CarteServo(Commun::CAN& can, uint8_t id) : Carte(can, id) {}

	enum ModeBlocage : uint8_t { MAINTIEN_SUR_BLOCAGE = 1, DEBLOCAGE = 0 };

	enum Couleur : uint8_t {
		NOIR = 0x00,
		ROUGE = 0x01,
		VERT = 0x02,
		BLEU = 0x04,
		JAUNE = 0x03,
		MAGENTA = 0x05,
		CYAN = 0x06,
		BLANC = 0x07,
	};

	// Règle le mode de gestion du blocage du servo concerné
	virtual void reglerModeBlocage(uint8_t servo, ModeBlocage mode) = 0;

	// Règle la position d'un servo
	virtual void reglerPosition(uint8_t servo, Angle angle) = 0;

	// Demande l'actualisation de la mesure de position d'un servo
	virtual void actualiserPosition(uint8_t servo) = 0;

	// Vérifie si la mesure de position d'un servo est à jour ou pas
	virtual bool verifierPositionActualisee(uint8_t servo) = 0;

	// Retourne la position actuelle du servo voulu
	virtual Angle lirePosition(uint8_t servo) = 0;

	// Vérifie si le dernier ordre envoyé au servo est exécuté ou pas encore
	virtual bool verifierDeplacementTermine(uint8_t servo) = 0;

	// Vérifie si le servo en question est bloqué ou pas
	virtual bool verifierServoBloque(uint8_t servo) = 0;

	// Règle la vitesse d'un servo
	virtual void setServoVitesse(uint8_t servo, uint8_t vitesse) = 0;

	virtual void setCouleur(uint8_t servo, Couleur couleur) = 0;
};

#endif
