
#ifndef ROOT_NAVIGATIONUTILITY_H
#define ROOT_NAVIGATIONUTILITY_H

#include "Units.h"

namespace PhysicalRobot::NavigationUtility {

	inline int32_t distance_to_i32(Distance distance) {
		// TODO controller overflow et underflow
		return static_cast<int32_t>(distance.toMm() * 10);
	}

	inline Distance i32_to_distance(int32_t distance) {
		return Distance::makeFromMm(distance / 10.0);
	}

	inline int32_t angle_to_i32(Angle angle) {
		return static_cast<int32_t>(angle.toMilliRad() * 10.0);
	}

	inline Angle i32_to_angle(int32_t angle) {
		return Angle::makeFromMilliRad(angle / 10.0);
	}

	inline uint16_t angular_speed_to_u16(AngularSpeed speed) {
		return static_cast<uint16_t>(speed.toMilliRad_s());
	}

	inline uint16_t speed_to_u16(Speed speed) {
		return static_cast<uint16_t>(speed.toM_s() * 1000); // mm/s
	}

} // namespace PhysicalRobot::NavigationUtility

#endif // ROOT_NAVIGATIONUTILITY_H
