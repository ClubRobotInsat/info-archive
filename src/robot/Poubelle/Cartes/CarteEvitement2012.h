#ifndef _CARTE_EVITEMENT_2012_H_
#define _CARTE_EVITEMENT_2012_H_

#include "../../Commun/ICapteurs.h"
#include "Carte.h"

class CarteEvitement2012 : public Carte {
public:
	// La carte récupère la position du robot car elle l'envoie aux cartes
	CarteEvitement2012(Commun::CAN& can, uint8_t id, Commun::ICapteurs* robot);

	virtual void traiterMessage(Trame const& message) override;

	// Fait redémarrer le PIC
	void reset();

	// Demande si l'initialisation s'est terminée
	void demandeEstPret();

	// Retourne estPret
	bool getEstPret();

	// Demander la position de l'adversaire numAdv (0 ou 1)
	void demandePositionAdv(uint8_t numAdv);

	void envoyerPositionHyperion();

	// Met à jour la config (depuis la carte IHM)
	void setConfig(uint8_t envoiAuto, uint8_t petitRobot, uint8_t cataRobots, uint8_t cataExterne);

	// retourne la coordonne en X de la position du robot adverse en mm
	double getPositionAdversaireX(uint8_t numAdv);

	// retourne la coordonne en Y de la position du robot adverse en mm
	double getPositionAdversaireY(uint8_t numAdv);

private:
	/// Pointeur vers le robot ; la création et la destruction de ce robot n'est PAS à la charge de l'environnement.
	/// On utilise une interface pour ne permettre que la lecture des capteurs par l'environnement.
	Commun::ICapteurs* _robot;

	typedef struct {
		double x;
		double y;
	} COORD;

	std::atomic_bool _estPret;
	COORD _positionAdversaire[2];

	// ID des messages pour la carte évitement 2012

	enum IDMessages { RESET = 15, CONFIG = 7, EST_PRET = 3, DEMANDE = 6, ROBOT0 = 5, ROBOT1 = 4 };
};

#endif
