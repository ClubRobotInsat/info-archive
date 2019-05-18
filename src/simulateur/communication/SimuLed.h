#ifndef ROOT_SIMULED_H
#define ROOT_SIMULED_H

#include "SimuModule.h"

class SimuLed : public SimuModule {
public:
	explicit SimuLed(uint8_t id);

	void deactivation() override;

	JSON getModuleState() override;

protected:
	bool _on_off;


	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};


#endif // ROOT_SIMULED_H
