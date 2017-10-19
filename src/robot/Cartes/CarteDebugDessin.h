/*******************************************************************
 * CarteDebugDessin.h *
 * Spécifie la classe gérant l'affichage de l'a star *
 * et l'affichage de forme geometrique comme une ligne, un disque *
 *******************************************************************/

#ifndef _CARTE_DEBUG_DESSIN_H_
#define _CARTE_DEBUG_DESSIN_H_

#include "Carte.h"

class CarteDebugDessin : public Carte {
public:
	enum Couleur : uint8_t {
		COULEUR_ROUGE = 0xC3,
		COULEUR_VERT = 0x33,
		COULEUR_BLEU = 0x0F,
		COULEUR_BLANC = 0xFF,
		COULEUR_NOIR = 0x03,
		COULEUR_TRANSPARENT = 0x00,
		COULEUR_ORANGE1 = 0xF2, // Voir Environnement2015 à la révision 4433
		COULEUR_ORANGE2 = 0xA2, // idem
		COULEUR_ORANGE3 = 0x52, // idem
	};

	CarteDebugDessin(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// modifie le nombre de case de la grille
	void setTailleGrille(Vector2u16 taille);

	// modifie la position d'affichage de la grille
	void setCoordExtremite(Vector2m coordHautGauche, Vector2m coordBasDroit);

	// modifie la hauteur d'affichage de la grille
	void setHauteur(Distance hauteur);

	// modifie la couleur d'une case
	void setCouleurCase(Couleur couleur);

	// acceder a l'etat dactivation de l'affichage des dessin
	bool getEtatActivation();

private:
	// taille de la grille (en nbr case)
	Vector2u16 _grilleTaille;

	// coordonnees courante dans la grille par rapport a la recpetion des trames de l'etat de la grille
	// unsigned char _grilleCoordX = 0, _grilleCoordY = 0;

	// stokage temporaire des 8 couleurs de la grille a envoyer au robot
	uint8_t _couleurAEnvoyer[8];
	uint8_t _couleurAEnvoyerCourant;

	// numero de la case a envoyer
	uint8_t _numCaseAEnvoyer;

	// activation/desactivation de l'affichage des dessin
	std::atomic_bool _activationDessin;
	// Socket _simuSocket;
};

#endif
