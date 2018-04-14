//
// Created by terae on 08/02/18.
//

//#include "../../petri/src/Bee.h"
//#include "../../petri/src/IA.h"
#include "MagicStrategy.h"

using namespace StrategyGenerator;
using namespace repere;

int main() {
	// Definition of all possible actions
	// auto petri_bee  = std::make_shared<::Petri::MemberPetriDynamicLib>(false, "IA", 12346, "../../petri/src/Bee.h");
	auto action_bee = [](Coordonnees coords) -> Action {
		return Action(5_s, 50, Element(ElementType::BEE, coords), {}, "bee");
	};

	// auto petri_cube  = std::make_shared<::Petri::MemberPetriDynamicLib>(false, "IA", 12346,
	// "../../petri/src/Cube.h");
	auto action_cube = [](Coordonnees coords) -> Action {
		return Action(20_s, 30, Element(ElementType::CUBE, coords), {}, "cube");
	};

	// auto petri_sphere  = std::make_shared<::Petri::MemberPetriDynamicLib>(false, "IA", 12346,
	// "../../petri/src/Sphere.h");
	auto action_sphere = [&action_cube](Coordonnees coords) -> Action {
		return Action(20_s, 80, Element(ElementType::SPHERE, coords), {Element(ElementType::CUBE, Coordonnees({1.5_m, 0_m}, 0_deg))}, "sphere");
	};

	auto action_switch = [](Coordonnees coords) -> Action {
		return Action(3_s, 50, Element(ElementType::SWITCH, coords), {}, "switch");
	};

	// Initial state
	// TODO: choose initial robot position in function of its color
	Table table;

	table.emplace(std::make_shared<Element>(ElementType::BEE, Coordonnees({1_m, 1.5_m})));
	table.emplace(std::make_shared<Element>(ElementType::SPHERE, Coordonnees({10_cm, 50_cm})));
	table.emplace(std::make_shared<Element>(ElementType::SPHERE, Coordonnees({50_cm, 2_m})));

	DecisionalTree tree(table);
	tree.add_edge(tree.get_root(), table, ActionWait(0_s));


	MagicStrategy best_ia_ever;

	/// TODO: only one petri to run; into an Action: give it the good variable
	// TODO: determine if an action is possible or not in function of the robot state
	std::function<bool()> always_possible = []() -> bool { return true; };
	best_ia_ever.associate_element(ElementType::BEE, action_bee, always_possible);
	best_ia_ever.associate_element(ElementType::CUBE, action_cube, always_possible);
	best_ia_ever.associate_element(ElementType::SPHERE, action_sphere, always_possible);
	best_ia_ever.associate_element(ElementType::SWITCH, action_switch, always_possible);

	best_ia_ever.initialize(table);
	// best_ia_ever.generate_tree(tree, 500_ms);
	std::cout << tree << std::endl;
	return 5;
	best_ia_ever.run(500_ms);
}