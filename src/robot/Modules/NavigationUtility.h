
#ifndef ROOT_NAVIGATIONUTILITY_H
#define ROOT_NAVIGATIONUTILITY_H

namespace PhysicalRobot {

	namespace NavigationUtility {

		inline int32_t distance_to_i32(Distance distance) {
			// TODO controller overflow et underflow
			return static_cast<int32_t>(distance.toMm() * 10);
		}

		inline Distance i32_to_distance(int32_t distance) {
			return Distance::makeFromMm(distance / 10.0);
		}

		inline int32_t angle_to_i32(Angle angle) {
			return static_cast<int32_t>(angle.toMilliRad() * 10);
		}

		inline Angle i32_to_angle(int32_t angle) {
			return Angle::makeFromMilliRad(angle / 10.0);
		}
	}; // namespace NavigationUtility
} // namespace PhysicalRobot
#endif // ROOT_NAVIGATIONUTILITY_H
