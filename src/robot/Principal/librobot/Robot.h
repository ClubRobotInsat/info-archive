/* Robot2015.h
 */

#ifndef ROBOT_2015_H
#define ROBOT_2015_H

#include "../../Commun/Robot.h"
#include "Strategie.h"

// Identifiants des cartes
#include "../../../commun/Commun.h"
#include "../../../commun/RobotPrincipal/Constantes.h"

#include "../../Commun/Deplacement.h"
#include <atomic>
#include <iostream>
#include <mutex>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RobotPrincipal
    : public Commun::RobotAnnee<StrategiePrincipal, IDCartesPrincipal::IDCartes, ConstantesPrincipal::CarteInfo, ConstantesPrincipal::Contacteur> {
public:
	//////////////////
	/// Constantes ///
	//////////////////
	static const AngularSpeed VITESSE_ASCENSEUR_DEFAUT;
	static const AngularSpeed VITESSE_RECALAGE_ASCENSEUR;

	static const Distance RAYON_ROBOT_RECALAGE;
	static const Distance RAYON_ROBOT_RECALAGE_X;

	static const float COEFF_FIT_MULT;
	static const float COEFF_FIT_POW;

public:
	///////////////////
	// Constructeurs //
	///////////////////
	RobotPrincipal(std::vector<std::string> const& args,
	               Speed vitesseDeplacement = ConstantesPrincipal::VITESSE_LINEAIRE_DEFAUT,
	               AngularSpeed vitesseAngulaire = ConstantesPrincipal::VITESSE_ANGULAIRE_DEFAUT);

	~RobotPrincipal() = default;

	Vector2m getPositionTourelle() const override;

	// L'appel est effectué par la classe Strategie lors de sa construction et de sa destruction
	virtual void desactivation() override;

	StrategiePrincipal& getStrategie();

private:
	virtual void initialisation() override;
	virtual void assignerCartes() override;
};

#endif
