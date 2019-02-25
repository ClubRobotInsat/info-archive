
#ifndef ROOT_NAVIGATIONUTILITY_H
#define ROOT_NAVIGATIONUTILITY_H

namespace PhysicalRobot {

    namespace NavigationUtility {

        uint16_t distance_to_u16(Distance distance) {
            // TODO controller overflow et underflow
            return static_cast<uint16_t>(distance.toMm() * 10);
        }

        Distance u16_to_distance(uint16_t distance) {
            return Distance::makeFromMm(distance / 10.0);
        }

        uint16_t angle_to_u16(Angle angle) {
            return static_cast<uint16_t>(angle.toMilliRad() * 10);
        }

        Angle u16_to_angle(uint16_t angle) {
            return Angle::makeFromMilliRad(angle / 10.0);
        }
    };
}
#endif //ROOT_NAVIGATIONUTILITY_H
