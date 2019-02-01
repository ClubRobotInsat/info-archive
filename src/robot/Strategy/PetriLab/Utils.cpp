//
// Created by terae on 01/02/19.
//

#include "Utils.h"
#include "../AbstractStrategy.h"

namespace {
	Strategy::AbstractStrategy* _strategy;
} // namespace

void inits_petri_util(Strategy::AbstractStrategy& strategy) {
	_strategy = &strategy;
}

bool color_purple() {
	return _strategy->get_color() == Constants::RobotColor::Purple;
}

bool color_yellow() {
	return _strategy->get_color() == Constants::RobotColor::Yellow;
}

bool color_undef() {
	return _strategy->get_color() == Constants::RobotColor::Undef;
}

ActionResult set_points(int points) {
	_strategy->set_points(points);
	return ActionResult::SUCCESS;
}

ActionResult add_points(int points) {
	_strategy->add_points(points);
	return ActionResult::SUCCESS;
}

int get_points() {
	return _strategy->get_points();
}
