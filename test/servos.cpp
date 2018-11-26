//
// Created by terae on 14/10/18.
//

#include "../src/robot/Robot.h"
#include "json.hpp"

using namespace PhysicalRobot;

int main() {
	const uint8_t ID_SERVO = 254;
	auto m = std::make_shared<ModuleManager>();

	auto& servos = m->add_module<Servos>(4);
	servos.add_servo(ID_SERVO);
	servos.set_color(ID_SERVO, Servos::CYAN);

	// Communication::protocol_udp udp("192.168.0.222", 54000, 51);

	Robot robot(m, {"prog_servos", "UDP", "192.168.0.222", "50000", "51"});
	servos.set_speed(ID_SERVO, 1000);
	std::cout << "set_speed(1000)" << std::endl;

	/**for(int i = 0; i < 1024; ++i) {
	    JSON json;
	    json["id"] = 0xFE;
	    json["known_position"] = 0;
	    json["control"] = "Speed";
	    json["data"] = i;
	    //json["servo"]["command_type"] = 1;
	    json["blocked"] = false;
	    json["mode"] = "HoldOnblock";
	    json["color"] = "Blue";

	    //std::cout << std::setw(4) << json << std::endl << std::endl;
	    //auto v = JSON::to_msgpack(json);

	    std::stringstream ss;
	    ss << json;
	    std::string str = ss.str();
	    //std::cout << "\n\n" << str << std::endl;

	    std::vector<uint8_t> v(str.begin(), str.end());
	    GlobalFrame f(v.size(), &v[0]);

	    //std::cout << std::endl;

	    Communication::protocol_udp udp("192.168.0.222", 5000, 51);

	    udp.send_frame(f);
	    //sleep(0_ms);
	}

	sleep(5_s);

	for(int i = 1023; i >= 0; --i) {
	    JSON json;
	    json["id"] = 0xFE;
	    json["known_position"] = 0;
	    json["control"] = "Speed";
	    json["data"] = i;
	    //json["servo"]["command_type"] = 1;
	    json["blocked"] = false;
	    json["mode"] = "HoldOnblock";
	    json["color"] = "Blue";

	    //std::cout << std::setw(4) << json << std::endl << std::endl;
	    //auto v = JSON::to_msgpack(json);

	    std::stringstream ss;
	    ss << json;
	    std::string str = ss.str();
	    //std::cout << "\n\n" << str << std::endl;

	    std::vector<uint8_t> v(str.begin(), str.end());
	    GlobalFrame f(v.size(), &v[0]);

	    //std::cout << std::endl;

	    Communication::protocol_udp udp("192.168.0.222", 5000, 51);

	    udp.send_frame(f);
	    //sleep(0_ms);
	}*/

	/*sleep(5_s);

	for(int count = 0; count < 10; count++) {
	    const Angle command = -150_deg + count * 30_deg;
	    std::cout << "set_position_servo_254(" << command << ")." << std::endl;
	    servos.set_position(ID_SERVO, command);
	    sleep(1_s);
	}
	servos.set_position(ID_SERVO, 0_deg);
	sleep(1_s);

	servos.set_speed(ID_SERVO, 30);
	sleep(5_s);

	// Envoi de plein d'ordres différents
	for(int count = 0; count < 500; ++count) {
	    servos.set_position(ID_SERVO, -90_deg);
	    servos.set_speed(ID_SERVO, 5000);
	    servos.set_blocking_mode(ID_SERVO, Servos::BlockingMode::UNBLOCKING);
	    servos.set_position(ID_SERVO, 90_deg);
	    servos.set_position(ID_SERVO, 0_deg);
	    servos.set_speed(ID_SERVO, 2000);
	}
	*/
	sleep(5_s);
	logInfo("End of the sequence test.");
	// robot.deactivation();

	return EXIT_SUCCESS;
}
