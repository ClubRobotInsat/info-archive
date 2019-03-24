//
// Created by terae on 14/10/18.
//

#include "../../src/robot/Robot.h"
#include "../../src/robot/Strategy/AbstractStrategy.h"
#include "json.hpp"

class StrategyServos final : public Strategy::AbstractStrategy {
	uint8_t _n;

public:
	StrategyServos(uint8_t servo) : AbstractStrategy(Constants::RobotColor::Yellow), _n(servo) {
		this->add_robot(std::make_shared<PhysicalRobot::Robot>("primary"));
	}

private:
	void execute() override {
		auto servos = _interfacers["primary"]->get_interfacer<Strategy::Interfacer::ServosInterfacer>();
		servos->add_servo(_n);
		// servos.set_position(_n, 80_deg);
		servos->set_speed(_n, PhysicalRobot::Servos::MAX_SPEED);
	}
};

using namespace PhysicalRobot;

#define TEST_STRAT 1
#define TEST_ROBOT 0
#define TEST_JSON 0

int main() {
	const uint8_t ALL_SERVOS = 254;

#if TEST_STRAT
	StrategyServos strat(ALL_SERVOS);
	strat.start(10_s);
#endif

#if TEST_ROBOT
	auto m = std::make_shared<ModuleManager>();

	auto& servos = m->add_module<Servos>(4);

	servos.add_servo(ALL_SERVOS);
	servos.set_blocking_mode(ALL_SERVOS, BlockingMode::HoldOnBlock);
	servos.set_color(ALL_SERVOS, Color::Blue);

	Robot robot(m, {"prog_servos", "UDP", "192.168.1.2", "50000", "51"}, Lidar::None);
	std::cout << "set_speed(1000)" << std::endl;
	servos.set_speed(ALL_SERVOS, 60_deg_s, Rotation::CounterClockwise);
	sleep(500_ms);
	servos.set_color(ALL_SERVOS, Color::Green);
	sleep(1_s);

	std::cout << "Plusieurs ordres à la suite" << std::endl;

	for(int count = 0; count < 10; count++) {
		const Angle command = -150_deg + count * 30_deg;
		servos.set_position(ALL_SERVOS, command);
		sleep(1_s);
	}
	servos.set_position(ALL_SERVOS, 0_deg);
	sleep(1_s);

	servos.set_speed(ALL_SERVOS, 30_deg_s);
	sleep(5_s);

	robot.deactivation();

#endif

#if TEST_JSON
	Communication::protocol_udp udp("192.168.1.2", 5000, 51);
	udp.debug_active = true;
	for(int i = 0; i < 1024; ++i) {
		JSON json;
		json["id"] = ALL_SERVOS;
		json["known_position"] = 0;
		json["control"] = "Speed";
		json["rotation"] = "CounterClockwise";
		json["data"] = i;
		json["blocked"] = false;
		json["mode"] = "HoldOnBlock";
		json["color"] = "Blue";

		std::stringstream ss;
		ss << 4 << json;
		std::string str = ss.str();
		// std::cout << "\n\n" << str << std::endl;

		std::vector<uint8_t> v(str.begin(), str.end());
		GlobalFrame f(v.size(), &v[0]);

		// std::cout << std::endl;

		udp.send_frame(f);
	}

	sleep(5_s);

	for(int i = 1023; i >= 0; --i) {
		JSON json;
		json["id"] = ALL_SERVOS;
		json["known_position"] = 0;
		json["control"] = "Speed";
		json["rotation"] = "Clockwise";
		json["data"] = i;
		json["blocked"] = false;
		json["mode"] = "HoldOnBlock";
		json["color"] = "Blue";

		std::stringstream ss;
		ss << 4 << json;
		std::string str = ss.str();
		// std::cout << "\n\n" << str << std::endl;

		std::vector<uint8_t> v(str.begin(), str.end());
		GlobalFrame f(v.size(), &v[0]);

		// std::cout << std::endl;

		Communication::protocol_udp udp("192.168.0.222", 5000, 51);

		udp.send_frame(f);
		sleep(10_ms);
	}

#endif

	logInfo("End of the sequence test.");

	return EXIT_SUCCESS;
}
