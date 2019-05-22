#ifndef ROOT_SIMUPUMPS_H
#define ROOT_SIMUPUMPS_H

#include "SimuModule.h"

class SimuPumps : public SimuModule {
public:
	explicit SimuPumps(uint8_t id);

	void deactivation() override;

	JSON getModuleState() override;

protected:
	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};


#endif // ROOT_SIMUPUMPS_H
