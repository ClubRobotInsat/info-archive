#include "catch.hpp"

#include "../src/simulateur/communication/SimuNavigation.h"
#include "../src/robot/Modules/Navigation.h"

TEST_CASE("SimuNavigation") {

    SECTION("Communication test with Navigation module") {
        PhysicalRobot::Navigation iaModule(1);
        SimuNavigation simuModule(1);

        iaModule.forward(20_cm, PhysicalRobot::SensAdvance::Forward);
        auto frames = iaModule.make_frames();

        REQUIRE(!frames.empty());

        for (auto &frame : frames) {
            simuModule.update(frame);
        }

        CHECK(simuModule.get_current_command() == MovingCommand::GoForward);
    }
}
