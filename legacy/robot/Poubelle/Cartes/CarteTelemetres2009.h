/*****************************************************************
 * CarteTelemetres2009.h *
 * Spécifie la classe gérant les telemetres IR 2009 *
 *****************************************************************/

#ifndef _CARTE_TELEMETRES_2009_H_
#define _CARTE_TELEMETRES_2009_H_

#include "Carte.h"

class CarteTelemetres2009 : public Carte {
public:
	static const int NB_MAX_TELEMETRES = 3;

	CarteTelemetres2009(Commun::CAN& can, uint8_t id, int nbTelemetres = NB_MAX_TELEMETRES);

	virtual void traiterMessage(Trame const& message) override;

	// Demande au télémètre d'envoyer une mesure
	void actualiserTelemetre(uint8_t numeroTelemetre);

	// Vérifie que la mesure du télémètre est à jour
	bool telemetreActualise(uint8_t numeroTelemetre);

	// Récupère la distance mesurée par le télémètre
	uint16_t lireDistanceTelemetre(uint8_t numeroTelemetre);

	// Active l'envoi de trames toutes les 50 ms
	void activerEnvoiAuto();

	// Désactive l'envoi de trames toutes les 50 ms
	void desactiverEnvoiAuto();

private:
	std::atomic<uint16_t> _distances[NB_MAX_TELEMETRES];
	std::atomic_bool _telemetresOk[NB_MAX_TELEMETRES];

	int _nbTelemetres; // Nombre de télémètres gérés par cette instance de la carte
};

#endif // _CARTE_TELEMETRES_2009_H_
