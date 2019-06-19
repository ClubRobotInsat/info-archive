//
//  CarteCAN_USB.h
//  Club Robot
//
//  Created by Rémi on 17/10/2014.
//

#ifndef __Club_Robot__CarteCAN_USB__
#define __Club_Robot__CarteCAN_USB__

#include "Carte.h"


class CarteCAN_USB : public Carte {
public:
	CarteCAN_USB(unsigned char id, RobotLogic& robot);

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;
};

#endif /* defined(__Club_Robot__CarteCAN_USB__) */
