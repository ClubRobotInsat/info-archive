//
// Created by terae on 23/10/18.
//

#ifndef ROOT_LED_H
#define ROOT_LED_H

#include "Module.hpp"

namespace PhysicalRobot {

	class LED2019 final : public Module<SharedLED2019> {
	public:
		explicit LED2019(uint8_t id);

		void set_state(bool on_off);

		uint8_t get_frame_size() const override;

	private:
		SharedLED2019 generate_shared() const override;
		void message_processing(const SharedLED2019&) override;

		void deactivation() override {}

		bool _on_off;
	};
} // namespace PhysicalRobot

#endif // ROOT_LED_H
