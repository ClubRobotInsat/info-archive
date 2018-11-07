//
// Created by paul on 14/04/16.
//

#ifndef ROOT_CARTEELECTROVANNES2016_H
#define ROOT_CARTEELECTROVANNES2016_H
#include "../simulateur/robot/Carte.h"
#include "IVanne.h"


class CarteElectrovannes2016 : public Carte {
public:
	CarteElectrovannes2016(unsigned char id, RobotLogic& robot, IVanne& iVanne);
	virtual ~CarteElectrovannes2016() = default;

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

private:
	IVanne& _iVanne;
};


#endif // ROOT_CARTEELECTROVANNES2016_H


/* info de la part des élecs

 ---------   MESSAGES PC -> Carte   -----------

. Ping:
Type: PING (0x0)
Data: 0x55

. Activation d'une electrovanne:
Type: ACTIV (0x1)
Data: Numero d'electrovanne: 0x1->6

. Activation des six electrovannes:
type: ACTIV (0x1)
Data: 0x7

. Desactivation d'une electrovanne:
Type: DESACTIV (0x2)
Data: Numero d'electrovanne: 0x1->6

. Desactivation des six electrovannes:
Type: DESACTIV (0x2)
Data: 0x7

---------   MESSAGES Carte -> PC   -----------

. Pong:
Type: PING (0x0)
Data: 0x55

. Electrovanne activee:
Type: ACTIV (0x1)
Data: Numero d'electrovanne: 0x1->6

. Toutes les electrovannes activees:
type: ACTIV (0x1)
Data: 0x7

. electrovanne desactivee:
Type: DESACTIV (0x2)
Data: Numero d'electrovanne: 0x1->6

. toutes les electrovannes desactivee:
Type: DESACTIV (0x2)
Data: 0x7

 */