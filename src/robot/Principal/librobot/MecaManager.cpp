// MecaManagerPrincipal2015.cpp

#include "MecaManager.h"
//#include "../petri/src/Test.h"
#include <cassert>
#include <petri/Runtime/Cpp/DebugServer.h>

#define ADD(Vector, Function, ...) Vector.push_back(std::bind(&MecaManagerPrincipal::Function, this, ##__VA_ARGS__))
#define NBR_PLACES_STOCKAGE 2

typedef std::function<ResultatAction(void)> fun_ra;

inline ResultatAction combiner_actions(std::vector<fun_ra> actions) {
	ResultatAction result = ResultatAction::REUSSI;

	for(fun_ra action : actions) {
		if(result != ResultatAction::REUSSI && result != ResultatAction::TIMEOUT)
			return result;
		result = action();
	}
	return result;
}

#define SLEEP_TIME_US 100
const Duration MecaManagerPrincipal::TIMEOUT = 4_s;

using namespace Constantes;
using namespace ConstantesPrincipal;
using namespace IDCartesPrincipal;

MecaManagerPrincipal::MecaManagerPrincipal(RobotPrincipal& robot)
        : _robot(robot)
        , _ascenseur(_robot.getCarte<ASCENSEUR>(), positionAscenseur)
        , _servos(robot.getCarte<SERVOS>(),
                  std::bind(&MecaManagerPrincipal::getPositionServo, this, std::placeholders::_1, std::placeholders::_2))
/*
        , _servoX(_robot.getCarte<SERVO_X>(),
                                    std::bind(&MecaManagerPrincipal::getPositionServoX, this, std::placeholders::_1,
   std::placeholders::_2))
        , _pompes{&_robot.getCarte<PNEUMATIQUE_1>(), &_robot.getCarte<PNEUMATIQUE_2>() */
{

	logDebug("Initialisation de la meca");
	// for(int i = 0 ; i < 5 ; ++i) {
	//  std::cout << "n°" << i << std::endl;
	this->setModeBlocageServos();

	this->pencherPinceDevant();
	this->orienterPinceAttraper();
	this->ouvrirPince();
	_ascenseur.initialiser(SensRotation::Trigo);
	this->ascenseurCylindreSol();
	this->fermerPince();
	this->orienterPinceStocker();
	this->fermerFesseGauche();
	this->fermerFesseDroite();

	// logDebug4("init de la fusee : ", this->bougerFuseeDe(360_deg));
	this->initialiserEnginSpatial();
	//}

	for(int i = 0; i < PositionStockage::NBR - NBR_PLACES_STOCKAGE; ++i) {
		_cylindresStockes[i] = true;
	}
	logDebug("Fin initialisation Méca");
}

MecaManagerPrincipal::~MecaManagerPrincipal() {
	this->couperMeca();
}

void MecaManagerPrincipal::couperMeca() {
	this->ouvrirPince();
}

ResultatAction MecaManagerPrincipal::ascenseurStockageHaut() {
	return this->_ascenseur.allerAPosition(PositionAscenseur::STOCKAGE_HAUT);
}

ResultatAction MecaManagerPrincipal::ascenseurStockageBas() {
	return this->_ascenseur.allerAPosition(PositionAscenseur::STOCKAGE_BAS);
}

ResultatAction MecaManagerPrincipal::ascenseurCylindreSol() {
	auto result = this->_ascenseur.allerAPosition(PositionAscenseur::SAISIE_CYLINDRE_SOL);
	if(result == ResultatAction::REUSSI || result == ResultatAction::BLOQUE)
		return ResultatAction::REUSSI;
	return result;
}

ResultatAction MecaManagerPrincipal::ascenseurAjustagePourSaisieCylindreBas() {
	return this->_ascenseur.allerAPosition(PositionAscenseur::SAISIE_CYLINDRE_STOCKAGE_BAS);
}

ResultatAction MecaManagerPrincipal::ascenseurPoserRails() {
	return this->_ascenseur.allerAPosition(PositionAscenseur::POSER_DANS_RAILS);
}


ResultatAction MecaManagerPrincipal::ouvrirPince() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::PINCE), enumToInt(ConstantesPrincipal::Pince::OUVERT));
}

ResultatAction MecaManagerPrincipal::ouvrirPinceMax() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::PINCE), enumToInt(ConstantesPrincipal::Pince::OUVERT_MAX));
}

ResultatAction MecaManagerPrincipal::ouvrirPinceMoitie() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::PINCE), enumToInt(ConstantesPrincipal::Pince::SEMI_OUVERT));
}

