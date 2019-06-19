#include "SimuServos.h"

// TODO Déplacer les élements utilisés par le robot et le simulateur dans "commun/"
#include "../../robot/Modules/Servos.h"

SimuServos::SimuServos(uint8_t id) : SimuModule(id, "SimuServos") {}

uint8_t SimuServos::get_nbr_servos() const {
	return 5; // TODO get_nbr_servos
}

JSON SimuServos::getModuleState() {
	return {};
}

void SimuServos::deactivation() {}

std::vector<JSON> SimuServos::generate_list_jsons() const {
	JSON j;
	j["id"] = 1;
	j["known_position"] = 450;
	j["blocked"] = false;
	j["color"] = toString(PhysicalRobot::Color::Red);
	// TODO generate simuservos json

	return {j};
}

void SimuServos::message_processing(const JSON&) {
	_state_changed.exchange(true);
}