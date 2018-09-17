//
// Created by terae on 17/09/18.
//

#include "unit.cpp"

#include "../src/commun/RobotPrincipal/Constantes.h"
#include "../src/robot/Commun/Robot.h"

TEST_CASE("Initialization of a common robot.") {
	auto m = std::make_shared<Commun::ModuleManager>();

	auto& servos = m->add_module<Commun::Servos2019>(2);
	servos.add_servo(5, 120_deg);
	servos.add_servo(6, 50_deg, Commun::Servos2019::BlockingMode::HOLD_ON_BLOCKING);

	Commun::Robot robot(m, std::make_unique<ConstantesCommunes>(), std::make_unique<ConstantesRobotPrincipal>(), {"ehCoucou", "PIPES"});

	CHECK(robot.get_module<Commun::Servos2019>().get_nbr_servos() == 2);

	robot.get_communicator().disconnect();
}
