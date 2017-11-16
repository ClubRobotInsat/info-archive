//
// Created by paul on 19/04/16.
//

#ifndef ROOT_MODULEVALVE_H
#define ROOT_MODULEVALVE_H
#include "../cartes/IVanne.h"

class Robot2016;

class ModuleValve : public IVanne {
public:
	ModuleValve(Robot2016& robot);


	virtual void setValveOn(unsigned char idVanne) override;
	virtual void setValveOff(unsigned char idVanne) override;
	// True si vanne on, false si vanne off
	virtual bool getValveStatus(unsigned char idVanne) const override;
	// TRUE si toutes vannes activées ; FALSE si au moins une vanne désactivée
	virtual bool getAllValveStatus() const override;

	virtual void activateAllValves() override;
	virtual void desactivateAllValves() override;


private:
	int infoToElecID(unsigned char idVanne) const;
	Robot2016& _robot;
};


#endif // ROOT_MODULEVALVE_H
