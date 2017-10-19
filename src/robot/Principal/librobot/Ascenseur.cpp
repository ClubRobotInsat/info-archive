//
//  Ascenseur2015.cpp
//  Club Robot
//
//  Created by Rémi on 05/03/2015.
//

#include "Ascenseur.h"

using namespace Constantes;
using namespace ConstantesPrincipal;
using namespace IDCartesPrincipal;

Ascenseur::Ascenseur(CarteAsservissement& carteAsservissement, Angle const (&positionsAscenseur)[enumToInt(PositionAscenseur::NBR)])
        : Commun::Ascenseur(carteAsservissement, {positionsAscenseur, positionsAscenseur + enumToInt(PositionAscenseur::NBR)})
        , _positionAscenseur(PositionAscenseur::SAISIE_CYLINDRE_SOL) {}

ResultatAction Ascenseur::allerAPosition(PositionAscenseur position) {
	std::cout << "Consigne ascenseur : " << enumToInt(position) << std::endl;
	ResultatAction res = positionnerAscBloquant(enumToInt(position));
	if(res == ResultatAction::REUSSI) {
		_positionAscenseur = position;
	}
	sleep(2_s);
	return res;
}

/*
ResultatAction Ascenseur::initialiser() {
    return this->Commun::Ascenseur::initialiser(SensRotation::Horaire);
}
*/