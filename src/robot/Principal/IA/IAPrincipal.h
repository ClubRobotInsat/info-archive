#pragma once

//#define HOMOLOGATION

#include "../../../commun/ConstantesCommunes.h"
#include "../librobot/libRobot.h"
#include <Commun.h>


#ifdef HOMOLOGATION
#include "../petri/src/Homologation.h"
#else
#include "../petri/src/IA2017.h"
#endif

class IAPrincipal : public StrategiePrincipal {
public:
	IAPrincipal(std::vector<std::string> const& args, Constantes::RobotColor color, bool debugMode);

protected:
	void initialisation();

	/// Contient la logique de la stratégie elle-même
	virtual void executer() override;
	virtual void funnyAction() override;
	/**
	 * Lance le pétri qui contient l'IA en release ou debug
	 */
	ResultatAction lancerPetri(bool debug);
	std::unique_ptr<Petri::PetriDynamicLib> _petri;
	bool _debugMode;
};