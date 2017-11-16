//
// Created by benjamin on 17/04/16.
//

#ifndef ROOT_MODULEPNEUMATIQUE_H
#define ROOT_MODULEPNEUMATIQUE_H

#include "../../core/Simulateur.h"
#include "../cartes/IPneumatique.h"
#include <log/Log.h>

class Robot2016;

class ModulePneumatique : public IPneumatique {
public:
	ModulePneumatique(Robot2016& robot2016);

	virtual void setActivation(unsigned char idCarte) override;
	virtual void setDesactivation(unsigned char idCarte) override;
	virtual bool isActivated(unsigned char idCarte) override;

private:
	int infoToElecID(unsigned char cardID);
	Robot2016& _robot;
};

#endif // ROOT_MODULEPNEUMATIQUE_H
