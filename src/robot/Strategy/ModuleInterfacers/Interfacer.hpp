//
// Created by terae on 21/01/19.
//

#ifndef ROOT_INTERFACER_H
#define ROOT_INTERFACER_H

#include "../../Modules/Servos.h"
#include <ActionResult.h>

namespace Strategy {
	namespace Interfacer {
		class AbstractInterfacer {
		protected:
			AbstractInterfacer() = default;

		public:
			virtual ~AbstractInterfacer() = default;
		};
	} // namespace Interfacer
} // namespace Strategy

#endif // ROOT_INTERFACER_H
