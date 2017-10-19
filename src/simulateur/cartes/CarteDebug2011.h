// CarteDebug2011.h

#ifndef CARTEDEBUG2011_H_
#define CARTEDEBUG2011_H_

#include "../simu2011/titan2011/RobotTitan2011.h"
#include "../simulateur/Robot/Carte.h"

class CarteDebug2011 : public Carte {
public:
	CarteDebug2011(unsigned char id, RobotTitan2011& robot);

	virtual ~CarteDebug2011();

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame*);

private:
	RobotTitan2011& _robot2011;
};

#endif /*CARTEDEBUG2011_H_*/
