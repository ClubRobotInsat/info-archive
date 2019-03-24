#include "catch.hpp"

#include "../src/robot/Modules/Navigation.h"
#include "../src/simulateur/communication/SimuNavigation.h"

class DummyRobotController : public IRobotController {

	void forward(Distance) override {}

	void turn(Angle) override {}

	void stop() override {}

	repere::Coordinates getCoordinates() override {
		return repere::Coordinates();
	}

	SimuRobotState getState() override {
		return SimuRobotState::Idle;
	}

	void update(Duration) override {}
};

TEST_CASE("SimuNavigation") {

	SECTION("Communication test with Navigation module") {
		PhysicalRobot::Navigation iaModule(1);
		auto robotController = std::make_shared<DummyRobotController>();
		SimuNavigation simuModule(1, robotController);

		iaModule.forward(20_cm, PhysicalRobot::SensAdvance::Forward);
		auto frames = iaModule.make_frames();

		REQUIRE(!frames.empty());

		for(auto& frame : frames) {
			simuModule.update(frame);
		}

		CHECK(simuModule.get_current_command() == MovingCommand::GoForward);
	}
}
