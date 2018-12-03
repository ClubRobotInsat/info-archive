#ifndef ROOT_SIMULED_H
#define ROOT_SIMULED_H

#include "../../robot/Modules/Module.hpp"

class SimuLed : public PhysicalRobot::Module<SharedLED2019> {
public:
	SimuLed(uint8_t id);

	uint8_t get_frame_size() const override;

	void deactivation() override;

protected:
	bool _on_off;


	SharedLED2019 generate_shared() const override;
	void message_processing(const SharedLED2019&) override;
};


#endif // ROOT_SIMULED_H
