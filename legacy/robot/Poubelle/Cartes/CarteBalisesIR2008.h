/*********************************************************
 * CarteBalisesIR2008.cpp *
 * Spécifie la classe de la carte des balises IR 2008 *
 *********************************************************/

#ifndef _CARTE_BALISES_IR_2008_H_
#define _CARTE_BALISES_IR_2008_H_

#include "Carte.h"

class CarteBalisesIR2008 : public Carte {
public:
	CarteBalisesIR2008(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une mise à jour des récepteurs
	void actualiserRecepteurs();

	// Vérifie que les récepteurs sont à jour
	bool recepteursActualises();

	// Liste les récepteurs captant une balise donnée
	uint16_t lireRecepteurs();

	// Active l'envoi automatique des mesures
	void activerEnvoiAuto();

	// Desactive l'envoi automatique des mesures
	void desactiverEnvoiAuto();

private:
	std::atomic<uint16_t> _recepteurs;
	std::atomic_bool _recepteursOk;
};

#endif
