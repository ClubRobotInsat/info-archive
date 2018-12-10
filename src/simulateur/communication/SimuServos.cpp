#include "SimuServos.h"

SimuServos::SimuServos(uint8_t id) : Module(id) {}

uint8_t SimuServos::get_nbr_servos() const {
	return 5; // TODO get_nbr_servos
}

void SimuServos::deactivation() {}

std::vector<JSON> SimuServos::generate_list_jsons() const {
	JSON j;
	// TODO generate simuservos json

	return {j};
}

void SimuServos::message_processing(const JSON&) {
	std::cout << "Trame recue" << std::endl;
}