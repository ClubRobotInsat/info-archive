#ifndef ROOT_SIMUMODULE_H
#define ROOT_SIMUMODULE_H

#include "../../robot/Modules/Module.hpp"

class SimuModule : public PhysicalRobot::Module {
public:
	SimuModule(uint8_t id, const std::string& module_name) : Module(id, module_name) {}

	virtual JSON getModuleState() = 0;
};

#endif // ROOT_SIMUMODULE_H
