//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <Outcome.h>

ENUM_CLASS(ElevatorPosition, IDLE, GROUND, TOP_RIGHT, TOP_LEFT)

// @PetriLab@ ServosSecondary

Outcome raise_arm(Angle);
Outcome rotate_arm(Angle);
Outcome elevator(Angle);

Outcome arm_ground();
Outcome arm_top_right();
Outcome arm_top_left();

Outcome raise_elevator();
Outcome lower_elevator();

ElevatorPosition get_elevator_position();