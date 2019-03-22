#ifndef CARTEDETECTADV2012_H
#define CARTEDETECTADV2012_H

#include "../simulateur/Robot/Carte.h"
#include "ICoordonneesAdversaire.h"

class CarteDetectAdv2012 : public Carte {
public:
	CarteDetectAdv2012(unsigned char id, Robot& robot, ICoordonneesAdversaire* robotAdverse);

	virtual ~CarteDetectAdv2012();

	void setAdversaire(ICoordonneesAdversaire* robot);

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	void sendPong();

	void sendPositionAdversaire();

	void sendEstPret();

	virtual void update(Duration elapsed) override;

private:
	ICoordonneesAdversaire* _robotAdverse;
	bool _envoiAutomatique; // initialise a false
};

#endif // CARTEDETECTADV2012_H
