/**
 * Cette classe permets d'accéder aux états de contacteurs sur le robot
 * Actuellement, seule la tirette est présente
 * L'IO permets donc de savoir quand commence le match
 */

#ifndef ROOT_MODULEIO_H
#define ROOT_MODULEIO_H

#include "Module.hpp"

namespace PhysicalRobot {

	class IO final : public Module {
	public:
		explicit IO(uint8_t id);

		bool read_tirette() const;

	private:
		std::vector<JSON> generate_list_jsons() const override;
		void message_processing(const JSON&) override;

		void deactivation() override {}

		bool _tirette;
	};
} // namespace PhysicalRobot


#endif // ROOT_MODULEIO_H
