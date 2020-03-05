//
// Created by terae on 29/05/19.
//

#ifndef ROOT_CAPTORS_H
#define ROOT_CAPTORS_H

#include "IO.h"
#include "Module.hpp"

namespace PhysicalRobot {

	ENUM_CLASS_NS(PhysicalRobot, CaptorColor, Red, Blue, Green);

	class Captors final : public Module {
	public:
		using switch_t = uint8_t;

		static const switch_t NBR_MAX_SWITCHES = 7;

		explicit Captors(uint8_t id);

		CaptorColor read_color() const;
		TriggerState read_switch(switch_t id) const;

	private:
		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override;

		std::atomic<CaptorColor> _color;
		std::array<std::atomic<TriggerState>, NBR_MAX_SWITCHES> _switches_on_off;
	};

} // namespace PhysicalRobot

#endif // ROOT_CAPTORS_H
