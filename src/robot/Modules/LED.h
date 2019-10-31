/**
 * Cette classe permets d'accéder aux états de contacteurs sur le robot
 * Actuellement, seule la tirette est présente
 * L'IO permets donc de savoir quand commence le match
 */

#ifndef ROOT_MODULELED_H
#define ROOT_MODULELED_H

#include "IO.h"
#include "Module.hpp"
#include <thread>

namespace PhysicalRobot {

	ENUM_CLASS_NS(PhysicalRobot, LEDMode, Manual, Auto);

	class LED final : public Module {
	public:
		explicit LED(uint8_t, uint8_t);
		explicit LED(uint8_t);
		~LED();
		IOState get_state();
		void on();
		void off();
		/*inline*/ void trigger();
		void start();
		void stop();
		LEDMode get_mode();
		bool get_auto_onoff();
		float get_freq();

	private:
		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;


		void deactivation() override {}
		// std::mutex _mutex_auto;
		std::unique_ptr<std::thread> _pth;
		float _freq_max; // Hz
		IOState _state;
		LEDMode _mode;
		bool _auto_onoff; // Tells in mode auto if the led is trigging
		std::mutex _m_state;
	};

} // namespace PhysicalRobot

#endif // ROOT_MODULELED_H
