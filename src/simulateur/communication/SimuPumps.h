#ifndef ROOT_SIMUPUMPS_H
#define ROOT_SIMUPUMPS_H

#include "SimuModule.h"

class SimuPumps : public SimuModule {
public:
	explicit SimuPumps(uint8_t id);

	void add_field(const std::string& key, const std::string& value);

	void deactivation() override;

	JSON getModuleState() override;

protected:
	JSON _state;
	JSON _frame;

	std::vector<JSON> generate_list_jsons() const override;
	void message_processing(const JSON&) override;
};


class SimuColor : public SimuModule {
public:
	explicit SimuColor(uint8_t id) : SimuModule(id, "SimuColor") {
		for(uint8_t id = 0; id < 7; ++id) {
			_frame["switches"][id] = "Off";
		}
		_frame["color"] = "Red";
	}

	void deactivation() override {}

	JSON getModuleState() override {
		return _frame;
	}

protected:
	JSON _state;
	JSON _frame;

	std::vector<JSON> generate_list_jsons() const override {
		return {_frame};
	}
	void message_processing(const JSON&) override {
		_state_changed.exchange(true);
	}
};


#endif // ROOT_SIMUPUMPS_H
