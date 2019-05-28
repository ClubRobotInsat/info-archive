//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <ActionResult.h>

ENUM_CLASS(ElevatorPosition, IDLE, GROUND, TOP_RIGHT, TOP_LEFT)

// @PetriLab@ ServosSecondary

ActionResult raise_arm(Angle);
ActionResult rotate_arm(Angle);
ActionResult elevator(Angle);

ActionResult arm_ground();
ActionResult arm_top_right();
ActionResult arm_top_left();

ActionResult raise_elevator();
ActionResult lower_elevator();

ElevatorPosition get_elevator_position();