// MecaManager2015.h

#ifndef _MECAMANAGER_2015_H_
#define _MECAMANAGER_2015_H_

#include "../../../commun/ConstantesCommunes.h"
#include "../../../commun/RobotPrincipal/Constantes.h"
#include "../../Commun/Servo.h"
#include "Ascenseur.h"
#include "Robot.h"
#include <functional>

// Le robot peut stocker 3 cylindres à la fois
enum PositionStockage {
	HAUT = 0,
	BAS = 1,
	BRAS = 2,

	NBR = 3
};

class MecaManagerPrincipal {
public:
	MecaManagerPrincipal(RobotPrincipal& robot);
	virtual ~MecaManagerPrincipal();

	/// CONSTANTES ///
	static const Duration TIMEOUT;

	/// HAUT NIVEAU ///
	[[deprecated]] ResultatAction testerPetri();

	void couperMeca();

	// Actions pour les trucs aux fesses du robot TODO

	// Actions pour l'ascenseur du robot
	ResultatAction ascenseurStockageHaut();
	ResultatAction ascenseurStockageBas();
	ResultatAction ascenseurCylindreSol();
	ResultatAction ascenseurAjustagePourSaisieCylindreBas();
	ResultatAction ascenseurPoserRails();

	// Actions pour le bras
	ResultatAction orienterPinceDe(Angle val);
	ResultatAction pencherPindeDe(Angle val);
	ResultatAction ouvrirPinceDe(Angle val);
	ResultatAction ouvrirFesseGaucheDe(Angle val);
	ResultatAction ouvrirFesseDroiteDe(Angle val);
	ResultatAction bougerFuseeDe(Angle val);

	// Actions pour la pince du robot
	ResultatAction ouvrirPince();
	ResultatAction ouvrirPinceMax();
	ResultatAction ouvrirPinceMoitie();
	ResultatAction fermerPince();
	ResultatAction attraperCylindre();

	ResultatAction orienterPinceAttraper();
	ResultatAction orienterPinceStocker();
	ResultatAction orienterPinceRecalage();

	ResultatAction pencherPinceBas();
	ResultatAction pencherPinceDevant();

	ResultatAction lancerEnginSpatial();
	ResultatAction initialiserEnginSpatial();

	/// Automatisation du stockage
	/**
	 * Lorsqu'un cylindre est juste devant la pince ouverte,
	 * le robot attrape ce cylindre pour le stocker dans une place libre
	 *
	 * L'action se finit avec la pince ouverte, en bas (ou fermée si 3 cylindres stockés)
	 *
	 * @return Resultat REUSSI si tout s'est bien passé (un cylindre a été rajouté au stockage)
	 * 					RATE si une action s'est mal passée pour une raison quelconque
	 */
	ResultatAction stockerCylindre();
	/**
	 * Le robot finit avec un cylindre dans sa pince fermée, en bas
	 */
	ResultatAction destockerCylindre();

	// retourne le nombre de cylindres stockés
	int getNbrCylindres();
	ResultatAction printCylindres();

	/// Gestion des fesses du robot
	ResultatAction ouvrirFesseGauche();
	ResultatAction ouvrirFesseDroite();
	ResultatAction fermerFesseGauche();
	ResultatAction fermerFesseDroite();

	Ascenseur& getAscenseur() {
		return _ascenseur;
	}

	void setCouleurServos(CarteServo::Couleur couleur);

	void setModeBlocageServos();

	// Debug : permet de tester le destocker cylindre
	void setTousCylindresStockes();

	// private:
	Angle getPositionServo(uint8_t servo, uint8_t pos);

private:
	RobotPrincipal& _robot;

	Ascenseur _ascenseur;
	// On sait quelles zones sont utilisées pour le stockage
	bool _cylindresStockes[PositionStockage::NBR] = {false, false, false};
	ResultatAction ajouterCylindre(int i);
	ResultatAction supprimerCylindre(int i);

	Commun::ServoManager _servos;
};

#endif
