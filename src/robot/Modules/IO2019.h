/**
 * Cette classe permets d'accéder aux états de contacteurs sur le robot
 * Actuellement, seule la tirette est présente
 * L'IO permets donc de savoir quand commence le match
 */

#ifndef ROOT_MODULEIO2019_H
#define ROOT_MODULEIO2019_H

#include "SharedWithRust.h"

#include "Module.hpp"

namespace PhysicalRobot {

	class IO2019 final : public Module<SharedIO2019> {
	public:
		explicit IO2019(uint8_t id);

		bool read_tirette() const;

		uint8_t get_frame_size() const override;

	private:
		SharedIO2019 generate_shared() const override;
		void message_processing(const SharedIO2019&) override;

		void deactivation() override {}

		std::atomic_bool _tirette;
	};
} // namespace PhysicalRobot


#endif // ROOT_MODULEIO2019_H
