#ifndef ROOT_SIMUSERVOS_H
#define ROOT_SIMUSERVOS_H

#include "SimuModule.h"

class SimuServos : public SimuModule {
public:
	explicit SimuServos(uint8_t id);

	uint8_t get_nbr_servos() const;

	void deactivation() override;

	JSON getModuleState() override;

protected:
	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};

#endif // ROOT_SIMUSERVOS_H
