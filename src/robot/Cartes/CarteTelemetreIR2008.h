/*****************************************************************
 * CarteTelemetreIR2008.h *
 * Spécifie la classe gérant les telemetres IR 2008 *
 *****************************************************************/

#ifndef _CARTE_TELEMETRE_IR_2008_H_
#define _CARTE_TELEMETRE_IR_2008_H_

#include "Carte.h"

#define NOMBRE_TELEMETRE 3

class CarteTelemetreIR2008 : public Carte {
public:
	CarteTelemetreIR2008(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande au télémètre d'envoyer une mesure
	void actualiserTelemetre(uint8_t numero_telemetre);

	// Vérifie que la mesure du télémètre est à jour
	bool telemetreActualise(uint8_t numero_telemetre);

	// Récupère la mesure du télémètre
	uint16_t lireTelemetre(uint8_t numero_telemetre);

	void activerEnvoiAuto();
	void desactiverEnvoiAuto();

private:
	std::atomic<uint16_t> _telemetre[NOMBRE_TELEMETRE];
	std::atomic_bool _telemetreOk[NOMBRE_TELEMETRE];
};

#endif
