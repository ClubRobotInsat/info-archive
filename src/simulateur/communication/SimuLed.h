#ifndef ROOT_SIMULED_H
#define ROOT_SIMULED_H

#include "../../robot/Modules/Module.hpp"

class SimuLed : public PhysicalRobot::Module {
public:
	explicit SimuLed(uint8_t id);

	void deactivation() override;

protected:
	bool _on_off;


	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};


#endif // ROOT_SIMULED_H
