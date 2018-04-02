//
// Created by terae on 08/02/18.
//

#include "Action.hpp"
#include "Element.hpp"
#include "Table.hpp"

//#include "../../petri/src/Bee.h"
//#include "../../petri/src/IA.h"
#include "MagicStrategy.h"

int main() {
	// Definition of all possible actions
	// auto petri_bee  = std::make_shared<::Petri::MemberPetriDynamicLib>(false, "IA", 12346, "../../petri/src/Bee.h");
	auto action_bee = [/*&petri_bee*/](Vector2m pos) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(5_s, 50, pos, -90_deg, {} /*, petri_bee*/);
	};

	// auto petri_cube  = std::make_shared<::Petri::MemberPetriDynamicLib>(false, "IA", 12346,
	// "../../petri/src/Cube.h");
	auto action_cube = [/*&petri_cube*/](Vector2m pos) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(20_s, 30, pos, 20_deg, {} /*, petri_cube*/);
	};

	// auto petri_sphere  = std::make_shared<::Petri::MemberPetriDynamicLib>(false, "IA", 12346,
	// "../../petri/src/Sphere.h");
	auto action_sphere = [/*&petri_sphere, */ &action_cube](Vector2m pos) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(
		    20_s, 80, pos, 50_deg, {StrategyGenerator::Element(StrategyGenerator::ElementType::CUBE, {1.5_m, 0_m})} /*, petri_sphere*/);
	};

	auto action_switch = [/*&petri_switch*/](Vector2m pos) -> StrategyGenerator::Action {
		return StrategyGenerator::Action(3_s, 50, pos, -90_deg, {});
	};

	std::map<StrategyGenerator::ElementType, std::function<StrategyGenerator::Action(Vector2m)>> element_to_action;
	element_to_action[StrategyGenerator::ElementType::BEE] = action_bee;
	element_to_action[StrategyGenerator::ElementType::CUBE] = action_cube;
	element_to_action[StrategyGenerator::ElementType::SPHERE] = action_sphere;
	element_to_action[StrategyGenerator::ElementType::SWITCH] = action_switch;

	// TODO: determine if an action is possible or not in function of the robot state
	std::function<bool()> always_possible = []() -> bool { return true; };
	std::map<StrategyGenerator::ElementType, std::function<bool()>> element_actionable;
	element_actionable[StrategyGenerator::ElementType::BEE] = always_possible;
	element_actionable[StrategyGenerator::ElementType::CUBE] = always_possible;
	element_actionable[StrategyGenerator::ElementType::SPHERE] = always_possible;
	element_actionable[StrategyGenerator::ElementType::SWITCH] = always_possible;

	// Initial state
	// TODO: choose initial robot position in function of its color
	StrategyGenerator::Table table({20_cm, 1.6_m});

	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::BEE, {1_m, 1.5_m})));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::SPHERE, {10_cm, 50_cm})));
	table.emplace(std::make_shared<StrategyGenerator::Element>(
	    StrategyGenerator::Element(StrategyGenerator::ElementType::SPHERE, {50_cm, 2_m})));

	StrategyGenerator::MagicStrategy best_ia_ever(element_to_action, element_actionable);
	best_ia_ever.run(table, 500_ms);
}