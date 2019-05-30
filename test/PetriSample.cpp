//
// Created by terae on 01/02/19.
//

#include "PetriSample.h"
#include "TestUtils.h"

namespace {
	std::shared_ptr<Strategy::Interfacer::RobotManager> _manager;

	TestUtils::InterfacerTest& tests() {
		return _manager->get_interfacer<TestUtils::InterfacerTest>();
	}

	std::atomic_bool action_was_called;
} // namespace

void init_petri_tests(std::shared_ptr<Strategy::Interfacer::RobotManager> manager) {
	_manager = manager;
	action_was_called = false;
}

Outcome test_action(int a, int b) {
	tests()->set_a_value(a);
	tests()->set_b_value(b);
	action_was_called = true;
	return Outcome::SUCCESS;
}

bool test_bool() {
	return action_was_called;
}
