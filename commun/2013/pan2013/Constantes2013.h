// Constantes2013.h

#ifndef CONSTANTES_2013_H
#define CONSTANTES_2013_H

#include <iostream>


namespace ConstantesPan2013
{
    
    /***********************************/
	/********** CONTACTEURS ************/
    /***********************************/
    
	enum NumContacteurs	{
		CONTACTEUR_TIRETTE 			= 1,
		CONTACTEUR_COULEUR          = 0,
	};

    
    /***********************************/
    /************ SERVOS ***************/
    /***********************************/
    
    //ID des servos
	enum Servos {
		SERVO_BRAS_DROIT            = 1,
		SERVO_BRAS_GAUCHE           = 2,
		SERVO_RAMASSE               = 0,
		SERVO_EJECTE                = 3,
        SERVO_NBR                   = 4
	};

    //Index des positions dans le tableau
	enum PositionServosBras {
        SERVO_OUVRE_CADEAU = 0,
        SERVO_MILIEU_BRAS,
        SERVO_SOUFFLE_BOUGIE,
        SERVO_RANGE,
        SERVO_BRAS_NBR_POSITIONS
    };

    //Index des positions dans le tableau
    enum PositionServosRamasse {
        SERVO_BAS = 0,
        SERVO_MILIEU,
        SERVO_RELEVE,
        SERVO_RAMASSE_NBR_POSITIONS
    };
    
    //Position des servos
    static const int positionsServosBrasGauche[SERVO_BRAS_NBR_POSITIONS] =   {1600,1420,1200, 600};
    static const int positionsServosBrasDroit[SERVO_BRAS_NBR_POSITIONS] =  {1050,1255,1455, 2100};
    static const int positionServosRamasse[SERVO_RAMASSE_NBR_POSITIONS] =   {600,2000, 1000};

    
    /***********************************/
	/************ MOTEURS **************/
    /***********************************/
    
    enum VitesseRotation{
        ARRET = 0,
        CASE_1,
        CASE_2,
        CASE_3,
        CASE_4,
        CASE_5,
        LANCEUR_NBR_VITESSES
    };
    
    static const bool SENS_EJECTION = 0;
    
    //Vitesse de rotation du moteur du lanceur
  	static const int vitessesRotationLanceur[LANCEUR_NBR_VITESSES] = {0, 1000, 2000, 3000, 4000, 5000};
    
}

#endif //CONSTANTES_2013_H

