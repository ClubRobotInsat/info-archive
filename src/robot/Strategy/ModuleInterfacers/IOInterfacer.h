//
// Created by terae on 13/02/19.
//

#ifndef ROOT_IOINTERFACER_H
#define ROOT_IOINTERFACER_H

#include "Interfacer.hpp"

namespace Strategy::Interfacer {

	class IOInterfacer final : public AbstractInterfacer {
	public:
		using interfaced_type = PhysicalRobot::IO;
		using TriggerState = PhysicalRobot::TriggerState;
		using BuzzerState = PhysicalRobot::BuzzerState;

		explicit IOInterfacer(interfaced_type& module_io);
		explicit IOInterfacer(std::shared_ptr<PhysicalRobot::Robot>);
		explicit IOInterfacer(PhysicalRobot::Robot&);

		TriggerState get_tirette_state() const;

		bool is_tirette_triggered() const;

		void wait_insertion_tirette() const;

		void wait_deletion_tirette() const;

		void reset_buzzer();

		void play_success_sound();

		void play_error_sound();

	private:
		interfaced_type& _module;
	};

} // namespace Strategy::Interfacer

#endif // ROOT_IOINTERFACER_H
