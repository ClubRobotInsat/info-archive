//
// Created by terae on 16/09/18.
//

#ifndef ROOT_MODULEIO2019_H
#define ROOT_MODULEIO2019_H

#include "SharedWithRust.h"

#include "Module.hpp"

namespace Commun {

	class ModuleIO2019 : public Module<SharedIO2019> {
	public:
		explicit ModuleIO2019(uint8_t id);

		bool read_tirette() const;

		inline uint8_t get_frame_size() const override;

	private:
		SharedIO2019 generate_shared() const override;
		void message_processing(const SharedIO2019&) override;

		std::atomic_bool _tirette;
	};
} // namespace Commun


#endif // ROOT_MODULEIO2019_H
