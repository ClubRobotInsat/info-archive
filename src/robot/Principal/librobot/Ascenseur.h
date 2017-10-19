//
//  Ascenseur.h
//  Club Robot
//
//  Created by Rémi on 05/03/.
//

#ifndef __Club_Robot__Ascenseur__2015
#define __Club_Robot__Ascenseur__2015

#include "../../../commun/Commun.h"
#include "../../../commun/RobotPrincipal/Constantes.h"
#include "../../Commun/Ascenseur.h"

class Ascenseur : public Commun::Ascenseur {
public:
	/**
	 * Correspond à l'offset de l'ascenseur = ce qui est rajouté de base à tous ses déplacements
	 * Dans l'idée si on utilise cet offset c'est qu'on veut déposer sur la zone rouge au milieu de la table
	 */
	static const Angle OFFSET;
	/**
	 * Correspond à la hauteur d'un élément porté par les ascenseurs en roulant
	 */
	static const Distance HAUTEUR_AVANCE;

public:
	Ascenseur(CarteAsservissement& carteAsservissement,
	          Angle const (&positionsAscenseur)[enumToInt(ConstantesPrincipal::PositionAscenseur::NBR)]);

	/*	bool estEnPositionStockage() {
	        return _positionAscenseur == ConstantesPrincipal::PositionAscenseur::ETAGE1;
	    }

	    bool estAuSol() {
	        return _positionAscenseur == ConstantesPrincipal::PositionAscenseur::MAX_BAS;
	    }

	    bool estEnPositionDeplacement() {
	        return _positionAscenseur == ConstantesPrincipal::PositionAscenseur::ROULER;
	    }

	    bool estMaxHaut() {
	        return _positionAscenseur == ConstantesPrincipal::PositionAscenseur::MAX_HAUT;
	    }
	*/
	// ResultatAction initialiser();

	/**
	 * Utilitaire pour aller à une position
	 */
	ResultatAction allerAPosition(ConstantesPrincipal::PositionAscenseur position);

private:
	ConstantesPrincipal::PositionAscenseur _positionAscenseur;
};

#endif /* defined(__Club_Robot__Ascenseur__) */
