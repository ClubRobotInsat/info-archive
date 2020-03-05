//
// Created by abdelsaidt on 2/27/20.
//

#include "Webcam.h"

    Webcam::Webcam(uint8_t id, Position position=Position::Straight) :  Module(id, "Webcam"), _position(position) {
        _decision_time = 2_s;

    }
