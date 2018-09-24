#pragma once

//#define HOMOLOGATION
#define USE_MAGIC_STRATEGY

#include "../../../Principal/librobot/libRobot.h"
#include "ConstantesCommunes.h"
#include <Commun.h>

#include "../../Principal/librobot/StrategyGenerator/MagicStrategy.h"

#ifdef HOMOLOGATION
#include "../petri/src/Homologation.h"
#else
#include "../../Principal/petri/src/IA2018.h"
#endif

class IAPrincipal : public StrategiePrincipal {
public:
	IAPrincipal(std::vector<std::string> const& args, Constantes::RobotColor color, bool debugMode, bool magicStrategy);

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
	bool _magicStrategy;

	StrategyGenerator::MagicStrategy _strategy;

	std::thread _points_printer;
};