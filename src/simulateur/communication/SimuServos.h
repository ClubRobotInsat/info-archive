#ifndef ROOT_SIMUSERVOS_H
#define ROOT_SIMUSERVOS_H

#include "../../robot/Modules/Module.hpp"

class SimuServos : public PhysicalRobot::Module<SharedServos2019> {
public:
	SimuServos(uint8_t id);

	uint8_t get_frame_size() const override;
	uint8_t get_nbr_servos() const;

	void deactivation() override;

protected:
	SharedServos2019 generate_shared() const override;
	void message_processing(const SharedServos2019&) override;
};

#endif // ROOT_SIMUSERVOS_H
