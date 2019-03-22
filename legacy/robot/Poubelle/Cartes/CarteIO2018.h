/*
 * CarteIO2014.h
 *
 *  Created on: 20 mars 2014
 *      Author: seydoux
 */

#ifndef CARTEIO2014_H_
#define CARTEIO2014_H_

#include "Carte.h"

class CarteIO2018 : public Carte {
public:
	enum : uint8_t { NBCAPTEURS = 255 };

	CarteIO2018(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	/**
	 * Fonction HL
	 */
	// Demande l'actualisation, attend le retour du capteur et renvoie sa valeur
	bool actualiserPuisLireUnCapteur(uint8_t noCapteur);

	/**
	 * Utilitaires
	 */
	// Fonction qui demande l'actualisatio d'un capteur à la carte
	void actualiserUnCapteur(uint8_t noCapteur);
	// Retourne la dernière valeur enregistrée par la carte
	bool lireUnCapteur(uint8_t noCapteur);
	// Retourne vrai si le capteur a été mis à jour depuis la dernière demande d'actualisation
	bool capteurEstActualise(uint8_t noCapteur);

private:
	std::atomic_bool _capteurs_actualises[NBCAPTEURS];
	std::atomic_bool _capteurs[NBCAPTEURS];
};


#endif /* CARTEIO2014_H_ */
