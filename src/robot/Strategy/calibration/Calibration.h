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

	PhysicalRobot::NavigationParameters& navigation_parameters() {
		return _module_manager->get_module<PhysicalRobot::NavigationParameters>();
	}

	void check_physical_blocking_appeared();

	std::shared_ptr<Interfacer::RobotManager> _robot;
	Distance _length_robot;
	std::shared_ptr<PhysicalRobot::ModuleManager> _module_manager;
	std::unique_ptr<Environment> _env;

	Distance _distD_Avant;
	Distance _distD_Apres;
	Distance _distG_Avant;
	Distance _distG_Apres;

	Distance _entreAxe;
	double _rapportEntreAxe_Reel_sur_Mesure;
	Angle _angleBrutRobot_Apres;
	Angle _angleBrutRobot_Avant;
	double _facteurEchelle_Reel_sur_Mesure;
	Distance _distanceParcourue;

	Distance _diamRoueG;
	Distance _diamRoueD;
	double _rapport_D_sur_G;

	int _choix;


	const Speed REPOSITIONING_LINEAR_SPEED = 50_mm_s;

	ActionResult _res;
};

#endif // CALIBRATION_DEPLA_H