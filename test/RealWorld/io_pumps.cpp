//
// Created by terae on 24/03/19.
//

#include "../../src/robot/Robot.h"
#include "../../src/robot/Strategy/AbstractStrategy.h"
#include "../../src/robot/Strategy/ModuleInterfacers/RobotManager.h"

class StrategyIO final : public Strategy::AbstractStrategy {
public:
	StrategyIO() : AbstractStrategy(Constants::RobotColor::Yellow) {
		this->add_robot(std::make_shared<PhysicalRobot::Robot>("primary"));
	}

private:
	void execute() override {
		auto io = _interfacers["io"]->get_interfacer<Strategy::Interfacer::ServosInterfacer>();
		auto pumps = _interfacers["pumps"]->get_interfacer<Strategy::Interfacer::PumpsInterfacer>();

		wait_for_tirette();

		using Strategy::Interfacer::PumpsInterfacer;

		pumps.catch_forward(PumpsInterfacer::StockingRail::Blueium, PumpsInterfacer::StockingRail::Redium);
		pumps.catch_forward(PumpsInterfacer::StockingRail::Goldenium, PumpsInterfacer::StockingRail::Nothing);
	}
};

using namespace PhysicalRobot;

#define TEST_STRAT 0
#define TEST_ROBOT 1

int main() {
#if TEST_STRAT
	StrategyIO strat;
	strat.start(10_s);
#endif

#if TEST_ROBOT
	auto m = std::make_shared<ModuleManager>();

	// TODO check ID
	auto& io = m->add_module<IO>(4);
	auto& pumps = m->add_module<Pumps>(5);

	// TODO check address
	Robot robot(m, {"prog_io_pumps", "UDP", "192.168.1.4", "5000", "51"}, Lidar::None);
	robot.set_debug(true);

	std::cout << "insert tirette" << std::endl;
	while(io.read_tirette() != TriggerState::Triggered) {
		sleep(50_ms);
	}
	std::cout << "inserted" << std::endl;
	pumps.activate_pump(0);
	pumps.activate_pump(1);
	io.set_sound(BuzzerState::PlaySuccessSound);
	sleep(2_s);

	std::cout << "remove it" << std::endl;
	while(io.read_tirette() != TriggerState::Waiting) {
		sleep(50_ms);
	}
	std::cout << "ok" << std::endl;
	pumps.deactivate_pump(0);
	pumps.deactivate_pump(1);
	io.set_sound(BuzzerState::PlayErrorSound);
#endif
}
