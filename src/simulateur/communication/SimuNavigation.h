
#ifndef ROOT_SIMUNAVIGATION_H
#define ROOT_SIMUNAVIGATION_H

#include "../../robot/Modules/Module.hpp"
#include "../../robot/Modules/Navigation.h"
#include "../physique/IRobotController.h"

using PhysicalRobot::MovingCommand;

class SimuNavigation : public PhysicalRobot::Module {
public:
	explicit SimuNavigation(uint8_t id);

	void deactivation() override;

	MovingCommand get_current_command();

protected:
	// writeable
	/// x, y, angle
	repere::Coordinates _coords;

	std::atomic_bool _blocked;
	std::atomic_bool _moving_done;

	// read-only
	std::atomic_bool _leds;
	std::atomic_bool _asserv_on_off;
	std::atomic_bool _reset;

	MovingCommand _command;
	uint16_t _args_cmd[2];
	uint16_t _counter;

	std::shared_ptr<IRobotController> _robotController;

	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};

#endif // ROOT_SIMUNAVIGATION_H
