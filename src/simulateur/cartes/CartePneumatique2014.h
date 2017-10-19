/************************************************************
 * CartePneumatique2014.h *
 * Spécifie la classe gérant la carte PNEUMATIQUE *
 ************************************************************/

#ifndef _CARTE_PNEUMATIQUE2014_H_
#define _CARTE_PNEUMATIQUE2014_H_

#include "../simulateur/robot/Carte.h"
#include "IPneumatique.h"

class CartePneumatique2014 : public Carte {
public:
	CartePneumatique2014(unsigned char id, RobotLogic& robot, IPneumatique& iPneumatique);
	virtual ~CartePneumatique2014() = default;

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

private:
	IPneumatique& _iPneumatique;
};

#endif


/* infos de la part des élecs

---------   MESSAGES PC -> Carte   -----------

. Ping:
Type: PING (0x0)
Data: 0x55

. Changer l'etat de la pompe:
Type: CHANGE_ETAT (0x1)
Data: Desactivation: 0x0
      Activation: 0x1

. Demande de lecture de l'etat de la pompe:
Type: SEND_ETAT (0x2)
Data: /

---------   MESSAGES Carte -> PC   -----------

. Pong:
Type: PING (0x0)
Data: 0x55

. lecture de l'etat de la pompe: //Message envoyé
// soit après demande de lecture de l'etat
// soit après modification de l'etat
Type: SEND_ETAT (0x2)
Data: Pompe desactivee: 0x0
      Pompe Activee: 0x1

 */