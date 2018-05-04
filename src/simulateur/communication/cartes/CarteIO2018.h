#ifndef CARTEIO2018_H_
#define CARTEIO2018_H_

#include "Carte.h"
#include "IContacteursV2.h"
#include <atomic>

#define NBCAPTEURS 255

class CarteIO2018 : public Carte {

public:
	CarteIO2018(unsigned char id, RobotLogic& robot, IContacteursV2& iContacteurs);

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// envoyer la reponce a un ping
	virtual void envoyerPong();

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


	// erreur le numero du contacteur est trop grand
	class ErreurNumContact : public std::runtime_error {
	public:
		ErreurNumContact(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " + to_string((short)NBCAPTEURS)) {}
	};

	// erreur le Nombre de contacteur est trop grand
	class ErreurNbrContact : public std::runtime_error {
	public:
		ErreurNbrContact()
		        : std::runtime_error("le nombre de contacteurs (" + to_string((short)NBCAPTEURS) +
		                             ") est trop grand pour etre stocker sur un char ") {}
	};


private:
	IContacteursV2& _iContacteurs;

	std::atomic_bool _capteurs_actualises[NBCAPTEURS];
	std::atomic_bool _capteurs[NBCAPTEURS];
};

#endif /* CARTEIO2018_H_ */
