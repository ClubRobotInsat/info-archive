// CartePneumatique2013.h

#ifndef CartePneumatique2013_H_
#define CartePneumatique2013_H_

#include "../simulateur/robot/Carte.h"
#include "IPneumatique.h"

class CartePneumatique2013 : public Carte {
public:
	CartePneumatique2013(unsigned char id, RobotLogic& robot, IPneumatique& iPneumatique);

	virtual ~CartePneumatique2013();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// verifie si les contacteurs on ete modifies et envoie une trame si besoin
	virtual void update(Duration elapsed) override;
};

#endif /*CartePneumatique2013_H_*/
