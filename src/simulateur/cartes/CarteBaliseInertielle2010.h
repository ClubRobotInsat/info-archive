// CarteBaliseInertielle2010.h

#ifndef CARTE_BALISE_INERTIELLE2010_H_
#define CARTE_BALISE_INERTIELLE2010_H_

#include "../simulateur/Robot/Carte.h"
#include "IBaliseInertielle.h"

class CarteBaliseInertielle2010 : public Carte {
public:
	CarteBaliseInertielle2010(unsigned char id, Robot& robot, IBaliseInertielle* robotAdverse);

	virtual ~CarteBaliseInertielle2010();

	void setAdversaire(IBaliseInertielle* robot);

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	void sendPong();

	void sendPositionAdversaire();

	void sendAngleAdversaire();

	void sendVitesseAdversaire();

	void sendVitesseAngulaireAdversaire();

	virtual void update(Duration elapsed) override;

private:
	IBaliseInertielle* _robotAdverse;
	bool _envoiAutomatique; // initialise a false
};

#endif /*CARTE_BALISE_INERTIELLE2010_H_*/
