//
// Created by abdelsaidt on 2/27/20.
//

#ifndef ROOT_WEBCAM_H
#define ROOT_WEBCAM_H

#include "Module.hpp"

namespace PhysicalRobot {

    ENUM_CLASS_NS(PhysicalRobot, DetectedColors, Red_and_None, Green_and_None, Red_and_Green, Green_and_Red, None_and_Red, None_and_Green, None);
    ENUM_CLASS_NS(PhysicalRobot, DetectedColor, Red, Green);
    ENUM_CLASS_NS(PhysicalRobot, Position, Straight, Inclined);
    ENUM_CLASS_NS(PhysicalRobot, PortCardinal, West, South);

    class Webcam final : public Module {
    public:
        explicit Webcam(uint8_t id, Position position=Position::Straight) :  Module(id, "Webcam"), _position(position) {};
        PhysicalRobot::DetectedColor detectColor() const;
        PhysicalRobot::DetectedColors detectColors() const;
        PhysicalRobot::PortCardinal detectPort() const;

        void setPosition(Position position);


    private:
        std::vector<JSON> generate_list_jsons() const override;
        void message_processing(const JSON&) override;

        void deactivation() override;

        std::atomic<Position> _position;
        std::atomic<Time> _decision_time;

    };
}


#endif //ROOT_WEBCAM_H
