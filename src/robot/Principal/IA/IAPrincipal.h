#pragma once

//#define HOMOLOGATION
#define USE_MAGIC_STRATEGY

#include "../../../commun/ConstantesCommunes.h"
#include "../librobot/libRobot.h"
#include <Commun.h>

#include "../librobot/StrategyGenerator/MagicStrategy.h"

#ifdef HOMOLOGATION
#include "../petri/src/Homologation.h"
#else
#include "../petri/src/IA2018.h"
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

	StrategyGenerator::MagicStrategy _strategy;
	int _nbr_points = 450;
};