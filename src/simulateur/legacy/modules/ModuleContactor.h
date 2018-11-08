//
// Created by paul on 14/04/16.
//

#ifndef ROOT_MODULECONTACTOR_H
#define ROOT_MODULECONTACTOR_H


#include "../cartes/IContacteursV2.h"

class Robot2018;

class ModuleContactor : public IContacteursV2 {
public:
	ModuleContactor(Robot2018& robot2018);
	// TODO : virer les idCarte qui servent à rien...
	virtual bool getContactorState(unsigned char numeroContact, unsigned char idCarte) const override;

	virtual void setContactorState(unsigned char numeroContact, unsigned char idCarte) override;

private:
	int infoToElecID(unsigned char cardID) const;
	Robot2018& _robot;
};


#endif // ROOT_MODULECONTACTOR_H
