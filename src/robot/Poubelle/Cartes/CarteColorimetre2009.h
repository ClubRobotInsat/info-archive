/************************************************************
 * CarteColorimetre2009.h *
 * Spécifie la classe gérant les colorimetres 2007/2008 *
 ************************************************************/

#ifndef _CARTE_COLORIMETRE_2009_H_
#define _CARTE_COLORIMETRE_2009_H_

#include "Carte.h"
#include <cstring>

#define NOMBRE_DE_COULEURS 3

class CarteColorimetre2009 : public Carte {
public:
	CarteColorimetre2009(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// Demande une actualisation des etats des colorimetres
	void actualiserCouleurs();

	// Demande si le capteur a bien ete actualise
	bool couleursActualise();

	// modifie le tableau de 3 shorts passé en paramètre représentant l'état d'un des colorimètres
	void getCouleurs(uint16_t* rgb);

	// accesseur sur une des couleurs d'un des colorimetres
	uint16_t getCouleurs(uint8_t couleur);

	// accesseur sur les couleurs d'un des colorimetres
	Vector3us getCouleurs();

	// Active l'envoi automatiques des mesures de couleurs du colorimetre
	void activerEnvoiAuto();

	// Desactive l'envoi automatiques des mesures de couleurs du colorimetre
	void desactiverEnvoiAuto();

	// Demande une actualisation de l'etat du colorimetre (presence d'un objet)
	void actualiserObjetDetecte();

	// Verifie si l'actualisation de l'etat a ete faite ou pas
	bool objetDetecteActualise();

	// Retourne la presence d'un objet ou non
	bool verifierObjetDetecte();

private:
	// bool _pongRecu;

	std::atomic<uint16_t> _couleurs[NOMBRE_DE_COULEURS];

	std::atomic_bool _ack;

	std::atomic_bool _objetDetecte;
	std::atomic_bool _objetDetecteOk;
};

#endif