ResultatAction MecaManagerPrincipal::fermerPince() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::PINCE), enumToInt(ConstantesPrincipal::Pince::FERME));
}

ResultatAction MecaManagerPrincipal::attraperCylindre() {
	auto action = fermerPince();
	logDebug1("fermerPince = ", action);
	switch(action) {
		case ResultatAction::TIMEOUT:
			return action;
		case ResultatAction::BLOQUE:
			return ResultatAction::REUSSI;
		case ResultatAction::REUSSI:
			return ResultatAction::REUSSI; // à supprimer soon
		default:
			return ResultatAction::RATE;
	}
}

ResultatAction MecaManagerPrincipal::orienterPinceAttraper() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::LACET), enumToInt(ConstantesPrincipal::Lacet::ATTRAPER));
}

ResultatAction MecaManagerPrincipal::orienterPinceStocker() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::LACET), enumToInt(ConstantesPrincipal::Lacet::RANGER));
}

ResultatAction MecaManagerPrincipal::orienterPinceRecalage() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::LACET), enumToInt(ConstantesPrincipal::Lacet::RECALAGE));
}

ResultatAction MecaManagerPrincipal::pencherPinceBas() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::TANGAGE), enumToInt(ConstantesPrincipal::Tangage::BAS));
}

ResultatAction MecaManagerPrincipal::pencherPinceDevant() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::TANGAGE), enumToInt(ConstantesPrincipal::Tangage::DEVANT));
}

ResultatAction MecaManagerPrincipal::lancerEnginSpatial() {
	logDebug("Lancement de la fusée !");
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::FUSEE), enumToInt(ConstantesPrincipal::Fusee::RELACHEMENT));
}

ResultatAction MecaManagerPrincipal::initialiserEnginSpatial() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::FUSEE), enumToInt(ConstantesPrincipal::Fusee::STOCKAGE));
}

ResultatAction MecaManagerPrincipal::ajouterCylindre(int i) {
	_cylindresStockes[i] = true;
	return ResultatAction::REUSSI;
}

ResultatAction MecaManagerPrincipal::supprimerCylindre(int i) {
	_cylindresStockes[i] = false;
	return ResultatAction::REUSSI;
}

ResultatAction MecaManagerPrincipal::stockerCylindre() {
	std::vector<fun_ra> actions; // toutes les fonctions enchaînées

	for(int i = PositionStockage::NBR - NBR_PLACES_STOCKAGE; i < PositionStockage::NBR; ++i) {
		if(!_cylindresStockes[i]) {
			ADD(actions, ascenseurCylindreSol);
			ADD(actions, orienterPinceAttraper);
			ADD(actions, pencherPinceDevant);
			ADD(actions, attraperCylindre);

			switch((PositionStockage)i) {
				case BAS:
					ADD(actions, ascenseurStockageBas);
					// ADD(actions, ouvrirPinceMoitie);
					ADD(actions, orienterPinceRecalage);
					ADD(actions, ouvrirPinceMax);
					ADD(actions, ascenseurCylindreSol);
					ADD(actions, orienterPinceStocker);
					ADD(actions, orienterPinceAttraper);
					ADD(actions, ouvrirPince);
					break;

				case HAUT:
					ADD(actions, ascenseurStockageBas);
					ADD(actions, orienterPinceStocker);
					ADD(actions, ouvrirPinceMoitie);
					ADD(actions, ascenseurCylindreSol);
					ADD(actions, attraperCylindre);
					ADD(actions, orienterPinceAttraper);
					ADD(actions, ascenseurStockageHaut);
					ADD(actions, orienterPinceStocker);
					// Ajouter des positions de stockage intermédiaires?
					ADD(actions, ouvrirPinceMax);
					ADD(actions, orienterPinceAttraper);
					ADD(actions, ascenseurCylindreSol);
					ADD(actions, ouvrirPince);
					break;

				default: // BRAS
					ADD(actions, ascenseurStockageBas);
					break;
			}
			ADD(actions, ajouterCylindre, i);
			ADD(actions, printCylindres);
			break;
		}
	}
	return combiner_actions(actions);
}

