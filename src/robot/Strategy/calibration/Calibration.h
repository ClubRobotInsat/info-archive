// CalibrationDepla.h

#ifndef CALIBRATION_DEPLA_H
#define CALIBRATION_DEPLA_H

#include <log/Log.h>

#include "../AbstractStrategy.h"

using namespace Strategy;

class CalibrationDepla {
public:
	CalibrationDepla(int argc, char* argv[]);
	virtual ~CalibrationDepla() {}

	// éviter de devoir appuyer sur le bouton d'arret d'urgence si le robot fait du caca
	void desactiverPuisActiverAsserv();

	void pid();
	void saveDatas();
	void entreAxes();
	void facteurEchelle();
	void diametres();
	void setDefaultParams();
	void displayCurrentSate();
	void testDeplacement();

	void diametresAuto();
	void facteurEchelleAuto();
	void entreAxesAuto();

	/// Contient la logique de la stratégie elle-même
	[[noreturn]] void execute();

private:
	Interfacer::NavigationInterfacer& navigation() {
		return _robot->get_interfacer<Interfacer::NavigationInterfacer>();
	}

	void attendreFinDeplacement(Duration timeout = 3_s);
	void attendreBlocage();

	std::shared_ptr<Interfacer::RobotManager> _robot;
	std::unique_ptr<Environment> _env;

	float _distD_Avant;
	float _distD_Apres;
	float _distG_Avant;
	float _distG_Apres;

	float _entreAxe;
	float _rapportEntreAxe_Reel_sur_Mesure;
	float _angleBrutRobot;
	float _angleBrutRobot_Avant;
	float _facteurEchelle_Reel_sur_Mesure;
	float _distanceParcourue;

	float _diamRoueG;
	float _diamRoueD;
	float _rapport_D_sur_G;

	int _choix;
};

#endif // CALIBRATION_DEPLA_H