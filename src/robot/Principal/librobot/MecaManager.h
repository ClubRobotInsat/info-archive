// MecaManager2015.h

#ifndef _MECAMANAGER_2015_H_
#define _MECAMANAGER_2015_H_

#include "../../../commun/ConstantesCommunes.h"
#include "../../../commun/RobotPrincipal/Constantes.h"
#include "../../Commun/Moteur.h"
#include "../../Commun/Servo.h"
//#include "Ascenseur.h"
#include "Robot.h"
#include <functional>


#define MAX_ELEVATOR_SIZE 5

/*// Le robot peut stocker 3 cylindres à la fois
enum PositionStockage {
    HAUT = 0,
    BAS = 1,
    BRAS = 2,

    NBR = 3
};*/

enum SensAvaleurs { AVALER = 0, RECRACHER = 1 };

class MecaManagerPrincipal {
public:
	MecaManagerPrincipal(RobotPrincipal& robot);
	virtual ~MecaManagerPrincipal();

	/// CONSTANTES ///
	static const Duration TIMEOUT;

	/// HAUT NIVEAU ///
	[[deprecated]] ResultatAction testerPetri();

	void couperMeca();

	// Actions pour l'ascenseur
	ResultatAction ouvrirPorteCube();
	ResultatAction fermerPorteCube();
	ResultatAction orienterPorteCubeDe(Angle);

	ResultatAction monterAscenseursDe(int nbr_tours);
	ResultatAction monterAscenseursDe(Angle);

	ResultatAction activerAvaleurs(SensAvaleurs);
	ResultatAction desactiverAvaleurs();

	int nbrPlaceAscenseur();

	// Actions pour les soutes
	ResultatAction ouvrirSouteD();
	ResultatAction fermerSouteD();
	ResultatAction orienterSouteDDe(Angle);

	ResultatAction activerTurbineD();
	ResultatAction desactiverTurbineD();
	bool turbineDLibre();

	ResultatAction ouvrirSouteG();
	ResultatAction fermerSouteG();
	ResultatAction orienterSouteGDe(Angle);

	ResultatAction activerTurbineG();
	ResultatAction desactiverTurbineG();
	bool turbineGLibre();

	// Actions pour l'abeille
	ResultatAction ouvrirAbeilleD();
	ResultatAction fermerAbeilleD();
	ResultatAction orienterAbeilleDDe(Angle);

	ResultatAction ouvrirAbeilleG();
	ResultatAction fermerAbeilleG();
	ResultatAction orienterAbeilleGDe(Angle);

	// Actions pour l'ascenseur du robot
	/*ResultatAction ascenseurStockageHaut();
	ResultatAction ascenseurStockageBas();
	ResultatAction ascenseurCylindreSol();
	ResultatAction ascenseurAjustagePourSaisieCylindreBas();
	ResultatAction ascenseurPoserRails();*/

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
	// ResultatAction stockerCylindre();
	/**
	 * Le robot finit avec un cylindre dans sa pince fermée, en bas
	 */
	// ResultatAction destockerCylindre();

	/*Ascenseur& getAscenseur() {
	    return _ascenseur;
	}*/

	void setCouleurServos(CarteServo::Couleur couleur);

	void setModeBlocageServos();

	// Debug : permet de tester le destocker cylindre
	// void setTousCylindresStockes();

	// private:
	Angle getPositionServo(uint8_t servo, uint8_t pos);

private:
	RobotPrincipal& _robot;

	// Ascenseur _ascenseur;
	// On sait quelles zones sont utilisées pour le stockage
	// bool _cylindresStockes[PositionStockage::NBR] = {false, false, false};
	// ResultatAction ajouterCylindre(int i);
	// ResultatAction supprimerCylindre(int i);

	Commun::ServoManager _servos;
	Commun::MoteurManager _moteurs;

	bool _turbine_left_free = true;
	bool _turbine_right_free = true;
	std::atomic_int _elevator_counter;
};

#endif
