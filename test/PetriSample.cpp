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
} // namespace

void init_petri_tests(std::shared_ptr<Strategy::Interfacer::RobotManager> manager) {
	_manager = manager;
}

ActionResult test_action(int a, int b) {
	tests()->set_a_value(a);
	tests()->set_b_value(b);
	return ActionResult::SUCCESS;
}

bool test_bool() {
	return true;
}
