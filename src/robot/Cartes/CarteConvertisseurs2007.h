/*****************************************************************
 * CarteConvertisseurs2007.h *
 * Spécifie la classe gérant les convertisseurs A/N 2006/2007 *
 *****************************************************************/

#ifndef _CARTE_CONVERTISSEURS_2007_H_
#define _CARTE_CONVERTISSEURS_2007_H_

#include "Carte.h"

class CarteConvertisseurs2007 : public Carte {
public:
	CarteConvertisseurs2007(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande au télémètre d'envoyer une mesure
	void actualiserTelemetre();

	// Vérifie que la mesure du télémètre est à jour
	bool telemetreActualise();

	// Récupère la mesure du télémètre
	uint16_t lireTelemetre();

	// Demande au capteur de luminosité d'envoyer une mesure
	void actualiserLuminosite();

	// Vérifie que la mesure du capteur de luminosité est à jour
	bool luminositeActualisee();

	// Récupère la mesure du capteur de luminosité
	uint16_t lireLuminosite();

private:
	std::atomic<uint16_t> _telemetre;
	std::atomic_bool _telemetreOk;

	std::atomic<uint16_t> _luminosite;
	std::atomic_bool _luminositeOk;
};

#endif
