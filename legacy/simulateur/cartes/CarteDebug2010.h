// CarteDebug2010.h

#ifndef CARTEDEBUG2010_H_
#define CARTEDEBUG2010_H_

#include "../simu2010/Robot2010.h"
#include "../simulateur/Robot/Carte.h"

class CarteDebug2010 : public Carte {
public:
	CarteDebug2010(unsigned char id, Robot2010& robot);

	virtual ~CarteDebug2010();

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame const&) override;
};

#endif /*CARTEDEBUG2010_H_*/
