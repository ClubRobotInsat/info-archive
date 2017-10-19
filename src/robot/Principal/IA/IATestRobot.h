//
// Created by scriptopathe on 26/04/16.
//

#ifndef ROOT_IATESTROBOT_H
#define ROOT_IATESTROBOT_H
#include "../librobot/libRobot.h"
#include <Commun.h>

class IATestRobot : public StrategiePrincipal {
public:
	IATestRobot(std::vector<std::string> const& args, Constantes::RobotColor color);

protected:
	void initialisation() {
		logCyan("Couleur robot : ", this->getCouleurEquipe());
		logDebug("Recalage du robot");

		repere::Coordonnees coords_robot(START_ROBOT_POSITION,
		                                 START_ROBOT_ANGLE,
		                                 getCouleurEquipe() == RobotColor::Blue ? REFERENCE_BLUE : REFERENCE_YELLOW);

		getDeplacement().setRepere(coords_robot);

		// Tirette
		logInfo("Attente tirette…");
		if(getenv("EPIA"))
			attendreTirette();
		//		getchar();
	}

	/// Contient la logique de la stratégie elle-même
	virtual void executer() override;
};


#endif // ROOT_IATESTROBOT_H
