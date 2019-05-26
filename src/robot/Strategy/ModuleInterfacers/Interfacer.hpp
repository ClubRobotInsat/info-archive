//
// Created by terae on 21/01/19.
//

#ifndef ROOT_INTERFACER_H
#define ROOT_INTERFACER_H

#include "../../Robot.h"
#include <ActionResult.h>

namespace Strategy::Interfacer {
	/**
	 * Base of all `Interfacer`s
	 *
	 * Basically, an `Interfacer` is an helper class which provides higher control functions
	 * than `PhysicalRobot::Module`s
	 * For instance, while a module sends non-intelligent commands to electronics, an `Interfacer` aims to make
	 * these commands blocking by reading captors or by using timers
	 *
	 * By the way, the higher-provided functions should return `ActionResult` objects to have a better view.
	 * Finally, these `Interfacer`s are managed by a `GlobalManager` instance.
	 * Its `Interfacer`-building function gives a `shared_ptr<PhysicalRobot::Robot>` object to `Interfacers`
	 * as the first argument so derived-`Interfacer`s need to provide appropriate constructors.
	 */
	class AbstractInterfacer {
	protected:
		AbstractInterfacer() = default;

	public:
		virtual ~AbstractInterfacer() = default;
	};
} // namespace Strategy::Interfacer

#endif // ROOT_INTERFACER_H
