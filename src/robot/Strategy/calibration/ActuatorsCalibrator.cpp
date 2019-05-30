//
// Created by terae on 13/02/19.
//

#include "ActuatorsCalibrator.h"

#include "../PetriLab/Avoidance.h"
#include "../PetriLab/IO.h"
#include "../PetriLab/Navigation.h"
#include "../PetriLab/PumpsPrimary.h"
#include "../PetriLab/ServosPrimary.h"
#include "../PetriLab/Utils.h"

int main(int argc, char* argv[]) {
	ActuatorsCalibrator calibrator(Constants::RobotColor::Yellow);
	calibrator.start(1_h);

	return EXIT_SUCCESS;
}

ActuatorsCalibrator::ActuatorsCalibrator(Constants::RobotColor color) : AbstractStrategy(color) {
	debug_mode = true;
	std::shared_ptr<PhysicalRobot::Robot> physical_robot = std::make_shared<PhysicalRobot::Robot>("primary", true);

	add_robot(physical_robot);
}

void ActuatorsCalibrator::execute() {
	auto& module_servos = get_robot("primary")->get_robot()->get_module<PhysicalRobot::Servos>();
	auto& module_pumps = get_robot("primary")->get_robot()->get_module<PhysicalRobot::Pumps>();
	auto& interfacer_servos = get_robot("primary")->get_interfacer<Interfacer::ServosInterfacer>();
	auto& interfacer_pumps = get_robot("primary")->get_interfacer<Interfacer::PumpsInterfacerPrimary>();

	logInfo("=== Starting the calibration process ===");

	int choice;
	for(;;) {
		std::cout << "1 - Set position\n2 - Run sequence for FRONT servos\n3 - Run sequence for BACK servos\n4 - "
		             "pumps\n5 - exit"
		          << std::endl;
		std::cin >> choice;
		bool should_exit = false;

		switch(choice) {
			// Setting position of servos by specifying id and angle
			case 1: {
				logInfo("Set servos' position");
				int id, a;
				Angle angle;

				for(;;) {
					std::cout << "id? (-1 for exit) " << std::flush;
					std::cin >> id;
					if(id == -1) {
						break;
					}
					std::cout << "angle?            " << std::flush;
					std::cin >> a;
					angle = Angle::makeFromDeg(a);

					module_servos.set_position(id, angle);
					std::cout << "module_servos().set_position(" << id << ", " << angle.toDeg() << ")\n" << std::endl;
				}
				break;
			}

				// Running pre-set servos configurations for front servos
			case 2: {
				logInfo("Running pre-set configuration for front servos");
				Outcome res;

				for(;;) {
					std::cout << "1 - arm_bottom_horizontal\n2 - arm_bottom_vertical\n3 - arm_top_vertical\n4 - "
					             "arm_catch_goldenium\n5 - arm_release_goldenium\n6 - arm_external_storage\n7 - "
					             "arm_activate_experience\n8 - open_right_external_storage\n9 - "
					             "open_left_external_storage\n10 - close_right_external_storage\n11 - "
					             "close_left_external_storage\n12 - exit"
					          << std::endl;
					std::cin >> choice;

					switch(choice) {
						case 1:
							res = arm_bottom_horizontal(Arm::Front);
							break;
						case 2:
							res = arm_bottom_vertical(Arm::Front);
							break;
						case 3:
							res = arm_top_vertical(Arm::Front);
							break;
						case 4:
							res = arm_catch_goldenium(Arm::Front);
							break;
						case 5:
							res = arm_release_goldenium(Arm::Front);
							break;
						case 6:
							res = arm_external_storage(Arm::Front);
							break;
						case 7:
							res = arm_activate_experience(Arm::Front);
							break;
						case 8:
							res = open_external_storage(ExternalStorage::FrontRight);
							break;
						case 9:
							res = open_external_storage(ExternalStorage::FrontLeft);
							break;
						case 10:
							res = close_external_storage(ExternalStorage::FrontRight);
							break;
						case 11:
							res = close_external_storage(ExternalStorage::FrontLeft);
							break;
						default:
							should_exit = true;
							break;
					}
					if(should_exit) {
						should_exit = false;
						break;
					}

					std::cout << "Outcome: " << res << "\n" << std::endl;
				}
				break;
			}

				// Running pre-set servos configurations for back servos
			case 3: {
				logInfo("Running pre-set configuration for back servos");
				Outcome res;

				for(;;) {
					std::cout << "1 - arm_bottom_horizontal\n2 - arm_bottom_vertical\n3 - arm_top_vertical\n4 - "
					             "arm_catch_goldenium\n5 - arm_release_goldenium\n6 - arm_external_storage\n7 - "
					             "arm_activate_experience\n8 - open_right_external_storage\n9 - "
					             "open_left_external_storage\n10 - close_right_external_storage\n11 - "
					             "close_left_external_storage\n12 - exit"
					          << std::endl;
					std::cin >> choice;

					switch(choice) {
						case 1:
							res = arm_bottom_horizontal(Arm::Back);
							break;
						case 2:
							res = arm_bottom_vertical(Arm::Back);
							break;
						case 3:
							res = arm_top_vertical(Arm::Back);
							break;
						case 4:
							res = arm_catch_goldenium(Arm::Back);
							break;
						case 5:
							res = arm_release_goldenium(Arm::Back);
							break;
						case 6:
							res = arm_external_storage(Arm::Back);
							break;
						case 7:
							res = arm_activate_experience(Arm::Back);
							break;
						case 8:
							res = open_external_storage(ExternalStorage::BackRight);
							break;
						case 9:
							res = open_external_storage(ExternalStorage::BackLeft);
							break;
						case 10:
							res = close_external_storage(ExternalStorage::BackRight);
							break;
						case 11:
							res = close_external_storage(ExternalStorage::BackLeft);
							break;
						default:
							should_exit = true;
							break;
					}
					if(should_exit) {
						should_exit = false;
						break;
					}

					std::cout << "Outcome: " << res << "\n" << std::endl;
				}
				break;
			}

				// Pumps
			case 4: {
				logInfo("Pumps control");
				int valve_id;
				for(;;) {
					std::cout
					    << "1 - Activate pump\n2 - deactivate pump\n3 - activate valve\n4 - deactivate valve\n5 - exit"
					    << std::endl;
					std::cin >> choice;
					switch(choice) {
						case 1:
							module_pumps.activate_pump(0);
							break;
						case 2:
							module_pumps.deactivate_pump(0);
							break;
						case 3: {
							std::cout << "Valve id? " << std::flush;
							std::cin >> valve_id;
							module_pumps.activate_valve(valve_id);
							break;
						}
						case 4: {
							std::cout << "Valve id? " << std::flush;
							std::cin >> valve_id;
							module_pumps.deactivate_valve(valve_id);
							break;
						}
						default:
							should_exit = true;
							break;
					}
					if(should_exit) {
						should_exit = false;
						break;
					}
				}
				break;
			}

			default:
				should_exit = true;
				break;
		}

		if(should_exit) {
			break;
		}
	}
}