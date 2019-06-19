#ifndef ROOT_SIMUIO_H
#define ROOT_SIMUIO_H

#include "SimuModule.h"

#include "../../robot/Modules/IO.h"

class SimuIO : public SimuModule {
public:
	explicit SimuIO(uint8_t id);

	void setTriggerState(PhysicalRobot::TriggerState state);

	void deactivation() override;

	JSON getModuleState() override;

protected:
	PhysicalRobot::TriggerState _tirette;


	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};


#endif // ROOT_SIMUIO_H
