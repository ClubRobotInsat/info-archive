/*****************************************************************
 * CarteAlimentation2009.h *
 * Spécifie la classe gérant la carte alimentation 2009 *
 *****************************************************************/

#ifndef _CARTE_ALIMENTATION_2009_H_
#define _CARTE_ALIMENTATION_2009_H_

#include "Carte.h"

class CarteAlimentation2009 : public Carte {
public:
	CarteAlimentation2009(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande à la carte d'envoyer une mesure
	void actualiserInfos();

	// Vérifie que les mesures sont à jour
	bool infosActualisees();

	// Vérifie que l'adapteur est en position
	bool adaptateurPresent();

	// Récupère la mesure de la tension
	float lireTension();

	// Active l'envoi de trames toutes les secondes
	void activerEnvoiAuto();

	// Désactive l'envoi de trames toutes les secondes
	void desactiverEnvoiAuto();

private:
	std::atomic_bool _infosOk;

	std::atomic<float> _tension;
	std::atomic_bool _adaptateur;
};

#endif // _CARTE_ALIMENTATION_2009_H_
