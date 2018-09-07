//
// Created by terae on 4/6/18.
//

#include "../src/robot/Principal/librobot/StrategyGenerator/MagicStrategy.h"
#include "catch.hpp"

using namespace StrategyGenerator;

Table generate_table() {
	Table table;

	table.emplace(std::make_shared<Element>(ElementType::BEE, Coordonnees({50_cm, 1.5_m})));
	table.emplace(std::make_shared<Element>(ElementType::GET_SPHERE, Coordonnees({0_m, 0_m})));
	table.emplace(std::make_shared<Element>(ElementType::GET_SPHERE, Coordonnees({1_m, 2_m})));

	table.set_robot_coords(Coordonnees({0_m, 0_m}));

	return table;
}

/*MagicStrategy generate_strategy(Table table) {
    auto action_bee = [](Coordonnees coords) -> Action { return Action(5_s, 50, Element(ElementType::BEE, coords), {});
};

    auto action_cube = [](Coordonnees coords) -> Action {
        return Action(20_s, 30, Element(ElementType::GET_CUBE, coords), {});
    };

    auto action_sphere = [&action_cube](Coordonnees coords) -> Action {
        return Action(20_s, 80, Element(ElementType::GET_SPHERE, coords), {Element(ElementType::GET_CUBE,
Coordonnees({2_m, 1_m}, 0_deg))});
    };

    auto action_switch = [](Coordonnees coords) -> Action {
        return Action(3_s, 50, Element(ElementType::SWITCH, coords), {});
    };

    MagicStrategy strategy;

    std::function<bool()> always_possible = []() -> bool { return true; };
    std::function<bool()> always_impossible = []() -> bool { return false; };

    strategy.associate_element(ElementType::BEE, action_bee, always_possible);
    strategy.associate_element(ElementType::GET_CUBE, action_cube, always_impossible);
    strategy.associate_element(ElementType::GET_SPHERE, action_sphere, always_possible);
    strategy.associate_element(ElementType::SWITCH, action_switch, always_possible);

    strategy.initialize(std::move(table));

    return strategy;
}*/

TEST_CASE("StrategyGenerator") {
	SECTION("DecisionalTree") {
		DecisionalTree tree(generate_table());
		CHECK(tree.size() == 0);

		tree.add_edge(tree.get_root(), generate_table(), Action(0_s, 0, Element(ElementType::NOTHING, Coordonnees({0_m, 1_m})), {}));

		CHECK(tree.get_root()->out_edges.back().target->cost.second == 1_m / LINEAR_SPEED_DEFAULT);

		DecisionalTree::node* n;
		Table next_table{tree.get_table((n = tree.pop_frontier_node()))};

		auto e = *next_table.cbegin();
		next_table.erase(e);

		// CHECK(e->get_type() == ElementType::GET_SPHERE);

		Action associated_action(20_s,
		                         80,
		                         *e,
		                         {Element(ElementType::GET_CUBE, Coordonnees({1.5_m, 1_m})),
		                          Element(ElementType::GET_CUBE, Coordonnees({2_m, 1_m}))});
		for(Element new_e : associated_action.get_next_elements()) {
			next_table.emplace(std::make_shared<Element>(new_e));
		}
		tree.add_edge(n, next_table, associated_action);

		CHECK(tree.size() == 1);

		n = n->out_edges.back().target;
		CHECK(tree.calculate_cost(n).first == 80);
		CHECK(tree.calculate_cost(n).second - (1_m / LINEAR_SPEED_DEFAULT + 20_s + 2_m / LINEAR_SPEED_DEFAULT) < 1_s);
	}

	SECTION("Action") {
		SECTION("get_execution_time") {
			auto generator = [](Duration execution_time, repere::Coordonnees start_coords) -> Action {
				return Action(execution_time, 0, Element(ElementType::NOTHING, start_coords), {});
			};

			CHECK(generator(20_s, repere::Coordonnees({0_m, 0_m})).get_execution_time(repere::Coordonnees({1.2_m, 0_m})) == 23_s);
		}
	}

	/*SECTION("MagicStrategy") {
	    MagicStrategy strategy = generate_strategy(generate_table());

	    strategy.run(500_ms);

	    CHECK(strategy.get_total_points() == 0);

	}*/

	SECTION("Table") {}
}
