
#ifndef ROOT_SIMUNAVIGATION_H
#define ROOT_SIMUNAVIGATION_H

#include "../../robot/Modules/Navigation.h"
#include "../physique/IRobotController.h"
#include "SimuModule.h"

using PhysicalRobot::MovingCommand;

class SimuNavigation : public SimuModule {
public:
	explicit SimuNavigation(uint8_t id, const std::shared_ptr<IRobotController>& robotController);

	void deactivation() override;

	MovingCommand get_current_command();

	JSON getModuleState() override;

protected:
	// read-only
	std::atomic_bool _leds;
	std::atomic_bool _asserv_lin;
	std::atomic_bool _asserv_ang;
	std::atomic_bool _reset;

	MovingCommand _command;
	uint16_t _args_cmd[2];
	uint16_t _counter = 0;

	std::shared_ptr<IRobotController> _robotController;

	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};

#endif // ROOT_SIMUNAVIGATION_H