ResultatAction MecaManagerPrincipal::destockerCylindre() {
	std::vector<fun_ra> actions;

	for(int i = PositionStockage::NBR - 1; i >= PositionStockage::NBR - NBR_PLACES_STOCKAGE; --i) {
		if(_cylindresStockes[i]) {
			ADD(actions, orienterPinceAttraper);
			ADD(actions, pencherPinceDevant);
			switch((PositionStockage)i) {
				case BAS:
					ADD(actions, ascenseurCylindreSol);
					ADD(actions, ouvrirPinceMax);
					ADD(actions, orienterPinceRecalage);
					ADD(actions, attraperCylindre);
					ADD(actions, orienterPinceAttraper);
					break;

				case HAUT:
					ADD(actions, ascenseurStockageHaut);
					ADD(actions, ouvrirPinceMax);
					ADD(actions, orienterPinceStocker);
					ADD(actions, attraperCylindre);
					ADD(actions, orienterPinceAttraper);
					break;

				case BRAS:
					break;

				default:
					break;
			}
			ADD(actions, supprimerCylindre, i);
			ADD(actions, printCylindres);
			break;
		}
	}
	return combiner_actions(actions);
}

int MecaManagerPrincipal::getNbrCylindres() {
	int N = 0;
	for(int i = 0; i < PositionStockage::NBR; ++i)
		if(_cylindresStockes[i])
			N++;
	return N - PositionStockage::NBR + NBR_PLACES_STOCKAGE;
}

ResultatAction MecaManagerPrincipal::printCylindres() {
	logDebug1("je possede tant de cylindres : ", getNbrCylindres());
	return ResultatAction::REUSSI;
}

ResultatAction MecaManagerPrincipal::orienterPinceDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::LACET), val);
}
ResultatAction MecaManagerPrincipal::pencherPindeDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::TANGAGE), val);
}
ResultatAction MecaManagerPrincipal::ouvrirPinceDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::PINCE), val);
}
ResultatAction MecaManagerPrincipal::ouvrirFesseGaucheDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::GAUCHE), val);
}
ResultatAction MecaManagerPrincipal::ouvrirFesseDroiteDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::DROITE), val);
}
ResultatAction MecaManagerPrincipal::bougerFuseeDe(Angle val) {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::FUSEE), val);
}

ResultatAction MecaManagerPrincipal::ouvrirFesseGauche() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::GAUCHE), enumToInt(ConstantesPrincipal::Gauche::OUVERT));
}

ResultatAction MecaManagerPrincipal::ouvrirFesseDroite() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::DROITE), enumToInt(ConstantesPrincipal::Droite::OUVERT));
}

ResultatAction MecaManagerPrincipal::fermerFesseGauche() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::GAUCHE), enumToInt(ConstantesPrincipal::Gauche::FERME));
}

ResultatAction MecaManagerPrincipal::fermerFesseDroite() {
	return this->_servos.positionnerServoBloquant(enumToInt(Servo::DROITE), enumToInt(ConstantesPrincipal::Droite::FERME));
}

Angle MecaManagerPrincipal::getPositionServo(uint8_t servo, uint8_t pos) {
	switch(static_cast<Servo>(servo)) {
		case Servo::PINCE:
			return ConstantesPrincipal::positionPince[pos];

		case Servo::FUSEE:
			return ConstantesPrincipal::positionFusee[pos];

		case Servo::DROITE:
			return ConstantesPrincipal::positionDroite[pos];

		case Servo::GAUCHE:
			return ConstantesPrincipal::positionGauche[pos];

		case Servo::TANGAGE:
			return ConstantesPrincipal::positionTangage[pos];

		case Servo::LACET:
			return ConstantesPrincipal::positionLacet[pos];

		default:
			throw std::string("unimplemented");
	}
}

void MecaManagerPrincipal::setModeBlocageServos() {
	auto avec = CarteServo::ModeBlocage::MAINTIEN_SUR_BLOCAGE;
	auto sans = CarteServo::ModeBlocage::DEBLOCAGE;

	_servos.reglerModeBlocage(enumToInt(Servo::PINCE), avec);
	_servos.reglerModeBlocage(enumToInt(Servo::LACET), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::TANGAGE), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::FUSEE), avec);
	_servos.reglerModeBlocage(enumToInt(Servo::GAUCHE), sans);
	_servos.reglerModeBlocage(enumToInt(Servo::DROITE), sans);
}

void MecaManagerPrincipal::setCouleurServos(CarteServo::Couleur couleur) {
	for(uint8_t i = 0; i < enumToInt(Servo::NBR); ++i)
		_servos.setCouleur(i, couleur);
}

void MecaManagerPrincipal::setTousCylindresStockes() {
	for(int i = 0; i < PositionStockage::NBR; ++i)
		_cylindresStockes[i] = true;
}