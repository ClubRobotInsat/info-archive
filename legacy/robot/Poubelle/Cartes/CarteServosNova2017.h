/******************************************************
 * CarteServosNova2017.h *
 * Spécifie la classe gérant les servos 2017 *
 ******************************************************/

#ifndef _CARTE_SERVOS_NOVA_2017_H_
#define _CARTE_SERVOS_NOVA_2017_H_

#include "CarteServoCommun.h"

class CarteServosNova2017 : public CarteServo {
public:
	enum IDMessages : uint8_t {
		PING = 0x00_b,
		REGLER_POSITION = 0x01_b,
		RETOUR_POSITION = 0x02_b,
		TERMINE = 0x03_b,
		BLOQUE = 0x04_b,
		VITESSE = 0x05_b,
		MODE_BLOCAGE = 0x06_b,
		COULEUR = 0x07_b,
	};


	CarteServosNova2017(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Règle le mode de gestion du blocage du servo concerné
	virtual void reglerModeBlocage(uint8_t servo, ModeBlocage mode) override;

	// Règle la position d'un servo
	virtual void reglerPosition(uint8_t servo, Angle angle) override;

	// Demande l'actualisation de la mesure de position d'un servo
	virtual void actualiserPosition(uint8_t servo) override;

	// Vérifie si la mesure de position d'un servo est à jour ou pas
	virtual bool verifierPositionActualisee(uint8_t servo) override;

	// Retourne la position actuelle du servo voulu
	virtual Angle lirePosition(uint8_t servo) override;

	// Vérifie si le dernier ordre envoyé au servo est exécuté ou pas encore
	virtual bool verifierDeplacementTermine(uint8_t servo) override;

	// Vérifie si le servo en question est bloqué ou pas
	virtual bool verifierServoBloque(uint8_t servo) override;

	// Règle la vitesse d'un servo
	virtual void setServoVitesse(uint8_t servo, uint8_t vitesse) override;

	void setCouleur(uint8_t servo, Couleur couleur) override;

private:
	std::atomic_bool _positionActualisee[NB_MAX_SERVOS_NOVA];
	std::mutex _mutexPositions;
	Angle _positions[NB_MAX_SERVOS_NOVA];
	std::atomic_bool _termine[NB_MAX_SERVOS_NOVA];
	std::atomic_bool _bloque[NB_MAX_SERVOS_NOVA];
};


#endif
