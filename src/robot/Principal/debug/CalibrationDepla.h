// CalibrationDepla.h

#ifndef CALIBRATION_DEPLA_H
#define CALIBRATION_DEPLA_H

#include "../librobot/libRobot.h"

class CalibrationDepla : public StrategiePrincipal {
public:
	CalibrationDepla(int argc, char* argv[])
	        : StrategiePrincipal({argv, argv + argc}, RobotColor::Green)
	        , _deplacement(this->getRobot().getCarte<DEPLACEMENT>()) {
		_angleBrutRobot = 0.0f;
		_angleBrutRobot_Avant = 0.0f;
		_distD_Apres = 0.0f;
		_distG_Apres = 0.0f;
		_distD_Avant = 0.0f;
		_distG_Avant = 0.0f;
		_rapportEntreAxe_Reel_sur_Mesure = 0.0f;
		_entreAxe = 0.0f;
		_diamRoueD = 0.0f;
		_diamRoueG = 0.0f;
		_choix = 0;
		_facteurEchelle_Reel_sur_Mesure = 0.0f;
		_rapport_D_sur_G = 0.0f;
		_distanceParcourue = 0.0f;
	}
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
	virtual void executer() override;
	virtual void arreter() override;

private:
	void attendreFinDeplacement(Duration timeout = 3_s);
	void attendreBlocage();

	CarteInfo<DEPLACEMENT>::typeCarte& _deplacement;
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